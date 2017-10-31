from livewires import *
import serial

try:
    launchpad = serial.Serial('\dev\tm4c')
    print("launchpad connected: tm4c")
except:
    try:
        launchpad = serial.Serial('\dev\lm4f')
        print("launchpad connected: lm4f")
    except:
        print("no launchpad found.")
        exit()


while 1:
    line = launchpad.readline()
    print(line)
    
