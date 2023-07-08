# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#

IOP_BIN_DIR = ../
IOP_BIN  = $(IOP_BIN_DIR)hddload.irx
IOP_SRC_DIR = src/
IOP_INC_DIR = src/
IOP_OBJS = hddload.o imports.o
IOP_CFLAGS += -Os -mgpopt -G8192

all: $(IOP_BIN)
$(info $(IOP_BIN))

clean:
	rm -rf $(IOP_OBJS)

include $(PS2SDK)/Defs.make
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.iopglobal
