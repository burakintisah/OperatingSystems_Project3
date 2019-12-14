#include  <stdlib.h>
#include  <stdio.h>
#include  <limits.h>
#include <pthread.h>
#include  "memalloc.h"

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

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
	size = chunksize * 1024;
	end = start + (size);
	((Alloc*)start)->length = 0;
	((Alloc*)start)->next = NULL;
	((Alloc*)start)->prev = NULL;
	return	(0); //	if	success
}

void *mem_allocate(int	objectsize){
	pthread_mutex_lock(&mutex1);
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
	pthread_mutex_unlock(&mutex1);
	return	(NULL); //	if	not	success
}

void mem_free(void	*objectptr){
	pthread_mutex_lock(&mutex1);
	if(objectptr != NULL){
		Alloc * to_delete = (Alloc *) objectptr - allocSize;
		to_delete->prev->next = to_delete->next;
		if (to_delete -> next != NULL)
			to_delete->next->prev = to_delete->prev;
	}
	pthread_mutex_unlock(&mutex1);

	return;
}

void mem_print(void){
	pthread_mutex_lock(&mutex1);
	printf("print	called\n");
	int empty_space;
	Alloc* temp = (Alloc*) start;
	if(temp == NULL){
		printf("No Chunk provided!");
	}
	while(temp->next != NULL){
		empty_space = (char*)(temp->next) - (temp->length + allocSize + (char*)temp);
		if(temp->length != 0){
			printf("Start of occupied address : 0x%lx\n", (unsigned long)((char*)temp + allocSize));
			printf("Size of occupied address : %d\n", temp->length);
		}
		if(empty_space != 0){
			printf("Start of empty address : 0x%lx\n", (unsigned long)((char*)temp + temp->length + allocSize));
			printf("Size of empty address : %d\n", empty_space);
		}
		temp = temp->next;
	}
	if(temp->next == NULL){
		empty_space = (char*)(end) - (temp->length + allocSize + (char*)temp);
		if(temp->length != 0){
			printf("Start of occupied address : 0x%lx\n", (unsigned long)((char*)temp + allocSize));
			printf("Size of occupied address : %d\n", temp->length);
		}
		if(empty_space != 0){
			printf("Start of empty address : 0x%lx\n", (char*)temp + temp->length + allocSize);
			printf("Size of empty address : %d\n", empty_space);
		}
	}
	pthread_mutex_unlock(&mutex1);
	return;
}

void* firstFit(int osize){
	Alloc* temp = (Alloc*)start;
	Alloc* temp2;
	while(temp->next != NULL){
		temp2 = temp->next;
		char * place_next = (char *)temp->next;
		char * place = (char *)temp;
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
	if(temp->next == NULL && ((osize + allocSize) <= (char *)end - ((char *)temp + temp->length + allocSize))){
		char * place = (char *)temp;
		temp->next = (Alloc*) (place + temp->length + allocSize);
		printf("%d				%d\n",(osize + allocSize), (char *)end - (temp->length + allocSize));
		temp->next->length = osize;
		temp->next->next = NULL;
		temp->next->prev = temp;
		return (void*)(temp->next + allocSize);
	}
	return NULL;
}

void* bestFit(int osize){
	int space;
	int minValue = INT_MAX;
	Alloc* min = NULL;
	Alloc* temp = (Alloc*)start;
	while(temp->next != NULL){
		char * place_next = (temp->next);
		char * place = (temp);
		space = place_next - (place + allocSize + temp->length);
		if(space >= osize){
			if(space<minValue){
				minValue = space;
				min = place;
			}
		}
		temp = temp->next;
	}

	if(temp->next == NULL && ((osize + allocSize) <= (char *)end - ((char *)temp+temp->length + allocSize))){
		char * place = (char *)temp;
		space = (char *)end - (place + allocSize + temp->length);
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
	char * place = (char *) min;
	min->next = (Alloc*) (place + min->length + allocSize);
	min->next->length = osize;
	min->next->next = temp2;
	min->next->prev = min;
	if (temp2 != NULL){
		temp2->prev = min->next;
	}
	return (void*)(min->next + allocSize);
}

void* worstFit(int osize){
	int space;
	int maxValue = INT_MIN;
	Alloc* max = NULL;
	Alloc* temp = (Alloc*)start;
	while(temp->next != NULL){
		char * place_next = (char *)temp->next;
		char * place = (char *)temp;
		space = place_next - (place + allocSize + temp->length);
		if(space >= osize){
			if(space>maxValue){
				maxValue = space;
				max = place;
			}
		}
		temp = temp->next;
	}

	if(temp->next == NULL && osize + allocSize <= (char*)end - ((char *)temp+temp->length + allocSize)){
		char * place = (char *)temp;
		space = (char *)end - (place + allocSize + temp->length)  ;
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
	char * place = (char *)max;
	max->next = (Alloc*) (place + max->length + allocSize);
	max->next->length = osize;
	max->next->next = temp2;
	max->next->prev = max;
	if (temp2 != NULL){
		temp2->prev = max->next;
	}
	return (void*)(max->next + allocSize );
}
