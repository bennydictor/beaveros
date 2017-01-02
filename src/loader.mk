NAME	:= loader
ARCH	:= i686-elf
SRC		:= $(SRC)/loader
OBJ		:= $(OBJ)/loader
BMAKE	:= .bmake/loader

include src/bmake.mk
