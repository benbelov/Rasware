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
tTime timeout_us = 5000;

void GhettoPrintf(char *message,tTime number){
  Printf("%s: %d.%d\n", message, (int) ((long)number / 10000), (int) ((long)number % 10000));
}

void pulsePin(void){

  
  SetPin(PIN_F1, led_on);
  SetPin(PIN_F3, !led_on);

  led_on = !led_on;

  unsigned long ulSysPeriod = ROM_SysCtlClockGet();

  int divfive = (int) (ulSysPeriod / 1000);

  int delay = (int) (ulSysPeriod*0.000005 - 80);

  
  tTime start_time_1 = 0;
  tTime start_time_2 = 0;
  tTime end_time_1 = 0;
  tTime end_time_2 = 0;
  tTime timeout_time = GetTimeUS() + timeout_us;

  GhettoPrintf("Timeout time",timeout_time);
  GhettoPrintf("Start time",GetTimeUS());
  
  SetPin(PIN_B1, true);
  SetPin(PIN_D1, true);
  ROM_SysCtlDelay(delay);
  SetPin(PIN_B1, false);
  SetPin(PIN_D1, false);

  while ( GetTimeUS() < timeout_time) {
    GhettoPrintf("Current time", GetTimeUS());
    /*if (GetPin(PIN_B2) == true && start_time_1 == 0) {
      start_time_1 = GetTimeUS();
    }
    if (GetPin(PIN_B3) == true && start_time_2 == 0) {
      start_time_2 = GetTimeUS();
    }
    if (GetPin(PIN_B2) == false && start_time_1 != 0) {
      end_time_1 = GetTimeUS();
    }
    if (GetPin(PIN_B3) == false && start_time_2 != 0) {
      end_time_2 = GetTimeUS();
    }
    */
  }

  int time_1 = (int) end_time_1 - start_time_1;
  int time_2 = (int) end_time_2 - start_time_2;
   
  Printf("Time 1: %d us\n", time_1);
  Printf("Time 2: %d us\n", time_2);
  Printf("Clock : %d kHz\n", divfive);
  Printf("\n");
  
}

int main(void){
  
  Printf("hello world\n");
  CallEvery(pulsePin, 0, 1);

  return 0;

}
