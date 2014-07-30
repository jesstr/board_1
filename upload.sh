#!/bin/sh

MK=m2561
PRG=usbasp
FLASHFILE=./Debug/board_1.hex

sudo avrdude -p $MK -c $PRG -U flash:w:$FLASHFILE

# Manual run:
# sudo avrdude -p m2561 -c usbasp -U flash:w:"./Debug/board_1.hex":i
