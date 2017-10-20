#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/uart.h>
#include <stdio.h>
#include <StellarisWare/driverlib/sysctl.h>
#include <StellarisWare/driverlib/rom.h>
#include <StellarisWare/driverlib/timer.h>
#include <StellarisWare/driverlib/systick.h>

tBoolean led_on;

void pulsePin(void){

	SetPin(PIN_F1, led_on);
  SetPin(PIN_F3, !led_on);

  led_on = !led_on;

  unsigned long ulSysPeriod = ROM_SysCtlClockGet();



  int divfive = (int) ulSysPeriod / 1000;

  int delay = (int) ulSysPeriod*0.000005 - 80;

  SetPin(PIN_B1, true);
  ROM_SysCtlDelay(delay);
  SetPin(PIN_B1, false);

  Printf("%d kHz\n", divfive);
  
}

int main(void){

  CallEvery(pulsePin, 0, 1);

  return 0;

}