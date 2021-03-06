# -------------- #
# -- Makefile -- #
# -------------- #

# Copyright (c) 2000-2007 Lionel Lacassagne
# with a little help of Stephane Piskorski and Joel Falcou

# -- Lile list ----------
FILE = main.c mandelbrot.c pi.c nrutil.c vnrutil.c mutil.c

# -- Paths ----------
SRC_PATH = src
OBJ_PATH = obj
EXE_PATH = exe
INC_PATH = include

# -- OS ----------
OS = Linux

# -- Config ----------
# if CONFIG = CLI  (Command Line Interface, no Apple Framework)
CONFIG = CLI

# -- Macros ----------
CC = gcc
AR = ar -rc

# -- Flags ----------
C_DEBUG_FLAGS = -O0 -DDEBUG
C_CC_FLAGS = -std=c99 -DNOALIAS -DALIGNED -DmySSE -DTIMER_POSIX6 -fopenmp
C_OPTIMISATION_FLAGS = -O3 -fstrict-aliasing -DOPENMP
C_ARCH_FLAGS = -msse2 
#-no-vec

C_OS_FLAGS = -D$(OS)
C_CONFIG_FLAGS = -D$(CONFIG)
C_INC_FLAGS = -I$(INC_PATH)

CFLAGS =  $(C_CC_FLAGS) $(C_DEBUG_FLAGS)        $(C_ARCH_FLAGS) $(C_OS_FLAGS) $(C_CONFIG_FLAGS) $(C_INC_FLAGS) $(LIB_INC_PATH) 
CFLAGS = $(C_CC_FLAGS) $(C_OPTIMISATION_FLAGS) $(C_ARCH_FLAGS) $(C_OS_FLAGS) $(C_CONFIG_FLAGS) $(C_INC_FLAGS) $(LIB_INC_PATH)
LDFLAGS = $(C_CC_FLAGS) $(C_OPTIMISATION_FLAGS) $(C_ARCH_FLAGS) $(C_OS_FLAGS) $(C_CONFIG_FLAGS) $(C_INC_FLAGS) $(LIB_LIB_PATH)

# -- Final product ----------
PRODUCT   = tp_SSE.exe

# -- src and obj List ----------
SRC = $(addprefix ${SRC_PATH}/, $(FILE))
OBJ = $(addprefix ${OBJ_PATH}/, $(addsuffix .o, $(basename $(FILE))))

# -- Base rules ----------
$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
   
#-----Main rule ----------
$(EXE_PATH)/$(PRODUCT): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(OPTFLAGS) $(CFG) $(INC) $(LIB) -lm -fopenmp

# -- Other stuff ----------
depend:
	makedepend $(CFLAGS) -Y $(SRC)

clean:
	rm -f $(OBJ)
	rm -f ${LIB_PATH}/${PRODUCT}
	rm -rf *~   *pgm
tar:
	tar -cvf deriche_SSE.tar Makefile src include
	gzip -9 deriche_SSE.tar# DO NOT DELETE
