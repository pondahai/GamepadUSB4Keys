// Simple gamepad example that demonstraits how to read five Arduino
// digital pins and map them to the Arduino Joystick library.
//
// The digital pins 2 - 6 are grounded when they are pressed.
// Pin 2 = UP
// Pin 3 = RIGHT
// Pin 4 = DOWN
// Pin 5 = LEFT
// Pin 6 = FIRE
//
// NOTE: This sketch file is for use with Arduino Leonardo and
//       Arduino Micro only.
//
// by Matthew Heironimus
// 2016-11-24
// mod by Dahai Pon
// 2019-4-9
// add auto fire, auto fire on/off by key0 press when power on
// 2019-6-15 
//--------------------------------------------------------------------

#include <Joystick.h>

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  4, 0,                  // Button Count, Hat Switch Count
  true, true, false,     // X and Y, but no Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering

bool autoFireSwitch = false;

void setup() {
  // Initialize Button Pins
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);

  // Initialize Joystick Library
  Joystick.begin();
  Joystick.setXAxisRange(-1, 1);
  Joystick.setYAxisRange(-1, 1);
  if (digitalRead(6) == LOW) {
    autoFireSwitch = true;
  }
}

// Last state of the buttons
int lastButtonState[8] = {0,0,0,0,0,0,0,0};
int lastAutoButtonState[8] = {0,0,0,0,0,0,0,0};
volatile int autoFireCounter = 0;
#define AFCOUNT 8
void loop() {
  if(autoFireCounter++ == AFCOUNT) autoFireCounter = 0;
  // Read pin values
  for (int index = 0; index < 8; index++)
  {
    int currentButtonState = !digitalRead(index + 2);
    if (currentButtonState != lastButtonState[index])
    {
      switch (index) {
        case 0: // UP
          if (currentButtonState == 1) {
            Joystick.setYAxis(-1);
          } else {
            Joystick.setYAxis(0);
          }
          break;
        case 1: // RIGHT
          if (currentButtonState == 1) {
            Joystick.setXAxis(1);
          } else {
            Joystick.setXAxis(0);
          }
          break;
        case 2: // DOWN
          if (currentButtonState == 1) {
            Joystick.setYAxis(1);
          } else {
            Joystick.setYAxis(0);
          }
          break;
        case 3: // LEFT
          if (currentButtonState == 1) {
            Joystick.setXAxis(-1);
          } else {
            Joystick.setXAxis(0);
          }
          break;
        case 4: // FIRE
          Joystick.setButton(0, currentButtonState);
          break;
        case 5: // FIRE
          Joystick.setButton(1, currentButtonState);
          break;
        case 6: // FIRE
          Joystick.setButton(2, currentButtonState);
          break;
        case 7: // FIRE
          Joystick.setButton(3, currentButtonState);
          break;
      }
      lastButtonState[index] = currentButtonState;
      if(lastAutoButtonState[index] == 1 && currentButtonState == 0)
      {
        switch (index) {
          case 6:
            Joystick.setButton(0, 0);
          break;
          case 7:
            Joystick.setButton(1, 0);
          break;
        }
        
      }
      lastAutoButtonState[index] = currentButtonState;
    }
    for(int index = 0; index < 8; index++){
    if(autoFireSwitch){ // button no change
      if(autoFireCounter==0 && lastAutoButtonState[index]){

        switch (index) {
          case 6:
            Joystick.setButton(0, 0);
          break;
          case 7:
            Joystick.setButton(1, 0);
          break;
        }
      }
      if(autoFireCounter==(AFCOUNT/2) && lastAutoButtonState[index]){
          lastAutoButtonState[index] = 1;
          switch (index) {
            case 6:
              Joystick.setButton(0, 1);
            break;
            case 7:
              Joystick.setButton(1, 1);
            break;
          }
      }
    }
  }
  }

  delay(10);
}