#include <avr/io.h>

void SPI_Init(void)
{
	/* Set MOSI and SCK output, all others input */
	DDRB |= (1<<DDB4)| (1<<DDB5)|(1<<DDB7);
	PORTB |= (1<<4);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SPI_write(char cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while (!(SPSR & (1<<SPIF)));
}

char SPI_read(void)
{
	/* Start transmission */
	SPDR = 0x00;
	/* Wait for transmission complete */
	while (!(SPSR & (1<<SPIF)));
	return SPDR;
}

void SPI_on(void)
{
	PORTB &= 0xEF;
}

void SPI_off(void)
{
	PORTB |= (1<<4);
}