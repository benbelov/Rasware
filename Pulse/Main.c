#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/uart.h>
#include <StellarisWare/driverlib/sysctl.h>
#include <StellarisWare/driverlib/timer.h>
#include <StellarisWare/driverlib/systick.h>

void pulsePin(void){

  SetPin(PIN_B1, true);

  unsigned long ulSysPeriod = SysCtlClockGet();

  SysCtlDelay(ulSysPeriod * 0.000015);

  fPrintf("%lu", ulSysPeriod);

  SetPin(PIN_B1, false);

}

int main(void){

  CallEvery(pulsePin, 0, 5);

  return 0;

}
