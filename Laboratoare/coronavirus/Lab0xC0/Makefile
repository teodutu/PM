all: lab0.hex

lab0.hex: lab0.elf
	avr-objcopy  -j .text -j .data -O ihex $^ $@
	avr-size lab0.elf

lab0.elf: lab0.c
	avr-g++ -mmcu=atmega324p -DF_CPU=12000000 -Os -Wall -o $@ $^

clean:
	rm -rf lab0.elf lab0.hex
