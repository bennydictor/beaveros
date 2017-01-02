NAME	:= kernel
ARCH	:= x86_64-elf
SRC		:= $(SRC)/kernel
OBJ		:= $(OBJ)/kernel
BMAKE	:= .bmake/kernel

include src/bmake.mk
