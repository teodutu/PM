PORT ?= /dev/ttyUSB0
#PORT ?= COM14

all: lab0.hex

lab0.hex: lab0.elf
	avr-objcopy  -j .text -j .data -O ihex $^ $@
	avr-size lab0.elf

lab0.elf: lab0.c
	avr-g++ -mmcu=atmega324p -DF_CPU=12000000 -Os -Wall -o $@ $^

upload: lab0.hex
	sudo avrdude -c arduino -P $(PORT) -b 57600 -p atmega324p -U flash:w:$<:a

clean:
	rm -rf lab0.elf lab0.hex
