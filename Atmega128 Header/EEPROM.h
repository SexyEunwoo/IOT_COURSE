#include <avr/io.h>

void EEPROM_Write(uint16_t addr, uint8_t data)
{
	while ((EECR & (1 << EEWE)));

	EEAR = addr;
	EEDR = data;

	EECR |= (1 << EEMWE);
	EECR |= (1 << EEWE);
}

uint8_t EEPROM_Read(uint16_t addr)
{
	while ((EECR & (1 << EEWE)));

	EEAR = addr;
	EECR |= (1 << EERE);

	return EEDR;
}