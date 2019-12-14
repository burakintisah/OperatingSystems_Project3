#include	<stdlib.h>
#include	<stdio.h>
#include	<unistd.h>
#include 	<sys/time.h>
#include	"memalloc.h"

int	main(int	argc,	char	*argv[]){

	struct timeval startTime, endTime;
	void	*chunkptr;
	void	*endptr;
	char	*charptr;
	int	ret;
	int	i;
	int	size;
	void	*x1,	*x2,	*x3,  	*x4; //	object	pointers
	void	*x5,	*x6,	*x7,  	*x8;
	int x_length = 300;
	void * x[x_length];

	if	(argc	!=	2)	{
		printf("usage:	app	<size	in	KB>\n");
		exit(1);
	}

	size	=	atoi(argv[1]);			//	unit	is	in	KB
	//	allocate	a	chunk
	chunkptr	=	sbrk(0);	//	end	of	data	segment
	sbrk (size * 1024);	//	extend	data	segment	by	indicated	amount	(bytes)
	endptr	=	sbrk(0); //	new	end	of	data	segment

	printf("chunkstart=%lx,	chunkend=%lx,	chunksize=%lu	bytes\n",
								(unsigned	long)chunkptr,
								(unsigned	long)endptr,	(unsigned	long)(endptr	- chunkptr));
	//test	the	chunk
	printf("---starting	testing	chunk\n");
	charptr	=	(char	*)chunkptr;
	for	(i	=	0;	i	<	size;	++i)
		charptr[i]	=	0;
	printf("---chunk	test	ended	- success\n");

	ret	= mem_init(chunkptr,	size, FIRST_FIT);

	if	(ret	==	-1)	{
		printf("could	not	initialize	\n");
		exit(1);
	}


	//	below	we	allocate	and	deallocate	memory	dynamically
	gettimeofday(&startTime, NULL);
	// x1	=	mem_allocate(1000);
	// x2 = 	mem_allocate(400);
	// x3	=	mem_allocate(12300);
	// x4	=	mem_allocate(12300);
	// x5 = 	mem_allocate(1100);
	// x6 = 	mem_allocate(1100);


	for (int i=0; i<x_length; i++){
		printf("%d\n",i );
		if (i %2 == 0)
			x[i] = mem_allocate(400);
		else
		 	x[i] = mem_allocate(i*100);
	}

	// for (int i=0; i<x_length; i++){
	// 	if(i %2 != 0)
	// 		mem_free(x[i]);
	// }
	//
	// for (int i=0; i<x_length; i++){
	// 	if(i %2 != 0)
	// 		x[i] = mem_allocate(i*100);
	// }


	// mem_free(x2);
	// mem_free(x4);

	// x6 = mem_allocate(100);

	// mem_free(x1);
	// mem_free(x3);
	// mem_free(x5);
	// mem_free(x6);

	gettimeofday(&endTime, NULL);
	mem_print();

	long seconds = endTime.tv_sec - startTime.tv_sec;
	long mseconds = endTime.tv_usec - startTime.tv_usec;
	long total = (seconds * 1000000) + mseconds;
	printf("Allocation & Free took %ld microseconds\n", total);

	return	0;
}
