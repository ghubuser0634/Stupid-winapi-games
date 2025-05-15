#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

struct arrWithLen new_arr(unsigned int len);

struct arrWithLen{
	int length;
	//true for int, false for chars
	bool type;
	int* elements;
};

struct arrWithLen new_arr(unsigned int len){
	struct arrWithLen arr;
	arr.length = len;
	arr.type = true;
	arr.elements = (int*)(calloc(len, sizeof(int)));
	return arr;
}

void arr_set(struct arrWithLen* arr, unsigned int index, int value){

	if(index>(arr->length-1)){
		printf("Fatal error! Index out of bounds!");
	}

	int* to_be_edited = arr->elements;
	for(int i=0; i<index; i++){
		to_be_edited++;
	}
	*to_be_edited = value;
}

//!!!!!!!!!This function might not be safe and accurate!!!!!!!!!
int arr_get(struct arrWithLen arr, unsigned int index){

	int* ptr = arr.elements;
	for(int i=0; i<index; i++){
		ptr++;
	}
	
	return *ptr;

}


void arr_switch_element(struct arrWithLen* arr, unsigned int index_a, unsigned int index_b){

	if(index_a>=arr->length||index_b>=arr->length){
		printf("Fatal error! Index out of bounds!");
	}

	if(index_a==index_b){
		return;
	}

	int a_copy = arr_get(*arr, index_a);
	int b_copy = arr_get(*arr, index_b);

	arr_set(arr, index_a, b_copy);
	arr_set(arr, index_b, a_copy);

}

void arr_shuffle(struct arrWithLen* arr, unsigned int level){

	srand(time(NULL));

	for(int i=0; i<=arr->length*level; i++){
		arr_switch_element(arr, rand()%(arr->length), rand()%(arr->length));
	}

}

void arr_insertion_sort(struct arrWithLen* arr){

	int temp;
	int j;
	
	for(int i=1; i<(arr->length); i++){

		temp = arr_get(*arr, (unsigned int) i);
		j = i - 1;
		
		while(j>=0 && (arr_get(*arr, (unsigned int) j)>temp)){
			int k = arr_get(*arr, (unsigned int) j);
			arr_set(arr, (unsigned int) j+1, k);
			j -= 1;
		}

		arr_set(arr, j+1, temp);

	}
}

unsigned short int get_index(struct arrWithLen arr, int i){
	
	int index = 0;
	int* reader = arr.elements;
	while(*reader!=i){
		index++;
		reader++;
	}
	return index;

}

void print_arr_element(struct arrWithLen arr, unsigned int index){

	int* to_be_printed = arr.elements;
	for(int i=0; i<index; i++){
		to_be_printed++;
	}
	
	if(arr.type){
		printf("%d", *to_be_printed);
	}else{
		printf("%c", *to_be_printed);
	}

}

void print_arr(struct arrWithLen arr){

	printf("[");
	
	for(int i=0; i<(arr.length-1); i++){
		print_arr_element(arr, i);
		printf(", ");
	}

	print_arr_element(arr, arr.length-1);
	
	printf("]\n");
	
}

void free_arr(struct arrWithLen* arr){
	free(arr->elements);
}

/*int main(){
	unsigned int a;
	scanf("%d", &a);

	struct arrWithLen arr1 = new_arr(a);
	for(int i = 0; i<a; i++){
		arr_set(&arr1, i, i+1);
	}

	print_arr(arr1);

	arr_shuffle(&arr1, 4);
	print_arr(arr1);

	arr_insertion_sort(&arr1);
	print_arr(arr1);

	free_arr(&arr1);
	return 0;
}*/
