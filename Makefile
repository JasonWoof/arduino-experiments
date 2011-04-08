object=blink

all: upload

reset:
	stty --file=/dev/ttyACM0  hupcl
	sleep 0.1
	stty --file=/dev/ttyACM0 -hupcl 

%.elf: %.c
	/usr/bin/avr-gcc -mmcu=atmega328p -I/usr/lib/avr/include -lm -Wl,--gc-sections -Os -o $@ $<

blink.hex: blink.elf
	avr-objcopy -O ihex -R .eeprom $< $@

upload: $(object).hex reset
	/usr/bin/avrdude -q -V -p atmega328p -C /etc/avrdude.conf -c arduino -b 115200 -P /dev/ttyACM0  -U flash:w:$<:i

