#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

int
main (void) {
	uint16_t brightness;

	// set all digital pins for output
	DDRB = 0xff;
	DDRD = 0xff;

	// turn all digital pins off
	PORTB = 0x00;
	PORTD = 0x00;

	while (1) {
		PORTB=0xff;
		_delay_ms(0.05);
		PORTB=0;
		_delay_ms(18.5);
	}

	return 1;
}
