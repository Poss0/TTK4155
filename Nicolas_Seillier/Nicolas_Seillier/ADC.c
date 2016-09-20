#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/delay.h>

#define ADC 0x1400

uint8_t ADC_Convert(uint8_t channel)
{
	/* Start conversion on the selected channel: write on command pins */
	volatile char* adc = (char *) ADC;
	adc[0] = channel;
	/* Wait for the conversion to be performed */
	_delay_ms(2);
	/* Read the result */
	return adc[0];
}
