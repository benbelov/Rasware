#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>

// Blink the LED to show we're on

//This is Mina Gawargious's code from team 25.

int flash = 0;

tPin pins[3] = {PIN_E4, PIN_E5, PIN_B4};

void blink(void) {

  //PIN_B0 RED, PIN_B1 BLUE, PIN_E4 GREEN
  if (flash <= 2){

    SetPin(pins[flash], false);

    SetPin(pins[(flash + 1)%3], true);

    SetPin(pins[(flash + 2)%3], true);

    flash += 1;

  }else{

    flash = 0;

    SetPin(pins[0], true);

    SetPin(pins[1], true);

    SetPin(pins[2], true);

  }

}

// The 'main' function is the entry point of the program
int main(void) {
    // Initialization code can go here
    CallEvery(blink, 0, 0.1);

    /*while (1) {
        // Runtime code can go here
        Printf("Hello World!\n");

    }*/
}
