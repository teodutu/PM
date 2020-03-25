
all: lab1.hex

lab1.hex: lab1.elf
	avr-objcopy  -j .text -j .data -O ihex $^ $@
	avr-size lab1.elf

lab1.elf: lab1.c
	avr-g++ -mmcu=atmega324p -DF_CPU=12000000 -Os -Wall -o $@ $^

clean:
	rm -rf lab1.elf lab1.hex
