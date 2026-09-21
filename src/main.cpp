#include <Adafruit_GFX.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <Wire.h>

namespace {
constexpr int I2C_SDA_PIN = 8;
constexpr int I2C_SCL_PIN = 9;
constexpr uint8_t OLED_ADDRESS = 0x3C;
constexpr uint8_t MPU6050_ADDRESS = 0x68;
constexpr int SCREEN_WIDTH = 128;
constexpr int SCREEN_HEIGHT = 64;
constexpr int OLED_RESET = -1;

// Adafruit reports acceleration in m/s^2 and gyro values in rad/s.
constexpr float RESTING_ACCELERATION_ENTER = 11.5f;
constexpr float RESTING_ACCELERATION_EXIT = 12.5f;
constexpr float MOVING_GYRO_ENTER = 0.55f;
constexpr float RESTING_GYRO_EXIT = 0.35f;
constexpr float HIGH_ACTIVITY_ACCELERATION = 19.0f;
constexpr float HIGH_ACTIVITY_ACCELERATION_EXIT = 17.0f;
constexpr float HIGH_ACTIVITY_GYRO = 2.5f;
constexpr float HIGH_ACTIVITY_GYRO_EXIT = 2.0f;
constexpr uint8_t MOTION_CONFIRMATION_SAMPLES = 3;

// Deterministic prototype fall sequence: free fall -> impact -> observation.
constexpr float FALL_FREEFALL_ACCELERATION = 3.0f;
// Prototype/Wokwi threshold: 18.0 m/s^2 permits approximately 2 g input while
// retaining the required free-fall plus impact sequence; this is not clinically validated.
constexpr float FALL_IMPACT_ACCELERATION = 18.0f;
constexpr float FALL_IMPACT_GYRO_SUPPORT = 2.0f;
constexpr float FALL_POST_IMPACT_MOTION = 13.0f;
constexpr unsigned long FALL_FREEFALL_TO_IMPACT_MS = 1500;
constexpr unsigned long FALL_POST_IMPACT_MS = 1000;
constexpr unsigned long FALL_ALERT_MS = 3000;
constexpr unsigned long SERIAL_DIAGNOSTIC_INTERVAL_MS = 500;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_MPU6050 mpu;

enum class MotionState {
  RESTING,
  MOVING,
  HIGH_ACTIVITY,
  FALL_DETECTED,
};

struct MotionSample {
  sensors_event_t acceleration;
  sensors_event_t gyroscope;
  float accelerationMagnitude;
  float gyroMagnitude;
};

MotionState motionState = MotionState::RESTING;
MotionState pendingMotionState = MotionState::RESTING;
uint8_t pendingMotionSamples = 0;

enum class FallState {
  NORMAL,
  POSSIBLE_FREE_FALL,
  IMPACT_DETECTED,
  POST_IMPACT_CHECK,
  FALL_CONFIRMED,
};

FallState fallState = FallState::NORMAL;
unsigned long fallStateStartedAt = 0;
float impactGyroMagnitude = 0.0f;
unsigned long lastDiagnosticAt = 0;

const char *motionStateName(MotionState state) {
  switch (state) {
    case MotionState::RESTING:
      return "RESTING";
    case MotionState::MOVING:
      return "MOVING";
    case MotionState::HIGH_ACTIVITY:
      return "HIGH ACTIVITY";
    case MotionState::FALL_DETECTED:
      return "FALL DETECTED";
  }

  return "UNKNOWN";
}

const char *fallStateName(FallState state) {
  switch (state) {
    case FallState::NORMAL:
      return "NORMAL";
    case FallState::POSSIBLE_FREE_FALL:
      return "POSSIBLE_FREE_FALL";
    case FallState::IMPACT_DETECTED:
      return "IMPACT_DETECTED";
    case FallState::POST_IMPACT_CHECK:
      return "POST_IMPACT_CHECK";
    case FallState::FALL_CONFIRMED:
      return "FALL_CONFIRMED";
  }

  return "UNKNOWN";
}

bool initializeI2c() {
  if (!Wire.setPins(I2C_SDA_PIN, I2C_SCL_PIN)) {
    return false;
  }

  return Wire.begin();
}

bool initializeDisplay() {
  return display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
}

bool initializeMpu6050() {
  if (!mpu.begin(MPU6050_ADDRESS, &Wire)) {
    return false;
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  return true;
}

bool readMotionSample(MotionSample &sample) {
  sensors_event_t temperature;
  if (!mpu.getEvent(&sample.acceleration, &sample.gyroscope, &temperature)) {
    return false;
  }

  const float x = sample.acceleration.acceleration.x;
  const float y = sample.acceleration.acceleration.y;
  const float z = sample.acceleration.acceleration.z;
  sample.accelerationMagnitude = sqrtf(x * x + y * y + z * z);
  const float gyroX = sample.gyroscope.gyro.x;
  const float gyroY = sample.gyroscope.gyro.y;
  const float gyroZ = sample.gyroscope.gyro.z;
  sample.gyroMagnitude = sqrtf(gyroX * gyroX + gyroY * gyroY + gyroZ * gyroZ);
  return true;
}

MotionState classifyMotionCandidate(const MotionSample &sample) {
  const bool highActivity =
      sample.accelerationMagnitude >= HIGH_ACTIVITY_ACCELERATION ||
      sample.gyroMagnitude >= HIGH_ACTIVITY_GYRO;
  const bool moving = sample.accelerationMagnitude >= RESTING_ACCELERATION_EXIT ||
                      sample.gyroMagnitude >= MOVING_GYRO_ENTER;
  const bool resting = sample.accelerationMagnitude <= RESTING_ACCELERATION_ENTER &&
                       sample.gyroMagnitude <= RESTING_GYRO_EXIT;

  if (motionState == MotionState::HIGH_ACTIVITY) {
    if (sample.accelerationMagnitude >= HIGH_ACTIVITY_ACCELERATION_EXIT ||
        sample.gyroMagnitude >= HIGH_ACTIVITY_GYRO_EXIT) {
      return MotionState::HIGH_ACTIVITY;
    }
  } else if (highActivity) {
    return MotionState::HIGH_ACTIVITY;
  }

  if (motionState == MotionState::MOVING && resting) {
    return MotionState::RESTING;
  }
  if (motionState == MotionState::RESTING && moving) {
    return MotionState::MOVING;
  }
  if (motionState == MotionState::MOVING || moving) {
    return MotionState::MOVING;
  }
  return MotionState::RESTING;
}

void updateFallState(const MotionSample &sample, unsigned long now) {
  switch (fallState) {
    case FallState::NORMAL:
      if (sample.accelerationMagnitude < FALL_FREEFALL_ACCELERATION) {
        fallState = FallState::POSSIBLE_FREE_FALL;
        fallStateStartedAt = now;
        Serial.println("[Fall] POSSIBLE_FREE_FALL");
      }
      break;

    case FallState::POSSIBLE_FREE_FALL:
      if (now - fallStateStartedAt > FALL_FREEFALL_TO_IMPACT_MS) {
        fallState = FallState::NORMAL;
        Serial.println("[Fall] free-fall timed out; reset to NORMAL");
      } else if (sample.accelerationMagnitude >= FALL_IMPACT_ACCELERATION) {
        impactGyroMagnitude = sample.gyroMagnitude;
        fallState = FallState::IMPACT_DETECTED;
        fallStateStartedAt = now;
        Serial.printf("[Fall] IMPACT_DETECTED gyro=%.2f rad/s\n", impactGyroMagnitude);
      } else if (sample.accelerationMagnitude >= FALL_FREEFALL_ACCELERATION) {
        fallState = FallState::NORMAL;
        Serial.println("[Fall] free-fall ended without impact; reset to NORMAL");
      }
      break;

    case FallState::IMPACT_DETECTED:
      fallState = FallState::POST_IMPACT_CHECK;
      fallStateStartedAt = now;
      Serial.println("[Fall] POST_IMPACT_CHECK");
      break;

    case FallState::POST_IMPACT_CHECK:
      if (now - fallStateStartedAt >= FALL_POST_IMPACT_MS) {
        const bool impactHadGyroSupport = impactGyroMagnitude >= FALL_IMPACT_GYRO_SUPPORT;
        const bool postImpactStillMoving =
            sample.accelerationMagnitude >= FALL_POST_IMPACT_MOTION ||
            sample.gyroMagnitude >= MOVING_GYRO_ENTER;
        if (impactHadGyroSupport || postImpactStillMoving) {
          fallState = FallState::FALL_CONFIRMED;
          fallStateStartedAt = now;
          Serial.println("[Fall] FALL_CONFIRMED (prototype/demo logic)");
        } else {
          fallState = FallState::NORMAL;
          Serial.println("[Fall] impact not confirmed; reset to NORMAL");
        }
      }
      break;

    case FallState::FALL_CONFIRMED:
      if (now - fallStateStartedAt >= FALL_ALERT_MS) {
        fallState = FallState::NORMAL;
        Serial.println("[Fall] alert expired; reset to NORMAL");
      }
      break;
  }
}

MotionState updateMotionState(const MotionSample &sample) {
  if (fallState == FallState::FALL_CONFIRMED) {
    pendingMotionSamples = 0;
    return MotionState::FALL_DETECTED;
  }

  const MotionState candidate = classifyMotionCandidate(sample);
  if (candidate == motionState) {
    pendingMotionSamples = 0;
    return motionState;
  }
  if (candidate != pendingMotionState) {
    pendingMotionState = candidate;
    pendingMotionSamples = 1;
  } else if (pendingMotionSamples < MOTION_CONFIRMATION_SAMPLES) {
    ++pendingMotionSamples;
  }
  if (pendingMotionSamples >= MOTION_CONFIRMATION_SAMPLES) {
    motionState = candidate;
    pendingMotionSamples = 0;
  }
  return motionState;
}

void renderStatus(MotionState state) {
  const bool fallAlert = state == MotionState::FALL_DETECTED;
  const bool active = state == MotionState::MOVING ||
                      state == MotionState::HIGH_ACTIVITY;

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("VITALINK");
  display.println("SIH26181");
  display.println("----------------");
  display.println(fallAlert ? "STATUS: ALERT" : active ? "STATUS: ACTIVE" : "STATUS: ONLINE");
  display.print("MOTION: ");
  if (fallAlert) {
    display.println("FALL");
  } else if (state == MotionState::HIGH_ACTIVITY) {
    display.println("HIGH ACT");
  } else {
    display.println(motionStateName(state));
  }
  display.println(fallAlert ? "FALL DETECTED!" : "FALL: NO");
  display.display();
}
}

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("[VitaLink] ESP32 boot");

  if (!initializeI2c()) {
    Serial.println("[VitaLink] I2C initialization failed");
    while (true) {
      delay(1000);
    }
  }
  Serial.println("[VitaLink] I2C initialization complete");

  if (!initializeDisplay()) {
    Serial.println("[VitaLink] OLED initialization failed");
    while (true) {
      delay(1000);
    }
  }
  Serial.println("[VitaLink] OLED initialization complete");

  if (!initializeMpu6050()) {
    Serial.println("[VitaLink] MPU6050 initialization failed");
    while (true) {
      delay(1000);
    }
  }
  Serial.println("[VitaLink] MPU6050 initialization complete at 0x68");

  renderStatus(motionState);
  Serial.println("[VitaLink] Phase 2 status: prototype motion and demo fall detection online");
}

void loop() {
  static unsigned long lastSampleAt = 0;
  const unsigned long now = millis();
  if (now - lastSampleAt < 250) {
    return;
  }
  lastSampleAt = now;

  MotionSample sample;
  if (!readMotionSample(sample)) {
    Serial.println("[VitaLink] MPU6050 read failed");
    return;
  }

  updateFallState(sample, now);
  motionState = updateMotionState(sample);
  renderStatus(motionState);
  if (now - lastDiagnosticAt >= SERIAL_DIAGNOSTIC_INTERVAL_MS) {
    lastDiagnosticAt = now;
    Serial.printf(
        "[Motion] accel m/s^2: X=%.2f Y=%.2f Z=%.2f magnitude=%.2f | "
        "gyro rad/s: X=%.2f Y=%.2f Z=%.2f magnitude=%.2f | motion=%s\n",
        sample.acceleration.acceleration.x,
        sample.acceleration.acceleration.y,
        sample.acceleration.acceleration.z,
        sample.accelerationMagnitude,
        sample.gyroscope.gyro.x,
        sample.gyroscope.gyro.y,
        sample.gyroscope.gyro.z,
        sample.gyroMagnitude,
        motionStateName(motionState));
    Serial.printf("[Fall] state=%s\n", fallStateName(fallState));
  }
}
