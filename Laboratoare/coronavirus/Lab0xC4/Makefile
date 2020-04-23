all: lab4.hex
FLAGS=-Wl,-u,vfprintf -lprintf_flt -lm

lab4.hex: lab4.elf
	avr-objcopy  -j .text -j .data -O ihex $^ $@
	avr-size lab4.elf

lab4.elf: lab4.c usart.c
	avr-g++ -mmcu=atmega324p -DF_CPU=12000000 -Os -Wall $(FLAGS) -o $@ $^

clean:
	rm -rf lab4.elf lab4.hex
