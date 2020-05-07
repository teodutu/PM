
all: lab6.hex

lab6.hex: lab6.elf
	avr-objcopy  -j .text -j .data -O ihex $^ $@
	avr-size lab6.elf

lab6.elf: lab6.c usart.c i2c_master.c
	avr-g++ -mmcu=atmega324p -DF_CPU=16000000 -Os -Wall -o $@ $^

clean:
	rm -rf lab6.elf lab6.hex
