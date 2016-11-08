#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>

void ADC_init(){
	/* Enable ADC */
	ADCSRA |= (1 << ADEN);
	/* Set prescaler */
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
	/* Select channel 0 */
	ADMUX &= ~(1 << MUX0) & ~(1 << MUX1) & ~(1 << MUX2) & ~(1 << MUX3) & ~(1 << MUX4);
	ADCSRB &= ~(1 << MUX5);
	/* Choose reference */
	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << REFS1);
}

uint16_t ADC_convert(){
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1<<ADSC));
	printf("Result: %d\n", ADC & 0x3FF);
	return ADC & 0x3FF;
}