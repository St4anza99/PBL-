// Posture Checker
// Introduction to IoT - PBL
// Author:
// Date:

/* ───── Library ───── */
#include <NewPing.h> // Handles HC-SR04 timing and distance math

/* ───── PIN DEFINITIONS ───── */
const int triggerPin = 11;
const int echoPin    = 10;
const int ledPin     = 6;
const int buzzerPin  = 5;
const int buttonPin  = 4;    // push-button override (active-LOW)

/* ───── CONSTANTS ───── */
const int maxDistance = 25;   // maximum distance to measure (cm)
const int threshold   = 10;   // considered badPosture (cm)
const int minThreshold = 10;
const int maxThreshold = 15;

/* ───── GLOBAL STATE ───── */
NewPing sonar(triggerPin, echoPin, maxDistance);

bool alertSilenced   = false;   // true → alert muted until posture is corrected 
bool prevButtonState = HIGH;    // for edge detection (HIGH = not pressed)

/* ───── FUNCTION DECLARATIONS ───── */
long checkDistance();
void triggerAlert(bool alert);
void handleButton(bool badPosture);

/* ───── SETUP ───── */
void setup() {
  pinMode(ledPin,    OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);   // internal pull-up
  Serial.begin(9600);
}

/* ───── MAIN LOOP ───── */
void loop() {
  /* 1. Measure distance */
  long distance = checkDistance();     // in cm

  /* 2. Determine if posture is good or bad */
  bool badPosture;
if (distance == 0 || distance < minThreshold || distance > maxThreshold) {
  badPosture = true;
} else {
  badPosture = false;
}



  /* 3. Handle push-button toggle (mutes alert while posture is bad) */
  handleButton(badPosture);

  /* 4. Alert logic */
  if (badPosture && !alertSilenced) {
    triggerAlert(true);                // buzzer + LED ON
  } else {
    triggerAlert(false);               // buzzer + LED OFF
  }

  /* 5. Auto-re-arm alert once posture is corrected */
  if (!badPosture) {
    alertSilenced = false;             // alerts enabled for next incorrect posture event
  }

  delay(200);  // loop delay
}

/* ─────────────────────────────────── */
/* Measure distance (cm) with NewPing  */
long checkDistance() {
  delay(50);                             // sensor settling time
  long dist = sonar.ping_cm();           // 0 if out of range
  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.println(" cm");
  return dist;
}

/* Turn LED & buzzer ON or OFF */
void triggerAlert(bool alert) {
  if (alert) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);  // active buzzer sounds
  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);   // silence buzzer
  }
}

/* Edge-detect push-button and set alertSilenced */
void handleButton(bool badPosture) {
  bool buttonPressed;

  /*Read buttonPin (LOW when pressed) */
  int rawState = digitalRead(buttonPin);
  if (rawState == LOW) {
    buttonPressed = true;
  } else {
    buttonPressed = false;
  }

  /* rising-edge detection: pressed now, was not pressed last loop */
  if (buttonPressed && prevButtonState == HIGH && badPosture) {
    alertSilenced = true;               // mute alert until bowl refilled
    Serial.println("Alert silenced by button.");
  }

  prevButtonState = buttonPressed;      // update state for next loop
}
