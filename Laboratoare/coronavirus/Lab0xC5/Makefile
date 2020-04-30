all: lab5.hex
FLAGS=-Wl,-u,vfprintf -lprintf_flt -lm

lab5.hex: lab5.elf
	avr-objcopy  -j .text -j .data -O ihex $^ $@
	avr-size lab5.elf

lab5.elf: lab5.c spi.c
	avr-g++ -mmcu=atmega324p -DF_CPU=12000000 -Os -Wall $(FLAGS) -o $@ $^

clean:
	rm -rf lab5.elf lab5.hex
