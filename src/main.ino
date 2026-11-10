
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE

#include <Arduino.h>
#include <DabbleESP32.h>
#include <ESP32Servo.h>

// ==========================================
// DABBLE BLUETOOTH
// ==========================================

#define DABBLE_NAME "ESP32_Robot_Arm"

// ==========================================
// ROBOT ARM SERVO PINS
// ==========================================

#define BASE_PIN 13
#define RIGHT_PIN 25
#define LEFT_PIN 14
#define CLAW_PIN 26

Servo baseServo;
Servo rightMotor;
Servo leftMotor;
Servo clawServo;

// ==========================================
// SERVO CONFIGURATIONS
// ==========================================

// Continuous servo neutral positions
const int RIGHT_STOP = 95;
const int LEFT_STOP = 95;

// Right motor speeds
const int RIGHT_FORWARD = 150;
const int RIGHT_BACKWARD = 50;

// Left motor speeds
const int LEFT_UP = 20;
const int LEFT_DOWN = 100;

// Base positions
const int BASE_CENTER = 90;
const int BASE_LEFT = 110;
const int BASE_RIGHT = 70;

// Claw positions
const int CLAW_OPEN = 120;
const int CLAW_CLOSE = 88;

// ==========================================
// BASE STEP MOVEMENT
// ==========================================

int basePosition = BASE_CENTER;

const int BASE_STEP_SIZE = 1;
const unsigned long BASE_STEP_INTERVAL = 40;

unsigned long lastBaseStep = 0;

// ==========================================
// L298N MOTOR DRIVER PINS
// ==========================================

// Left wheel
#define LEFT_ENA 32
#define LEFT_IN1 33
#define LEFT_IN2 18

// Right wheel
#define RIGHT_ENB 22
#define RIGHT_IN1 19
#define RIGHT_IN2 21

// ==========================================
// RC CAR PWM SETTINGS
// ==========================================

const int PWM_FREQUENCY = 20000;
const int PWM_RESOLUTION = 8;

// Your speed values
const int LEFT_CAR_SPEED = 255;
const int RIGHT_CAR_SPEED = 180;

// ==========================================
// ROBOT ARM FUNCTIONS
// ==========================================

void stopMotors() {
  rightMotor.write(RIGHT_STOP);
  leftMotor.write(LEFT_STOP);
}

void goHome() {
  stopMotors();

  basePosition = BASE_CENTER;
  baseServo.write(BASE_CENTER);

  clawServo.write(CLAW_CLOSE);
}

// ==========================================
// SLOW BASE MOVEMENT
// ==========================================

void moveBaseSlowly(int direction) {

  unsigned long currentTime = millis();

  if (currentTime - lastBaseStep < BASE_STEP_INTERVAL) {
    return;
  }

  lastBaseStep = currentTime;

  if (direction < 0) {
    basePosition -= BASE_STEP_SIZE;

    if (basePosition < BASE_RIGHT) {
      basePosition = BASE_RIGHT;
    }
  }
  else if (direction > 0) {
    basePosition += BASE_STEP_SIZE;

    if (basePosition > BASE_LEFT) {
      basePosition = BASE_LEFT;
    }
  }

  baseServo.write(basePosition);
}

// ==========================================
// L298N MOTOR FUNCTIONS
// ==========================================

void controlMotor(
  int in1,
  int in2,
  int enablePin,
  int speedValue,
  bool forward
) {
  digitalWrite(in1, forward ? HIGH : LOW);
  digitalWrite(in2, forward ? LOW : HIGH);

  ledcWrite(enablePin, speedValue);
}

void stopCar() {
  ledcWrite(LEFT_ENA, 0);
  ledcWrite(RIGHT_ENB, 0);

  digitalWrite(LEFT_IN1, LOW);
  digitalWrite(LEFT_IN2, LOW);

  digitalWrite(RIGHT_IN1, LOW);
  digitalWrite(RIGHT_IN2, LOW);
}

void setupCarDriver() {

  pinMode(LEFT_IN1, OUTPUT);
  pinMode(LEFT_IN2, OUTPUT);

  pinMode(RIGHT_IN1, OUTPUT);
  pinMode(RIGHT_IN2, OUTPUT);

  ledcAttach(
    LEFT_ENA,
    PWM_FREQUENCY,
    PWM_RESOLUTION
  );

  ledcAttach(
    RIGHT_ENB,
    PWM_FREQUENCY,
    PWM_RESOLUTION
  );

  stopCar();
}

// ==========================================
// RC CAR MOVEMENT
// ==========================================

void moveCar(char command) {

  switch (command) {

    case 'F':
      // Forward
      controlMotor(
        LEFT_IN1, LEFT_IN2, LEFT_ENA,
        LEFT_CAR_SPEED, true
      );

      controlMotor(
        RIGHT_IN1, RIGHT_IN2, RIGHT_ENB,
        RIGHT_CAR_SPEED, true
      );
      break;

    case 'B':
      // Backward
      controlMotor(
        LEFT_IN1, LEFT_IN2, LEFT_ENA,
        LEFT_CAR_SPEED, false
      );

      controlMotor(
        RIGHT_IN1, RIGHT_IN2, RIGHT_ENB,
        RIGHT_CAR_SPEED, false
      );
      break;

    case 'L':
      // Pivot left
      controlMotor(
        LEFT_IN1, LEFT_IN2, LEFT_ENA,
        LEFT_CAR_SPEED, false
      );

      controlMotor(
        RIGHT_IN1, RIGHT_IN2, RIGHT_ENB,
        RIGHT_CAR_SPEED, true
      );
      break;

    case 'R':
      // Pivot right
      controlMotor(
        LEFT_IN1, LEFT_IN2, LEFT_ENA,
        LEFT_CAR_SPEED, true
      );

      controlMotor(
        RIGHT_IN1, RIGHT_IN2, RIGHT_ENB,
        RIGHT_CAR_SPEED, false
      );
      break;

    case 'S':
      stopCar();
      break;
  }
}

// ==========================================
// DABBLE CONTROLS
// ==========================================

void controlRobot() {

  // Read directional buttons
  bool up = GamePad.isUpPressed();
  bool down = GamePad.isDownPressed();

  bool left = GamePad.isLeftPressed();
  bool right = GamePad.isRightPressed();

  // Read action buttons
  bool triangle = GamePad.isTrianglePressed();
  bool cross = GamePad.isCrossPressed();

  bool circle = GamePad.isCirclePressed();
  bool square = GamePad.isSquarePressed();

  bool select = GamePad.isSelectPressed();
  bool start = GamePad.isStartPressed();

  // ========================================
  // START: HOME POSITION
  // ========================================

  if (start) {
    stopCar();
    goHome();
    return;
  }

  // ========================================
  // RIGHT SERVO
  // SELECT + UP/DOWN
  // ========================================

  if (select && up && !down) {
    rightMotor.write(RIGHT_FORWARD);
  }
  else if (select && down && !up) {
    rightMotor.write(RIGHT_BACKWARD);
  }
  else {
    rightMotor.write(RIGHT_STOP);
  }

  // ========================================
  // LEFT SERVO
  // TRIANGLE / CROSS
  // ========================================

  if (select) {
    leftMotor.write(LEFT_STOP);
  }
  else if (triangle && !cross) {
    leftMotor.write(LEFT_UP);
  }
  else if (cross && !triangle) {
    leftMotor.write(LEFT_DOWN);
  }
  else {
    leftMotor.write(LEFT_STOP);
  }

  // ========================================
  // BASE SERVO
  // GRADUAL LEFT / RIGHT MOVEMENT
  // ========================================

  if (!up && !down) {

    if (left && !right) {
      moveBaseSlowly(1);
    }
    else if (right && !left) {
      moveBaseSlowly(-1);
    }
  }

  // ========================================
  // CLAW
  // ========================================

  if (circle && !square) {
    clawServo.write(CLAW_OPEN);
  }
  else if (square && !circle) {
    clawServo.write(CLAW_CLOSE);
  }

  // ========================================
  // RC CAR
  // ========================================

  if (select) {
    // SELECT is reserved for arm control
    stopCar();
  }
  else if (up && !down) {

    if (left && !right) {
      moveCar('L');
    }
    else if (right && !left) {
      moveCar('R');
    }
    else {
      moveCar('F');
    }
  }
  else if (down && !up) {

    if (left && !right) {
      moveCar('L');
    }
    else if (right && !left) {
      moveCar('R');
    }
    else {
      moveCar('B');
    }
  }
  else {
    // No driving input: stop car
    stopCar();
  }
}

// ==========================================
// SETUP
// ==========================================

void setup() {

  Serial.begin(115200);

  // Allocate ESP32 servo PWM timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  // Servo frequency
  baseServo.setPeriodHertz(50);
  rightMotor.setPeriodHertz(50);
  leftMotor.setPeriodHertz(50);
  clawServo.setPeriodHertz(50);

  // Attach servos
  baseServo.attach(BASE_PIN, 500, 2400);
  rightMotor.attach(RIGHT_PIN, 500, 2400);
  leftMotor.attach(LEFT_PIN, 500, 2400);
  clawServo.attach(CLAW_PIN, 500, 2400);

  // Initialize arm
  goHome();

  // Initialize L298N
  setupCarDriver();

  // Start Dabble BLE
  Dabble.begin(DABBLE_NAME);

  Serial.println("==============================");
  Serial.println("ROBOT ARM + RC CAR READY");
  Serial.println("==============================");
  Serial.println("Bluetooth: ESP32_Robot_Arm");
  Serial.println("Controller: Dabble GamePad");
}

// ==========================================
// MAIN LOOP
// ==========================================

void loop() {

  // Receive Dabble commands
  Dabble.processInput();

  // Control both car and robot arm
  controlRobot();

  delay(10);
}