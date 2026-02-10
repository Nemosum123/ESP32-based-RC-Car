#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// --- SPEED CONTROL SETTINGS ---
int fullSpeed = 255;
int lowSpeed = 100;

// --- PIN DEFINITIONS (Updated for Per-Motor Wiring) ---

// Front-Left Motor (Front L298N: ENA, IN1, IN2)
#define SPEED_PIN_FL 19 // ENA
#define DIR1_PIN_FL 4   // IN1
#define DIR2_PIN_FL 16  // IN2 (RX2)

// Front-Right Motor (Front L298N: ENB, IN3, IN4)
#define SPEED_PIN_FR 5  // ENB
#define DIR1_PIN_FR 17  // IN3 (TX2)
#define DIR2_PIN_FR 18  // IN4

// Back-Left Motor (Back L298N: ENB, IN3, IN4)
#define SPEED_PIN_BL 25 // ENB
#define DIR1_PIN_BL 33  // IN3
#define DIR2_PIN_BL 32  // IN4

// Back-Right Motor (Back L298N: ENA, IN1, IN2)
#define SPEED_PIN_BR 14 // ENA
#define DIR1_PIN_BR 26  // IN1
#define DIR2_PIN_BR 27  // IN2


// --- STATE TRACKING VARIABLES ---
bool forwardPressed = false;
bool backwardPressed = false;
bool leftPressed = false;
bool rightPressed = false;
bool circlePressed = false;
bool squarePressed = false;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("KHALPAAR_RC_Car");
  
  // Initialize all 12 motor pins as OUTPUTS
  pinMode(SPEED_PIN_FL, OUTPUT); pinMode(DIR1_PIN_FL, OUTPUT); pinMode(DIR2_PIN_FL, OUTPUT);
  pinMode(SPEED_PIN_FR, OUTPUT); pinMode(DIR1_PIN_FR, OUTPUT); pinMode(DIR2_PIN_FR, OUTPUT);
  pinMode(SPEED_PIN_BL, OUTPUT); pinMode(DIR1_PIN_BL, OUTPUT); pinMode(DIR2_PIN_BL, OUTPUT);
  pinMode(SPEED_PIN_BR, OUTPUT); pinMode(DIR1_PIN_BR, OUTPUT); pinMode(DIR2_PIN_BR, OUTPUT);
  
  stopMotors();
}

// --- LOW-LEVEL (PER-MOTOR) FUNCTIONS ---
void setMotorFL(int speed, bool forward) {
  if (forward) {
    digitalWrite(DIR1_PIN_FL, HIGH); digitalWrite(DIR2_PIN_FL, LOW);
  } else {
    digitalWrite(DIR1_PIN_FL, LOW); digitalWrite(DIR2_PIN_FL, HIGH);
  }
  analogWrite(SPEED_PIN_FL, speed);
}

void setMotorFR(int speed, bool forward) {
  if (forward) {
    digitalWrite(DIR1_PIN_FR, HIGH); digitalWrite(DIR2_PIN_FR, LOW);
  } else {
    digitalWrite(DIR1_PIN_FR, LOW); digitalWrite(DIR2_PIN_FR, HIGH);
  }
  analogWrite(SPEED_PIN_FR, speed);
}

void setMotorBL(int speed, bool forward) {
  if (forward) {
    digitalWrite(DIR1_PIN_BL, HIGH); digitalWrite(DIR2_PIN_BL, LOW);
  } else {
    digitalWrite(DIR1_PIN_BL, LOW); digitalWrite(DIR2_PIN_BL, HIGH);
  }
  analogWrite(SPEED_PIN_BL, speed);
}

void setMotorBR(int speed, bool forward) {
  if (forward) {
    digitalWrite(DIR1_PIN_BR, HIGH); digitalWrite(DIR2_PIN_BR, LOW);
  } else {
    digitalWrite(DIR1_PIN_BR, LOW); digitalWrite(DIR2_PIN_BR, HIGH);
  }
  analogWrite(SPEED_PIN_BR, speed);
}


// --- CORE MOTOR FUNCTIONS (HIGH-LEVEL) ---
// These functions now call the low-level functions
void setRightMotors(int speed, bool forward) {
  setMotorFR(speed, forward);
  setMotorBR(speed, forward);
}

void setLeftMotors(int speed, bool forward) {
  setMotorFL(speed, forward);
  setMotorBL(speed, forward);
}

void stopMotors() {
  setRightMotors(0, true);
  setLeftMotors(0, true);
}

// --- "BRAIN" - UPDATED FOR NEW Q/C LOGIC ---
// This function decides what to do based on which buttons are pressed
void evaluateMovement() {
  // Check for combo presses FIRST (Q=Circle, C=Square)
  
  // F and Q or Q and F -> differential right front (Gentle Forward-Right)
  if (forwardPressed && circlePressed) {
    setLeftMotors(fullSpeed, true);
    setRightMotors(lowSpeed, true);
  } 
  // B and Q or Q and B -> differential right back (Gentle Backward-Right)
  else if (backwardPressed && circlePressed) {
    setLeftMotors(fullSpeed, false);
    setRightMotors(lowSpeed, false);
  } 
  // F and C or C and F -> differential left front (Gentle Forward-Left)
  else if (forwardPressed && squarePressed) {
    setRightMotors(fullSpeed, true);
    setLeftMotors(lowSpeed, true);
  } 
  // B and C or C and B -> differential left back (Gentle Backward-Left)
  else if (backwardPressed && squarePressed) {
    setRightMotors(fullSpeed, false);
    setLeftMotors(lowSpeed, false);
  }
  // If no combos, check for single presses
  else if (forwardPressed) {
    setLeftMotors(fullSpeed, true);
    setRightMotors(fullSpeed, true);
  } else if (backwardPressed) {
    setLeftMotors(fullSpeed, false);
    setRightMotors(fullSpeed, false);
  } 
  // Q Only -> Rotate Right (Spin Clockwise)
  else if (circlePressed) { 
    setLeftMotors(fullSpeed, true);
    setRightMotors(fullSpeed, false);
  } 
  // C Only -> Rotate Left (Spin Anti-Clockwise)
  else if (squarePressed) {  
    setRightMotors(fullSpeed, true);
    setLeftMotors(fullSpeed, false);
  } 
  // Handle old Left/Right presses if you still want them to do something, 
  // but they are no longer used for rotation. They will be ignored if 
  // Q or C are also pressed due to the combo checks above.
  // The original rightPressed and leftPressed logic has been effectively replaced.
  else if (rightPressed) { // No functional change, but kept for completeness
    // Car is already spinning with Q, you might want to remove this block.
    // For now, it will default to a stop if Q or C is not pressed.
  } else if (leftPressed) {  // No functional change
    // Car is already spinning with C, you might want to remove this block.
  }
  // If no buttons are pressed, stop
  else {
    stopMotors();
  }
}

// --- MAIN LOOP - NO CHANGES NEEDED HERE ---
void loop() {
  if (SerialBT.available()) {
    char command = SerialBT.read();
    Serial.println(command); // This prints the received character to the Serial Monitor

    switch (command) {
      case 'F': forwardPressed = true; break;
      case 'f': forwardPressed = false; break;
      case 'B': backwardPressed = true; break;
      case 'b': backwardPressed = false; break;
      case 'R': rightPressed = true; break; // R/r are still tracked
      case 'r': rightPressed = false; break;
      case 'L': leftPressed = true; break;  // L/l are still tracked
      case 'l': leftPressed = false; break;
      case 'Q': circlePressed = true; break;
      case 'q': circlePressed = false; break;
      case 'C': squarePressed = true; break;
      case 'c': squarePressed = false; break;
      default: 
        forwardPressed = backwardPressed = leftPressed = rightPressed = circlePressed = squarePressed = false;
        break;
    }
    
    evaluateMovement();
  }
}
