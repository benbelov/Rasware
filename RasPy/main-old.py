# Python visualizer for the launchpad
# Available functions:
#      'definecolor:str color,int r,int g,int b\n'
#             built in: black = 0,0,0; white = 255,255,255
#      'setscale:float scale\n'
#      'setoffset:int x,int y\n'
#      'drawline:float x_1,float y_1,float x_2,float y_2,str color\n'
#      'drawcircle:float x,float y,float r,str color\n'
#      'text:float x,float y,text\n'
#      'clrscrn'
#

from livewires import *
import serial

# launchpad data goes in a launchpad object instance
class tm4c_launchpad:

    def __init__(self, path):
        
        # 115200 baud is a huge improvement over 9600
        try:
            self.device = serial.Serial(path,115200,timeout=1)
            print("launchpad connected: "+path)
        except:
            print("no launchpad found.")
            exit()
            
        # dict for storing color definitions
        self.colors = {"black":(0,0,0),"white":(255,255,255)}

        # set up graphics
        begin_graphics(width=800,height=600,
                       title="Stellaris Launchpad Visual Interface")

        # default values
        self.scale = 1
        self.x_offset = 400
        self.y_offset = 100
        self.current_line = ""

    # get launchpad uart output
    def get_line(self):
        self.current_line = self.device.readline().strip()

    # decide which method to call
    def process_line(self):
        if(self.current_line[0:11] == "definecolor"):
            self.color_update()
        elif(self.current_line[0:8] == "setscale"):
            self.scale_update()
        elif(self.current_line[0:9] == "setoffset"):
            self.set_offset()
        elif(self.current_line[0:8] == "drawline"):
            self.draw_line()
        elif(self.current_line[0:4] == "text"):
            self.text()
        elif(self.current_line[0:7] == "clrscrn"):
            self.clrscrn()
        elif(self.current_line != ""):
            self.unknown_cmd()
            
    # if the 'definecolor' command is called:
    def color_update(self):

        color_name_end = self.current_line.find(",")
        r_end = self.current_line.find(",",color_name_end+1)
        g_end = self.current_line.find(",",r_end+1)
        b_end = len(self.current_line)
        color_name = self.current_line[12:color_name_end]
                
        r_val = float(self.current_line[color_name_end+1:r_end])
        g_val = float(self.current_line[r_end+1:g_end])
        b_val = float(self.current_line[g_end+1:b_end])
        self.colors[color_name] = (r_val,g_val,b_val)
            
    # if the 'setscale' command is called:
    def scale_update(self):
        scale_end = len(self.current_line)
        self.scale = float(self.current_line[9:scale_end])

    # if the 'setoffset' command is called:
    def set_offset(self):
        x_offset_end = self.current_line.find(",")
        y_offset_end = self.current_line.find(",",x_offset_end+1)

        self.x_offset = int(x_offset_end)
        self.y_offset = int(y_offset_end)

    # if the 'drawline' command is called:
    def draw_line(self):
        x1_end = self.current_line.find(",")
        y1_end = self.current_line.find(",",x1_end+1)
        x2_end = self.current_line.find(",",y1_end+1)
        y2_end = self.current_line.find(",",x2_end+1)
        color_end = len(self.current_line)

        x1 = (self.x_offset +
              self.scale*float(self.current_line[9:x1_end]))
        y1 = (self.y_offset +
              self.scale*float(self.current_line[x1_end+1:y1_end]))
        x2 = (self.x_offset +
              self.scale*float(self.current_line[y1_end+1:x2_end]))
        y2 = (self.y_offset +
              self.scale*float(self.current_line[x2_end+1:y2_end]))

        try:
            color = self.colors[self.current_line[y2_end+1:color_end]]

        except:
            color = (0,0,0)
            
        set_colour(Colour(color[0],color[1],color[2]))
        line(x1,y1,x2,y2)

    # if the text command is called:
    def text(self):
        x_end = self.current_line.find(",")
        y_end = self.current_line.find(",",x_end+1)
        text_end = len(self.current_line)

        x = (self.x_offset +
             self.scale*float(self.current_line[5:x_end]))
        y = (self.y_offset +
             self.scale*float(self.current_line[x_end+1:y_end]))
        move(x,y)
        text(self.current_line[y_end+1:text_end])
        
    # if the 'clrscrn' command is called:
    def clrscrn(self):
        clear_screen()

    # if the command isn't recognized:
    def unknown_cmd(self):
        print("unknown command:"+self.current_line+"\n")


# main code
launchpad = tm4c_launchpad('/dev/lm4f')

# wait for the beginning of the cycle
while(1):
    launchpad.get_line()
    print(launchpad.current_line)
    if (launchpad.current_line == "start"):
        break
    
while(1):
    launchpad.get_line()
    launchpad.process_line()
