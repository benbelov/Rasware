from livewires import *
import serial

try:
    launchpad = serial.Serial('/dev/lm4f', 9600, timeout=1)
    print("launchpad connected: lm4f")
except:
    print("no launchpad found.")
    exit()


while 1:
    line = launchpad.readline().strip()
    print(line)
    
