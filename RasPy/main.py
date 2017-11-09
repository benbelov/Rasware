# Python visualizer for the launchpad
#

from livewires import *
import serial

# launchpad data goes in a launchpad object instance
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

        # command dictionary
        self.commands =
        {
            # str name, int r, int g, int b
            "definecolor":["s","d","d","d"],
            # float scale
            "setscale":["f"],
            # int xoffset, in yoffset
            "setoffset":["d","d"],
            # float x_1, float y_1, float x_1, float y_1, str color
            "drawline":["f","f","f","f","s"],
            # float x, float y, float r, str color
            "drawcircle":["f","f","f","s"],
            # float x, float y, str label, str color
            "text":["f","f","s","s"],
            # clear screen
            "clrscrn":[],
            # start drawing
            "start":[],
            # stop drawing
            "stop":[]
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
    def parse_line(code_line):

        # intialize args as an empty array
        arguments = []

        # if a colon is not found, then the opcode has no args       
        opcode_end = code_line.find(":")
        if(opcode_end == -1):
            arguments += code_line

        # else, strip out the opcode
        else:
            arguments += code_line[0:opcode_end]
            previous_arg_end = opcode_end
            current_arg_end = code_line.find[",",opcode_end + 1]
            
            # then separate the arguments
            while(current_arg_end != -1):
                arguments += string[previous_arg_end + 1:current_arg_end]
                previous_arg_end = current_arg_end
                current_arg_end = string.find(",",current_arg_end + 1)
                
            arguments += code_line[previous_arg_end:len(code_line)]

        return(arguments)

    #
    # process arguments into an array
    #
    def process_args(raw_arguments):

        arguments = []
        opcode = raw_arguments[0]
        arguments[0] = raw_arguments[0]

        n = 1
        while(n<len(raw_arguments)):

            argument_type = self.commands[opcode][n-1]
            # integer type
            if(argument_type == "d"):
                arguments[n] = int(raw_arguments[n])
            # string type
            else if(argument_type == "s"):
                arguments[n] = raw_arguments[n]
            # float type
            else if(argument_type == "f"):
                arguments[n] = float(raw_arguments[n])
            # long type
            else if(argument_type == "l"):
                arguments[n] = int(raw_arguments[n]*10000 +
                                   raw_arguments[n+1])
            # empty type, for when one arg takes up two spaces
            else if(argument_type == "_"):
                arguments[n] = 0
            # error type, when the type doesn't match
            else:
                arguments[n] = "ERR"
        
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
        instruction = process_args(parse_line(get_line()))

        if(instruction[0] == "draw"):
            return(True)
        else:
            self.draw_buffer += instruction
            return(False)


    #
    # execute the instruction buffer
    #
    def execute_buffer(self):

        for instruction in self.draw_buffer:

            # definecolor: str color, int r, int g, int b
            if(instruction[0] == "definecolor"):
                self.colors[instruction[1] = (instruction[2],
                                              instruction[3],
                                              instruction[4])

            # setscale: float scale
            else if(instruction[0] == "setscale"):
                self.scale = instruction[1]

            # setoffset: int x, int y
            else if(instruction[0] == "setoffset"):
                self.x_offset = instruction[1]
                self.y_offset = instruction[2]
            
            # drawline: float x_1, float y_1,
            #           float x_1, float y_1, str color
            else if(instruction[0] == "drawline"):
                try:
                    colortuple = self.colors[instruction[5]]
                except:
                    colortuple = self.colors["black"]
                set_colour(colortuple[0],colortuple[1],colortuple[2])
                line(instruction[1],
                     instruction[2],
                     instruction[3],
                     instruction[4]

            # drawcircle: float x, float y, float r, str color
            else if(instruction[0] == "drawcircle"):
                try:
                    colortuple = self.colors[instruction[5]]
                except:
                    colortuple = self.colors["black"]
                set_colour(colortuple[0],colortuple[1],colortuple[2])
                circle(x,y,r)

            # text: float x, float y, str label, str color
            else if(instruction[0] == "text"):
                try:
                    colortuple = self.colors[instruction[4]]
                except:
                    colortuple = self.colors["black"]
                set_colour(colortuple[0],colortuple[1],colortuple[2])
                move(instruction[1],instruction[2])
                text(instruction[3])

            # clrscrn
            else if(instruction[0] == "clrscrn"):
                clear_screen()


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
    launchpad.initialize_buffer()
