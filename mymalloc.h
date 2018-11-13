#include <stdio.h>
#include <stddef.h>

// #define align4(x) (((((x)-1)>>2)<<2)+4) // align playload with metablock size
#define ALIGNMENT 8 // min size of a block
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

#define MEMORY_SIZE 25000

#define BLOCK_SIZE ALIGN(sizeof(struct s_block))


typedef struct s_block *t_block;

char mallocArray[MEMORY_SIZE] = {'\0'}; // make null array;


struct s_block{
  size_t size;
  t_block next;
  int free;
};

struct s_block *base=(void*)mallocArray;


/*

size of the mata-data block is 24 bytes
min size of a data-block 8 bytes

*/