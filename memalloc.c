#include  <stdlib.h>
#include  <stdio.h>
#include  <limits.h>
#include  "memalloc.h"

//endlerde sorun cikabilir
// *2lerde sorun cikabilir
// place values 
// start -- chunkpointer test

typedef struct alloc{
	int length;
	struct alloc * next;
	struct alloc * prev;
}Alloc;

int allocSize = sizeof(Alloc);
int methd;
void* start = NULL;
int size = 0;
void* end = NULL;

//	printfs	are	for	debugging;	remove	them	when	you	use/submit	your	library
int	mem_init (void	*chunkpointer,	int	chunksize,	int	method){
	methd = method; 
	start = chunkpointer;
	size = chunksize; 
	end = start + (size * 1024);

	((Alloc*)start)->length = 0;
	((Alloc*)start)->next = NULL;
	((Alloc*)start)->prev = NULL;

	return	(0); //	if	success
}

void *mem_allocate(int	objectsize){

	if(objectsize < size){
		if(methd == FIRST_FIT){
			return firstFit(objectsize);
		}
		else if(methd == BEST_FIT){
			return bestFit(objectsize);
		}
		else if(methd == WORST_FIT){
			return worstFit(objectsize);
		}
	}

	return	(NULL); //	if	not	success
}


void mem_free(void	*objectptr){
	Alloc * to_delete = (Alloc *) objectptr - allocSize;
	to_delete->prev->next = to_delete->next;
	to_delete->next->prev = to_delete->prev;
	return;
}

void mem_print(void){
	printf("print	called\n");
	int empty_space;
	int start_of_empty_space;
	Alloc* temp = (Alloc*) start;
	if(temp == NULL){
		printf("No Chunk provided!");
	}
	while(temp->next != NULL){
		empty_space = (temp->next) - (temp->length + allocSize + temp);
		start_of_empty_space = temp + temp->length + allocSize;
		if(temp->length != 0){
			printf("Start of occupied address : %d", temp + allocSize);
			printf("Size of occupied address : %d", temp->length);
		}
		if(empty_space != 0){
			printf("Start of empty address : %d", start_of_empty_space);
			printf("Size of empty address : %d", empty_space);
		}
		temp = temp->next;
	}
	if(temp->next == NULL){
		empty_space = (Alloc*)(end) - (temp->length + allocSize + temp);
		start_of_empty_space = temp + temp->length + allocSize;
		if(temp->length != 0){
			printf("Start of occupied address : %d", temp + allocSize);
			printf("Size of occupied address : %d", temp->length);
		}
		if(empty_space != 0){
			printf("Start of empty address : %d", start_of_empty_space);
			printf("Size of empty address : %d", empty_space);
		}
	}
	return;
}

void* firstFit(int osize){
	Alloc* temp = (Alloc*)start;
	Alloc* temp2;
	while(temp->next != NULL){
		temp2 = temp->next;
		Alloc * place_next = temp->next;
		Alloc * place = temp;
		if(place_next - (place + allocSize + temp->length) >= osize){
			temp->next = (Alloc*) (place + temp->length + allocSize);
			temp->next->length = osize;
			temp->next->next = temp2;
			temp2->prev =temp->next;
			temp->next->prev = temp;
			return (void*)(place + temp->length + 2 * allocSize);
		}
		temp = temp->next;
	}
	if(temp->next == NULL && osize + allocSize <= (Alloc *)end - temp->length + allocSize){
		Alloc * place = temp;
		temp->next = (Alloc*) (place + temp->length + allocSize);
		temp->next->length = osize;
		temp->next->next = NULL;
		temp->next->prev = temp;
		return (void*)(temp + temp->length + allocSize + 2 * allocSize);
	}
	return NULL;
}

void* bestFit(int osize){
	int space;
	int minValue = INT_MAX;
	Alloc* min = NULL;
	Alloc* temp = (Alloc*)start;
	while(temp->next != NULL){
		Alloc * place_next = (temp->next);
		Alloc * place = (temp);
		space = place_next - (place + allocSize + temp->length);
		if(space >= osize){
			if(space<minValue){
				minValue = space;
				min = place;
			}
		}
		temp = temp->next;
	}

	if(temp->next == NULL && osize + allocSize <= (Alloc *)end - temp->length + allocSize){
		Alloc * place = (temp);
		space = (Alloc *)end - (place + allocSize + temp->length);
		if(space >= osize){
			if(space<minValue){
				minValue = space;
				min = place;
			}
		}
	}
	if(min == NULL){
		return NULL;
	}
	Alloc * temp2 = min->next;
	Alloc * place = (min);
	min->next = (Alloc*) (place + min->length + allocSize);
	min->next->length = osize;
	min->next->next = temp2;
	min->next->prev = min;
	if (temp2 != NULL){
		temp2->prev = min->next;
	}
	return (void*)(min + min->length + allocSize + 2 * allocSize);
}

void* worstFit(int osize){
	int space;
	int maxValue = INT_MIN;
	Alloc* max = NULL;
	Alloc* temp = (Alloc*)start;
	while(temp->next != NULL){
		Alloc * place_next = (temp->next);
		Alloc * place = (temp);
		space = place_next - (place + allocSize + temp->length);
		if(space >= osize){
			if(space>maxValue){
				maxValue = space;
				max = place;
			}
		}
		temp = temp->next;
	}

	if(temp->next == NULL && osize + allocSize <= (Alloc *)end - temp->length + allocSize){
		Alloc * place = (temp);
		space = (Alloc *)end - (place + allocSize + temp->length)  ;
		if(space >= osize){
			if(space>maxValue){
				maxValue = space;
				max = place;
			}
		}
	}
	if(max == NULL){
		return NULL;
	}
	Alloc * temp2 = max->next;
	Alloc * place = max;
	max->next = (Alloc*) (place + max->length + allocSize);
	max->next->length = osize;
	max->next->next = temp2;
	max->next->prev = max;
	if (temp2 != NULL){
		temp2->prev = max->next;
	}
	return (void*)(max + max->length + allocSize + 2 * allocSize);
}