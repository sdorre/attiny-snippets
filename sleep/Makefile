DEVICE = attiny85
CLOCK = 8000000
PROGRAMMER = -c usbasp
SRC = sleep
OBJECTS = $(SRC).o
FUSES = -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
	# generated by http://www.engbedded.com/fusecalc/

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE) -b19200
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

all:	%.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:$(SRC).hex:i

fuse:
	$(AVRDUDE) $(FUSES)

install: flash fuse

clean:
	rm -f $(SRC).hex $(SRC).elf $(OBJECTS)

%.elf: $(OBJECTS)
	$(COMPILE) -o $(SRC).elf $(OBJECTS)

%.hex: $(SRC).elf
	rm -f $(SRC).hex
	avr-objcopy -j .text -j .data -O ihex $(SRC).elf $(SRC).hex

disasm:	$(SRC).elf
	avr-objdump -d $(SRC).elf

cpp:
	$(COMPILE) -E $(SRC).c
