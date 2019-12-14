#ifndef	MEMALLOC_H
#define	MEMALLOC_H

int	mem_init(void* ,int,	int);
void* mem_allocate(int);
void mem_free(void	*);
void mem_print	(void);
void* firstFit(int);
void* bestFit(int);
void* worstFit(int);

#define	FIRST_FIT	0
#define	BEST_FIT	1
#define	WORST_FIT	2
#endif
