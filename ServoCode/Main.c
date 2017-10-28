#include <RASLib/inc/common.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/time.h>
#include <StellarisWare/driverlib/rom.h>
#include <StellarisWare/driverlib/sysctl.h>

tMotor *left;
tMotor *right;

void initServos(void){

  left = InitializeServoMotor(PIN_E4, false);
  right = InitializeServoMotor(PIN_E5, true);

}

void moveServos(void){


  while(true){

    SetMotor(left, 1.0);
    SetMotor(right, 1.0);
    ROM_SysCtlDelay(ROM_SysCtlClockGet());
    SetMotor(left, 0.2);
    SetMotor(right, 0.2);
    ROM_SysCtlDelay(ROM_SysCtlClockGet());
    SetMotor(left, 0);
    SetMotor(right, 0);
    ROM_SysCtlDelay(ROM_SysCtlClockGet());
    SetMotor(left, -0.2);
    SetMotor(right, -0.2);
    ROM_SysCtlDelay(ROM_SysCtlClockGet());
    SetMotor(left, -1.0);
    SetMotor(right, -1.0);
    ROM_SysCtlDelay(ROM_SysCtlClockGet());

  }

}

int main(void){

  initServos();

  moveServos();

  return 0;

}
