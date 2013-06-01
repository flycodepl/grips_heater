default: compile
compile:
	avr-gcc -mmcu=attiny44 -Os -DF_CPU=1000000UL -Wall main.c -o test --std=c99

run: compile deploy

deploy:
	avr-objcopy -O ihex test test.hex
	avrdude -p t44 -P usb -c usbasp -U flash:w:test.hex -F
read:
	avrdude -p t44 -P usb -c usbasp -U eeprom:r:eeprom:i
