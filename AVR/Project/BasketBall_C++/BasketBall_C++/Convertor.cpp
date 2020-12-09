#include "Convertor.h"

void Convertor::itoa(int num, char* arr)
{
	int deg = 1;
	int cnt = 0;

	while (1)
	{
		if (num / deg > 0)
		{
			cnt++;
		}
		else
		{
			deg /= 10;
			break;
		}
		deg *= 10;
	}

	for (int i = 0; i < cnt; i++)
	{
		*(arr + i) = num / deg + 48;
		num -= ((num / deg) * deg);
		deg /= 10;
	}
	*(arr + cnt) = '\0';
}

int Convertor::atoi(char* arr)
{
	int ret = 0;
	uint8_t units = 0;
	while (*(arr++) != '\0')
	{
		units++;
	}

	for (int i = 0; i < units; i++)
	{
		ret += (*(arr+i) - 48) * pow(10, units- i - 1);
	}

	return ret;
}