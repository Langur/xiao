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

OBJS  = interrupt.o memory.o consdrv.o command.o
OBJS += reset.o handler.o main.o
OBJS += lib.o serial.o timer.o
OBJS += os.o syscall.o
OBJS += timestamp.o
OBJS += vector.o

TARGET = xiao

CFLAGS = -Wall -mcpu=cortex-m0 -mthumb -nostdinc -nostdlib -fno-builtin
CFLAGS += -I.
#CFLAGS += -g
CFLAGS += -Os
CFLAGS += -DXIAO
CFLAGS += -DCPU_CLOCK=12000000L

LFLAGS = -static -T ld.scr -L.

.SUFFIXES: .c .o
.SUFFIXES: .s .o
.SUFFIXES: .S .o

all :		$(TARGET)

$(TARGET) :	$(OBJS)
		$(CC) $(OBJS) -o $(TARGET) $(CFLAGS) $(LFLAGS)
		cp $(TARGET) $(TARGET).elf
		$(STRIP) $(TARGET)

timestamp.c:
		ruby -e 'print("const char timeStamp[]=\"", Time.now, "\";\n");' > timestamp.c

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
		rm -f $(OBJS) $(TARGET) $(TARGET).elf $(TARGET).mot $(TARGET).hex
		rm timestamp.c
		rm -f *~

