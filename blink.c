#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

void delay(uint8_t length) {
	while (--length) {
		_delay_loop_2(6000);
	}
}

void
delay_some() {
	delay(255);
}

void
show_10_bits(uint16_t bits) {
	uint8_t i;
	PORTB = 0x00;
	delay_some();
	delay_some();
	delay_some();
	for(i = 0; i < 10; ++i) {
		PORTB = 0xff;
		delay(30);
		if((bits & 0x200) == 0) {
			PORTB = 0;
		}
		delay(150);
		PORTB = 0;
		delay(250);
		bits <<= 1;
	}
}

void
adc_init() {
	ADMUX=(1<<REFS0); // voltage to Vcc which is hopefully 5v, but maybe requires me to set the reference voltage with wiring?
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //ADEN=enable ADPS[123]=rescalar div factor 128 (frequency thing)

	// some texts suggest a "dummy" read
	ADCSRA |= (1<<ADSC); // activate the analog -> digital converter
	while(!(ADCSRA &(1<<ADIF))) {
		;
	}
	ADCSRA |= (1<<ADIF); // clear (yeah, I know, it's a weird way to clear) the ADIF bit for next time
}

uint16_t
adc_read(channel) {
	uint16_t l, h;

	ADMUX = (ADMUX & 0xf8) | (channel & 0x7); // low 3 bits select read channel

	ADCSRA |= (1<<ADSC); // activate the analog -> digital converter
	while((ADCSRA & (1<<ADSC))) {
		;
	}
	ADCSRA|=(1<<ADIF); // clear (yeah, I know, it's a weird way to clear) the ADIF bit for next time

	l = ADCL; // book said to read the low first
	h = ADCH;

	return (h << 8) | l;
}
 
int
main (void) {

	// set all digital pins for output
	DDRB = 0xff;
	DDRD = 0xff;

	// turn all digital pins on
	PORTD = 0xff;
	PORTD = 0xff;

	// enable a/d conversion
	adc_init();

	while (1) {
		show_10_bits(ADSC);
		//show_10_bits(adc_read(0));
	}

	return 1;
}
