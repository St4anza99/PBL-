// Posture Checker
// Introduction to IoT - PBL
// Author: Kaeo Kloss and Constanza Avila Ramirez
// Date: 5/10/2025

/* ───── Library ───── */
#include <NewPing.h> // Using the NewPing library to help with the sensor stuff

/* ───── PIN DEFINITIONS ───── */
// Telling the Arduino which pins are connected to what
const int triggerPin = 11; // Pin to send the sound wave out
const int echoPin    = 10; // Pin to listen for the echo back
const int ledPin     = 6;  // Pin for the warning LED
const int buzzerPin  = 5;  // Pin for the noisy buzzer
const int buttonPin  = 4;  // Pin for the button to shut the alarm up

/* ───── CONSTANTS ───── */
// Setting some fixed numbers we'll use later
const int maxDistance = 30;   // Furthest distance the sensor should try to measure (in cm)
// *** These numbers decide what's "bad posture" ***
const int closeThreshold = 10; // If closer than this, it's bad (in cm)
const int farThreshold   = 25; // If further than this, it's also bad (in cm)

/* ───── GLOBAL STATE ───── */
// Setting up the sensor using the pins we defined
NewPing sonar(triggerPin, echoPin, maxDistance);

// Variables to keep track of things while the code runs
bool alertSilenced   = false;  // Is the alarm currently silenced by the button? (Starts as false)
int prevButtonState = HIGH;   // Remember if the button was pressed last time we checked (HIGH means not pressed)

/* ───── FUNCTION DECLARATIONS ───── */
// Just listing the functions we'll create later so the Arduino knows about them
long checkDistance();          // Function to measure the distance
void triggerAlert(bool alert); // Function to turn the LED/buzzer on or off
void handleButton(bool badPosture); // Function to check if the button is pressed

/* ───── SETUP ───── */
// This runs once when the Arduino first turns on
void setup() {
  // Tell the Arduino if pins are for input or output
  pinMode(ledPin,    OUTPUT);      // LED pin sends signals out
  pinMode(buzzerPin, OUTPUT);      // Buzzer pin sends signals out
  pinMode(buttonPin, INPUT_PULLUP); // Button pin takes signals in (PULLUP means it's HIGH when not pressed)
  Serial.begin(9600);              // Start communication with the computer (for printing messages)
}

/* ───── MAIN LOOP ───── */
// This part runs over and over again forever
void loop() {
  /* 1. Measure distance */
  long distance = checkDistance(); // Call the function to get the distance in cm

  /* 2. Figure out if posture is good or bad */
  bool badPosture; // Variable to store if posture is bad (true) or good (false)
  // Check if the distance is too close OR too far
  // Also make sure distance isn't 0 (which means the sensor didn't get a reading)
  if ((distance < closeThreshold || distance > farThreshold) && distance != 0) {
    badPosture = true; // Yep, posture is bad
  } else {
    badPosture = false; // Nope, posture is okay (or sensor error)
  }

  /* 3. Check the button */
  handleButton(badPosture); // Call the function to see if the button was pressed

  /* 4. Decide if the alarm should go off */
  // Only make noise/light if posture is bad AND the alarm isn't silenced
  if (badPosture && !alertSilenced) {
    triggerAlert(true); // Turn the alarm on
  } else {
    triggerAlert(false); // Keep the alarm off
  }

  /* 5. Reset the silence button if posture is fixed */
  // If posture is good now, make sure the alarm *can* go off next time it's bad
  if (!badPosture) {
    alertSilenced = false; // Allow alerts again
  }

  delay(200); // Wait a little bit before looping again (so it's not checking constantly)
}

/* ─────────────────────────────────── */
/* Function to measure distance */
long checkDistance() {
  delay(50); // Short pause to let the sensor settle down
  // Ask the NewPing library to send a ping and calculate the distance in cm
  long dist = sonar.ping_cm();
  // Print the distance to the computer screen (Serial Monitor)
  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.println(" cm");
  return dist; // Send the measured distance back to the main loop
}

/* Function to turn the LED & buzzer ON or OFF */
// The 'alert' variable will be true to turn on, false to turn off
void triggerAlert(bool alert) {
  // NOTE: This assumes LOW turns things ON. If your LED/buzzer turn ON with HIGH, swap these.
  if (alert) {
    // Turn things ON
    digitalWrite(ledPin, LOW);    // Set LED pin to LOW
    digitalWrite(buzzerPin, LOW); // Set buzzer pin to LOW
  } else {
    // Turn things OFF
    digitalWrite(ledPin, HIGH);   // Set LED pin to HIGH
    digitalWrite(buzzerPin, HIGH); // Set buzzer pin to HIGH
  }
}

/* Function to handle the button press */
// Needs to know if the posture is currently bad
void handleButton(bool badPosture) {
  // Read the button's current state (LOW if pressed, HIGH if not)
  int currentButtonState = digitalRead(buttonPin);

  // Check if the button was just pressed *down*
  // It needs to be LOW now, but HIGH last time we checked
  // Also, only silence the alarm if the posture is actually bad right now
  if (currentButtonState == LOW && prevButtonState == HIGH && badPosture) {
    alertSilenced = true; // Set the flag to silence the alarm
    Serial.println("Button pressed - Alarm off for now!"); // Print a message
  }

  // Remember the button's current state for the next time through the loop
  prevButtonState = currentButtonState;
}
