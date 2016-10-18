#ifndef SPI_H_
#define SPI_H_

void SPI_Init(void);
char SPI_write(char cData);
char SPI_read(void);
void SPI_on(void);
void SPI_off(void);

#endif /* SPI_H_ */