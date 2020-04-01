
all: lab2.hex

lab2.hex: lab2.elf
	avr-objcopy  -j .text -j .data -O ihex $^ $@
	avr-size lab2.elf

lab2.elf: lab2.c usart.c
	avr-g++ -mmcu=atmega324p -DF_CPU=16000000 -Os -Wall -o $@ $^

clean:
	rm -rf lab2.elf lab2.hex
