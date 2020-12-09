#pragma once

#include <avr/io.h>
#include <math.h>
#include <string.h>

class List {
private:
	char records[5][7];
	uint8_t scores[5];
	uint8_t record_cnt;
	uint8_t valid_num;
	uint8_t best_point;
	// char* itoa(int num);
	// int atoi(char* arr);
	// int atoi(char* arr, int cnt);

public:
	List();
	char* Append(char* record);
	void Sort();
	void UpCount();
	void UpValidNum();
	uint8_t GetValidNum();
	uint8_t GetCount();
	void SetCount(uint8_t cnt);
	char* At(uint8_t idx);
	void Swap(uint8_t a, uint8_t b);
	void SavePoint(uint8_t score);
	uint8_t GetBestPoint();
	void itoa(int num, char* arr);
	int atoi(char* arr);
	int atoi(char* arr, int cnt);
};