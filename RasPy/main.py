# --------------------------------------------------------------------------
#
# Python visual tool for the stellaris launchpad
#
# --------------------------------------------------------------------------
#
# tm4c_launchpad class information:
#
# self.colors: current list of colors;
#     dictionary with color names as keys,
#     and (r,g,b) tuples as values
# self.scale: current scale; float
# self.x_offset, launchpad.y_offset: x and y offset; integers
#     0,0 is on the bottom left
# self.run: should we take input from UART?; is a bool
# self.draw_buffer: buffer of commands taken from serial
#     is a list [[opcode1,arg1,arg2,...],[opcode2,arg1,arg2,...]...]
#
# --------------------------------------------------------------------------
#
# tm4c_launchpad methods:
#
# self.update: this should be used almost all the time,
#    unless a manual override is desired.
# self.execute_buffer: execute the draw buffer immediately.
# self.get_line, self.parse_line, self.process_line: read serial output,
#    and turn it into the list of the form [opcode,arg1,arg2,...],
#    with each arg cast to the appropriate type.
# self.initialize_buffer: clear the draw buffer
# self.update_buffer: read from serial, process, and add to draw buffer
#
# --------------------------------------------------------------------------

from raspy import *

# base code:
# will work perfectly on its own.
launchpad = tm4c_launchpad('/dev/lm4f')

while(True):
    launchpad.update()
