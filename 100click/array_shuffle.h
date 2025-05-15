#include <time.h>
#include <stdlib.h>
#include <stdbool.h>


#ifndef ARRAY_SHUFFLE_H_
#define ARRAY_SHUFFLE_H_

struct arrWithLen{
	int length;
	bool type;
	int* elements;
};

struct arrWithLen new_arr(unsigned int len);

void arr_set(struct arrWithLen* arr, unsigned int index, int value);

int arr_get(struct arrWithLen arr, unsigned int index);

void arr_switch_element(struct arrWithLen* arr, unsigned int index_a, unsigned int index_b);

void arr_shuffle(struct arrWithLen* arr, unsigned int level);

void arr_insertion_sort(struct arrWithLen* arr);

unsigned short int get_index(struct arrWithLen arr, int i);

void print_arr_element(struct arrWithLen arr, unsigned int index);

void print_arr(struct arrWithLen arr);

void free_arr(struct arrWithLen* arr);

#endif