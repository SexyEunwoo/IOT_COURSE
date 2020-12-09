#pragma once
#include <avr/io.h>

void EEPROM_Write(uint16_t addr, uint8_t data)
{
	while ((EECR & (1 << EEWE)));

	EEAR = addr;
	EEDR = data;

	EECR |= (1 << EEMWE);
	EECR |= (1 << EEWE);
}

void EEPROM_Write_Datas(uint16_t addr, char* datas, uint8_t len)
{
	for (int i = 0; i < len; i++)
	{
		while ((EECR & (1 << EEWE)));
		EEPROM_Write(addr + i, datas[i]);
	}

}

uint8_t EEPROM_Read(uint16_t addr)
{
	while ((EECR & (1 << EEWE)));

	EEAR = addr;
	EECR |= (1 << EERE);

	return EEDR;
}