#include "List.h"

List::List()
{
	this->record_cnt = 0;
	this->valid_num = 0;
	this->best_point = 0;
}

void List::UpValidNum()
{
	this->valid_num++;
}
uint8_t List::GetValidNum()
{
	return this->valid_num;
}
void List::UpCount()
{
	this->record_cnt++;
}
uint8_t List::GetCount()
{
	return this->record_cnt;
}
void List::SetCount(uint8_t cnt)
{
	this->record_cnt = cnt;
}

char* List::Append(char* record) // "CEW 89" << 이런 식으로 값이 들어옴
{
	uint8_t point = this->atoi(record + 4, 2);
	
	if(point <= 0) // 점수가 0점과 같거나 낮을 경우
	{
		return "SHIT";
	}
	
	memmove(this->records[this->valid_num], record, 7);
	this->records[this->valid_num][6] = '\0';
	this->scores[this->valid_num] = point;
	this->UpValidNum();
	
	return this->records[this->valid_num - 1];
}
void List::Sort()
{
	for (int i = 0; i < this->valid_num - 1; i++)
	{
		uint8_t idx = i;
		for (int j = i + 1; j < this->valid_num; j++)
		{
			if (this->scores[i] < this->scores[j])
			{
				idx = j;
			}
		}
		this->Swap(i, idx);
	}
	
	if (this->scores[0] > 0)
	this->best_point = this->scores[0];
}
void List::Swap(uint8_t a, uint8_t b)
{
	if (a == b)
	return;
	
	int temp_int;
	char temp_char[7];

	memmove(temp_char, this->records[a], 7);
	memmove(this->records[a], this->records[b], 7);
	memmove(this->records[b], temp_char, 7);

	temp_int = this->scores[a];
	this->scores[a] = this->scores[b];
	this->scores[b] = temp_int;
}

void List::SavePoint(uint8_t score)
{
	for(int i = 0; i < valid_num; i++)
	{
		if(this->scores[i] < score)
		{
			
		}
	}
}
char* List::At(uint8_t idx)
{
	return this->records[idx];
}

void List::itoa(int num, char* arr)
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
int List::atoi(char* arr, int cnt)
{
	int ret = 0;
	uint8_t units = cnt;

	for (int i = 0; i < cnt; i++)
	{
		ret += (*(arr + i) - 48) * pow(10, --units);
	}

	return ret;
}
int List::atoi(char* arr)
{
	int ret = 0;
	uint8_t units = 0;
	while (*(arr++) != '\0')
	{
		units++;
	}

	for (int i = 0; i < units; i++)
	{
		ret += (*(arr + i) - 48) * pow(10, units - i - 1);
	}

	return ret;
}

uint8_t List::GetBestPoint()
{
	return this->best_point;
}