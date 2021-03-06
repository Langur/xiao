PREFIX  = /usr/local
ARCH    = arm-none-eabi
BINDIR  = $(PREFIX)/$(ARCH)/bin
ADDNAME = $(ARCH)-

AR      = $(BINDIR)/$(ADDNAME)ar
AS      = $(BINDIR)/$(ADDNAME)as
CC      = $(BINDIR)/$(ADDNAME)gcc
LD      = $(BINDIR)/$(ADDNAME)ld
NM      = $(BINDIR)/$(ADDNAME)nm
OBJCOPY = $(BINDIR)/$(ADDNAME)objcopy
OBJDUMP = $(BINDIR)/$(ADDNAME)objdump
RANLIB  = $(BINDIR)/$(ADDNAME)ranlib
STRIP   = $(BINDIR)/$(ADDNAME)strip

## Auto generate
OBJS += kernel_cfg.o kernel_id.h

## Core
OBJS += reset.o main.o interrupt.o handler.o task.o service_call.o

## API
OBJS += act_tsk.o cre_tsk.o def_inh.o ext_tsk.o

## Task
OBJS += idle.o task1.o task2.o

## Library
OBJS += lib.o timestamp.o

## Driver
OBJS += serial.o
OBJS += vector.o

TARGET = xiao

CFLAGS = -Wall -mcpu=cortex-m0 -mthumb -nostdinc -nostdlib -fno-builtin
CFLAGS += -I.
#CFLAGS += -g
CFLAGS += -Os
CFLAGS += -DXIAO
CFLAGS += -DCPU_CLOCK=12000000L

LFLAGS = -static -T ld.scr -L.

CFGFLAGS = -E -x c

.SUFFIXES: .c .o
.SUFFIXES: .s .o
.SUFFIXES: .S .o

all :		$(TARGET)

$(TARGET) :	$(OBJS)
		$(CC) $(OBJS) -o $(TARGET) $(CFLAGS) $(LFLAGS)
		cp $(TARGET) $(TARGET).elf
		$(STRIP) $(TARGET)

timestamp.c:
		ruby -e 'print("const char timestamp[]=\"", Time.now, "\";\n");' > timestamp.c

kernel_cfg.c kernel_id.h: system.cfg
		$(CC) $(CFGFLAGS) $(CFLAGS) system.cfg > system.i
		ruby configure.rb

.c.o :		$<
		$(CC) -c $(CFLAGS) $<

.s.o :		$<
		$(CC) -c $(CFLAGS) $<

.S.o :		$<
		$(CC) -c $(CFLAGS) $<

$(TARGET).mot :	$(TARGET)
		$(OBJCOPY) -O srec $(TARGET) $(TARGET).mot

$(TARGET).hex :	$(TARGET).mot
		$(OBJCOPY) -I srec -O ihex $(TARGET).mot $(TARGET).hex

image :		$(TARGET).hex
		lpc21isp xiao.hex /dev/ttyUSB0 115200 12000
clean :
		rm -f $(OBJS) $(TARGET) $(TARGET).elf $(TARGET).mot $(TARGET).hex kernel_cfg.c kernel_id.h timestamp.c system.i
		rm -f *~

config:		task.cfg
		ruby configure.rb

