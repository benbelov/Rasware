from livewires import *
import serial
import math

# the class where it all happens.
# add commands by adding the appropriate profile to launchpad.commands,
# then adding the code to launchpad.execute_buffer()
class tm4c_launchpad:

    #
    # initialization
    #
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
        self.run = False
        self.draw_buffer = []

        # command dictionary
        self.commands = {
            # str name, int r, int g, int b
            "definecolor":["s","d","d","d"],
            # float scale
            "setscale":["f"],
            # int xoffset, in yoffset
            "setoffset":["d","d"],
            # float x_1, float y_1, float x_2, float y_2, str color
            "drawline":["f","f","f","f","s"],
            # float x, float y, float r, str color
            "drawcircle":["f","f","f","s"],
            # float x, float y, float r, float theta, str color
            "drawray":["f","f","f","f","s"],
            # float x, float y, str label, str color
            "text":["f","f","s","s"],
            # str message
            "echo":["s"],
            # float message with multiple parts
            "echol":["l","_"],
            # clear screen
            "clrscrn":[],
            # start drawing
            "start":[],
            # stop drawing
            "stop":[],
            # draw current buffer
            "draw":[]
        }

    #
    # get launchpad uart output
    #
    def get_line(self):
        return(self.device.readline().strip())

    #
    # parse command into opcode and arguments
    #
    def parse_line(self,code_line):
        
        # intialize args as an empty array
        arguments = []
        
        # if a colon is not found, then the opcode has no args       
        opcode_end = code_line.find(":")
        if(opcode_end == -1):
            arguments.append(code_line)

        # else, strip out the opcode
        else:
            arguments.append(code_line[0:opcode_end])
            previous_arg_end = opcode_end
            current_arg_end = code_line.find(",",opcode_end + 1)
            
            # then separate the arguments
            while(current_arg_end != -1):
                arguments.append(code_line[previous_arg_end+1:
                                           current_arg_end])
                previous_arg_end = current_arg_end
                current_arg_end = code_line.find(",",current_arg_end + 1)
                
            arguments.append(code_line[previous_arg_end+1:len(code_line)])

        return(arguments)

    #
    # process arguments into an array
    #
    def process_args(self,raw_arguments):

        arguments = []
        opcode = raw_arguments[0]
        arguments.append(raw_arguments[0])

        n = 1
        while(n<len(raw_arguments)):

            try:
                argument_type = self.commands[opcode][n-1]
            except:
                print("Error: unrecognized opcode")
                print(opcode)
                argument_type = "ERR"
                
            # integer type
            if(argument_type == "d"):
                try:
                    arguments.append(int(raw_arguments[n]))
                except:
                    arguments.append(0)
            # string type
            elif(argument_type == "s"):
                arguments.append(raw_arguments[n])
            # float type
            elif(argument_type == "f"):
                try:
                    arguments.append(float(raw_arguments[n]))
                except:
                    arguments.append(0)
            # long type
            elif(argument_type == "l"):
                try:
                    arguments.append(int(raw_arguments[n])*1000 +
                                     int(raw_arguments[n+1]))
                except:
                    arguments.append(0)
            # empty type, for when one arg takes up two spaces
            elif(argument_type == "_"):
                arguments.append(0)
            # error type, when the type doesn't match
            else:
                arguments.append("ERR")
                
            n += 1
                
        return(arguments)

    #
    # decide what to do
    #
    def update(self):
        is_frame_draw = self.update_buffer()
        if(is_frame_draw):
            self.execute_buffer()
            self.initialize_buffer()
        
    #
    # initialize (clear) instruction buffer
    #
    def initialize_buffer(self):
        self.draw_buffer = []

    #
    # update the instruction buffer with the current instruction
    #
    def update_buffer(self):
        instruction = self.parse_line(self.get_line())
        
        if(instruction[0] == "draw"):
            return(True)
        elif(instruction[0] == "start"):
            self.run = True
            return(False)
        elif(instruction[0] == "stop"):
            self.run = False
            return(False)
        elif(self.run == True):
            self.draw_buffer.append(self.process_args(instruction))
            return(False)

    #
    # execute the instruction buffer
    #
    def execute_buffer(self):

        for instruction in self.draw_buffer:

            # definecolor: str color, int r, int g, int b
            if(instruction[0] == "definecolor"):
                self.colors[instruction[1]] = (instruction[2],
                                               instruction[3],
                                               instruction[4])

            # setscale: float scale
            elif(instruction[0] == "setscale"):
                self.scale = instruction[1]

            # setoffset: int x, int y
            elif(instruction[0] == "setoffset"):
                self.x_offset = instruction[1]
                self.y_offset = instruction[2]
            
            # drawline: float x_1, float y_1,
            #           float x_1, float y_1, str color
            elif(instruction[0] == "drawline"):
                try:
                    colortuple = self.colors[instruction[5]]
                except:
                    colortuple = self.colors["black"]
                set_colour(Colour(colortuple[0],
                                  colortuple[1],
                                  colortuple[2]))
                line(self.scale*instruction[1] + self.x_offset,
                     self.scale*instruction[2] + self.y_offset,
                     self.scale*instruction[3] + self.x_offset,
                     self.scale*instruction[4] + self.y_offset)
                
            # drawcircle: float x, float y, float r, str color
            elif(instruction[0] == "drawcircle"):
                try:
                    colortuple = self.colors[instruction[4]]
                except:
                    colortuple = self.colors["black"]
                set_colour(Colour(colortuple[0],
                                  colortuple[1],
                                  colortuple[2]))
                circle(self.scale*instruction[1] + self.x_offset,
                       self.scale*instruction[2] + self.y_offset,
                       self.scale*instruction[3])

            # drawray: float x, float y, float r, float theta, str color
            elif(instruction[0] == "drawray"):
                try:
                    colortuple = self.colors[instruction[5]]
                except:
                    colortuple = self.colors["black"]
                set_colour(Colour(colortuple[0],
                                  colortuple[1],
                                  colortuple[2]))
                line(self.scale*instruction[1] + self.x_offset,
                     self.scale*instruction[2] + self.y_offset,
                     self.scale*instruction[3]*math.cos(instruction[4]) + self.x_offset,
                     self.scale*instruction[3]*math.sin(instruction[4]) + self.y_offset)
                
            # text: float x, float y, str label, str color
            elif(instruction[0] == "text"):
                try:
                    colortuple = self.colors[instruction[4]]
                except:
                    colortuple = self.colors["black"]
                set_colour(Colour(colortuple[0],
                                  colortuple[1],
                                  colortuple[2]))
                move(self.scale*instruction[1] + self.x_offset,
                     self.scale*instruction[2] + self.y_offset)
                text(instruction[3])

            # echo: str message
            elif(instruction[0] == "echo"):
                print(instruction[1])

            # echof: message with two parts
            elif(instruction[0] == "echol"):
                print(instruction[1])
                
            # clrscrn
            elif(instruction[0] == "clrscrn"):
                clear_screen()
