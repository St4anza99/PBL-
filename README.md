# PBL-Intro2IoT
This document describes a Project-Based Learning (PBL) activity for the Introduction to IoT course offered by Dawson College’s Electrical Engineering Technology Department.

# Smart Posture Checking system

> A quick, Arduino‐based IoT project that sounds a buzzer when you slouch in your chair or lean too far back. A one-press mute that automatically re-arms the system after you manually disable the notification for whatever reason you may need to. When in a good sitting position, the LED stays on.

![image](https://github.com/user-attachments/assets/664d19bd-f245-47af-b758-1abd95a56512)


---

# Features
#
**Ultrasonic sensing** | HC-SR04 checks back and neck position every 200 ms
**Active buzzer + LED** | Instant visual & audible alert for good or bad posture
**Push-button override** | One press silences the alert until user wants to maintain good posture again
**Self-reset** | Alarm re-enables automatically once bad posture is detected again
**Minimal parts** | No extra resistors except the LED’s 330 Ω series resistor
**Open-source** | GNU-licensed code, schematic and report included

---

## 🛠 Bill of Materials

| Qty | Part | Notes |
|-----|------|-------|
| 1 | **Arduino Uno / Nano** | 5 V logic. |
| 1 | **HC-SR04** ultrasonic sensor | Trigger pin → D11, Echo → D10. |
| 1 | **Active piezo buzzer** | + → D5, − → GND. |
| 1 | **5 mm red LED** | Anode → **330 Ω** → D6, cathode → GND. |
| 1 | **Momentary push-button** | One leg → D4, other → GND. |
| — | Jumper wires & breadboard | Standard half-size board is fine. |

---

## 🔌 Circuit

![image](https://github.com/user-attachments/assets/79252ad5-d134-4d63-843d-6f1e28112c89)




