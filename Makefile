# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2022, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.

EE_BIN_PKD = HDDLOADMAN.ELF
EE_BIN = unpacked_$(EE_BIN_PKD)

KERNEL_NOPATCH = 1 
NEWLIB_NANO = 1
DUMMY_TIMEZONE = 0
DUMMY_LIBC_INIT = 1

IOP_BINS = ps2dev9.o ps2atad.o hddload.o
EE_OBJS = main.o HDDSupport.o $(IOP_BINS)
EE_CFLAGS += -fdata-sections -ffunction-sections
EE_LDFLAGS += -Wl,--gc-sections
EE_INCS += -Iinclude
EE_LIBS += -lpatches

ifeq ($(DUMMY_TIMEZONE), 1)
   EE_CFLAGS += -DDUMMY_TIMEZONE
endif

ifeq ($(DUMMY_LIBC_INIT), 1)
   EE_CFLAGS += -DDUMMY_LIBC_INIT
endif

ifeq ($(KERNEL_NOPATCH), 1)
   EE_CFLAGS += -DKERNEL_NOPATCH
endif

ifeq ($(DEBUG), 1)
  EE_CFLAGS += -DDEBUG -O0 -g
else 
  EE_CFLAGS += -Os
  EE_LDFLAGS += -s
endif

all: $(EE_BIN_PKD)

clean:
	rm -rf $(EE_OBJS) $(EE_BIN) $(EE_BIN_PKD) iop/*.irx

$(EE_BIN_PKD): $(EE_BIN)
	$(EE_STRIP) $<
	ps2-packer $< $@

EE_OBJS_DIR = obj/
EE_SRC_DIR = src/
EE_ASM_DIR = asm/

EE_OBJS := $(EE_OBJS:%=$(EE_OBJS_DIR)%) # remap all EE_OBJ to obj subdir

$(BINDIR):
	@mkdir -p $@

$(EE_OBJS_DIR)%.o: $(EE_SRC_DIR)%.c
	$(DIR_GUARD)
	@echo "  - $@"
	@$(EE_CC) $(EE_CFLAGS) $(EE_INCS) -c $< -o $@

$(EE_OBJS_DIR)%.o: $(EE_ASM_DIR)%.s
	$(DIR_GUARD)
	@echo "  - $@"
	@$(EE_AS) $(EE_ASFLAGS) $< -o $@

IRXTAG = $(notdir $(addsuffix _irx, $(basename $<)))
vpath %.irx $(PS2SDK)/iop/irx/
vpath %.irx iop/
vpath %.irx iop/HDDLOAD

$(EE_ASM_DIR)%.s: %.irx
	@$(DIR_GUARD)
	@bin2s $< $@ $(IRXTAG)

hddload.irx: iop/HDDLOAD
	$(MAKE) -C $< clean all

# Include makefiles
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
