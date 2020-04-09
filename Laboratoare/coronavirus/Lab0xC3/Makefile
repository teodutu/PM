all: lab3.hex
FLAGS=-Wl,-u,vfprintf -lprintf_flt -lm

lab3.hex: lab3.elf
	avr-objcopy  -j .text -j .data -O ihex $^ $@
	avr-size lab3.elf

lab3.elf: lab3.c usart.c
	avr-g++ -mmcu=atmega324p -DF_CPU=12000000 -Os -Wall $(FLAGS) -o $@ $^

clean:
	rm -rf lab3.elf lab3.hex *.PWI *.SDF
