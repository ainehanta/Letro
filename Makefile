VPATH = ./src ./bin ./
TARGET = Letro

SRCS = app.c lcd.c led.c main.c menu.c sound.c switch.c swreset.c tetris.c gomoku.c osero.c
OBJS = $(SRCS:.c=.o)
OBJS_PATH = $(SRCS:%.c=$(DEST_DIR)/%.o)
DEST_DIR = ./bin
GIT_DIR = D:\programming\github\Letro

MCU = atmega1284p
F_CPU = 16000000UL
FUSES = FFFFFF #LOW HIGH EXTEND
PROGRAMMER = atprogram

OPT = 2
CC = avr-gcc
CFLAGS = -O$(OPT) -g -mmcu=$(MCU) -DF_CPU=$(F_CPU) -D_BUILD_
OBJCOPY = avr-objcopy
OBJCOPY_FLAGS = -O ihex -R .eeprom

ATPROGRAM = atprogram
ATPROGRAM_FLAGS = -t $(TARGET_TOOL) -i $(PROGRAM_INTERFACE) -v
ATPROGRAM_TOOL = avrdragon
ATPROGRAM_INTERFACE = isp

AVRDUDE = avrdude
AVRDUDE_FLAGS =
AVRDUDE_TOOL = dragon_isp 

.c.o : 
	$(CC) $(CFLAGS) -c -o $(DEST_DIR)/$@ $<

all : clean $(TARGET).hex clean_o
	
clean :
	rm -f $(OBJS_PATH) $(DEST_DIR)/$(TARGET).hex $(DEST_DIR)/$(TARGET).elf
	
clean_o :
	rm -f $(OBJS_PATH)
	
program : $(TARGET).hex
	$(ATPROGRAM) $(ATPROGRAM_FLAGS) program --verify -c -fl -f $(DEST_DIR)/$(TARGET).hex
	
program_fuses :
	$(ATPROGRAM) $(ATPROGRAM_FLAGS) write --verify -fs --values $(FUSES)
	

$(TARGET).elf : $(OBJS)
	$(CC) $(CFLAGS) -o $(DEST_DIR)/$@ $(OBJS_PATH)
	
$(OBJS_PATH) : $(SRCS)

$(TARGET).hex : $(TARGET).elf
	$(OBJCOPY) $(OBJCOPY_FLAGS) -O ihex -R .eeprom $(DEST_DIR)/$< $(DEST_DIR)/$@
	
app.c : app.h
lcd.c : lcd.h
led.c : led.h
main.c :
menu.c : menu.h
sound.c : sound.h
switch.c : switch.h
swreset.c : swreset.h
osero.c	:	osero.h
tetris.c : tetris.h

.PHONY : clean clean_o
