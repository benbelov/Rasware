/*#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RasLiB/inc/adc.h>
#include "Switch.h"

// Blink the LED to show we're on
tBoolean blink_on = true;

void blink(void) {
    SetPin(PIN_F3, blink_on);
    blink_on = !blink_on;
}

// The 'main' function is the entry point of the program
int main(void) {
    // Initialization code can go here
    CallEvery(blink, 0, 0.5);

    sensor jim = LineSensor(tPin p0, tPin p1, tPin p2, tPin p3, tPin p4, tPin p5, tPin p6, tPin p7);

    while (1) {
        // Runtime code can go here
        //Printf("Hello World!\n");

        PrintSwitch(PIN_DO);
    }
}*/

#include <RASLib/inc/time.h>

#include <RASLib/inc/common.h>
#include <RASLib/inc/servo.h>

tServo *micro;
tBoolean blink_on = true;

void blink(void) {
    SetPin(PIN_F3, blink_on);
    blink_on = !blink_on;
}

void initServo(void){

  micro = InitializeServo(PIN_B1);

}

void moveServo(void){

  initServo();
  //CallEvery(blink, 0, 0.5);

  //char characterInput = Getc();

  while(1){
    blink();
    SetServo(micro, 1.0);
    Wait(2);
    SetServo(micro, 0);
  }

}

int main(void){

  while(1){

    SetPin(PIN_F0, true);

    moveServo();

  }

  return 0;

}
