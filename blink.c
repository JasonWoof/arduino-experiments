#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

void
delay_some() {
	uint8_t counter = 0;
	while (counter != 255) {
		_delay_loop_2(3000);
		counter++;
	}
}

void
show_16_bits(uint16_t bits) {
	uint8_t i;
	DDRD = 0x00;
	delay_some();
	delay_some();
	delay_some();
	for(i = 0; i < 16; ++i) {
		DDRD = 0x00;
		delay_some();
		// 0x04 is the yellow always pulser
		// 0x08 is the red data pulser
		DDRD = 0x04 | ((bits & 1) << 3); // move bit 
		delay_some();
		bits >>= 1;
	}
}

 
int
main (void) {
	uint16_t counter;
	uint16_t interval = 100;

	/* set all digital pins for output*/
	DDRB = 0xff;
	DDRD = 0xff;

	/* turn on pins 0-7 */
	PORTD = 0xff;

	// enable a/d conversion
	ADCSRA |= 0xc3;
	
	while(!(ADCSRA * 0x10)) {
		;
	}
	ADCSRA |= 0x10;

	while (1) {
		/* set PORTB.2 high */
		PORTB = 0x20;
		// PORTD = 0x00;

		ADMUX = 0x80; // pick the channel
		ADCSRA |= (0x43); // activate the analog -> digital converter (can |3 here to scale down by 8x)
		while(!(ADCSRA & 0x10)) {
			;
		}
		ADCSRA |= 0x10; // ?? reset the "done" bit (for next time?)
		interval = ADCL; // book said to read the low first
		interval |= ((uint16_t)ADCH << 8);

		show_16_bits(interval);
	}

	return 1;
}
