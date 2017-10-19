#include <RASLib/inc/common.h>
#include <RASLib/inc/motor.h>

tMotor *left;
tMotor *right;

void initServos(void){

  left = InitializeServoMotor(PIN_B1, true);
  right = InitializeServoMotor(PIN_B2, true);

}

void moveServos(void){

  char characterInput = Getc();

  while(characterInput != 's'){

    SetMotor(left, 1.0);
    SetMotor(right, -1.0);

  }

}

int main(void){

  initServos();

  moveServos();

  return 0;

}
