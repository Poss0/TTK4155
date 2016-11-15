#include <avr/io.h>

void SPI_init(void)
{
	/* Set CS, MOSI and SCK output, MISO input */
	DDRB |= (1<<PB4)| (1<<PB5)|(1<<PB7);
	DDRB &= ~(1<<PB6);
	PORTB |= (1<<4);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SPI_write(char cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while (!(SPSR & (1<<SPIF)))
		;
}

char SPI_read(void)
{
	/* Start transmission */
	SPDR = 0x00;
	/* Wait for transmission complete */
	while (!(SPSR & (1<<SPIF)))
		;
	return SPDR;
}

void SPI_on(void)
{
	PORTB &= ~(1<<PB4);
}

void SPI_off(void)
{
	PORTB |= (1<<PB4);
}