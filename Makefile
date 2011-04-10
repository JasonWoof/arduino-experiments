object=blink

all: $(object).upload


%.elf: %.c
	/usr/bin/avr-gcc -mmcu=atmega328p -DF_CPU=16000000 -I/usr/lib/avr/include -lm -Wl,--gc-sections -Os -o $@ $<

%.hex: %.elf
	avr-objcopy -O ihex -R .eeprom $< $@

%.upload: %.hex
	stty -F /dev/ttyACM0 hupcl && sleep 0.01 && stty -F /dev/ttyACM0 -hupcl
	/usr/bin/avrdude -qqV -p atmega328p -c arduino -b 115200 -P /dev/ttyACM0  -U flash:w:$<:i

