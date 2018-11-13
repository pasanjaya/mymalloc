#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "mymalloc.h"

#define TRUE 1
#define FALSE 0

// initialize the heap memory 
void initialize(){
  base->size = MEMORY_SIZE - BLOCK_SIZE;
  base->free = 1;
  base->next = NULL;
}

// best-fit search
t_block find_block(t_block *previous, size_t size){
  t_block current = base;
  t_block block = NULL;
  int found = TRUE;
  while(current != NULL){
    if(current->free && (current->size >= size)){
      if(found){
        block = current;
        found = FALSE;
        printf("First block found\n");
      }
      else if(!found){
        if((block->size > current->size) && (current->size >= size)){
          block = current;
          printf("Block changed\n");
        }
      }
    }
    *previous = current;
    current = current->next;
    printf("------------------------------>check next block\n");
  }
  return (block);
}

// split the block
void split_block(t_block fitting_slot, size_t size){
  t_block new;
  new = (void*)((void*)fitting_slot + size + BLOCK_SIZE);
  new->size = (fitting_slot->size) - size - BLOCK_SIZE;
  new->next = fitting_slot->next;
  new->free = 1;
  fitting_slot->size = size;
  fitting_slot->next = new;
  fitting_slot->free = 0;
}


void* MyMalloc(size_t size){
  // check whether no of bytes is greater than 0
  if((int)size <= 0){
    printf("Memory request error!\n");
    return NULL;
  }

  t_block current, previous; 

  void *result;
  size_t s;
  s = ALIGN(size);
  // s = size;

  if(!base->size){
    initialize();
    printf("Memory Initialized\n");
  }

  current = find_block(&previous, s);
  if(current != NULL){
    if(current->size == (BLOCK_SIZE + s)){
      current->free = 0;
      result = (void*)(++current);
      printf("Exact fitting block found\n");
      return result;
    }
    else if(current->size > (BLOCK_SIZE + s)){
      split_block(current, s);
      result = (void*)(++current);
      printf("block split and fitted\n");
      return result;
    }
    else {
    result = NULL;
    printf("No fitting block found\n");
    return result;
    }

  } else {
    result = NULL;
    printf("No fitting block found\n");
    return result;
  }
}

void print(){
  t_block current = base;
  while(current != NULL){
    printf("start addr %d size %d end addr %d status %d\n", current, current->size, (void*)current+current->size, current->free);
    current = current->next; 
  }
}

void merge_block(t_block prev,t_block current){
  if(prev==NULL){
    if(current->next->free==0){
      current->free=1;
    }
    else{
      current->free=1;
      current->size += BLOCK_SIZE + current->next->size;
      current->next = current->next->next;
    }
  }
  else if(current->next==NULL){
    if(prev->free==0){
      current->free=1;
    }
    else{
      prev->size += BLOCK_SIZE + current->size;
      prev->next = NULL;
    }
  }
  else{
    if(prev->free==0 && current->next->free==0){
      current->free=1;
    }
    else if(prev->free==1 && current->next->free==0){
      prev->size += BLOCK_SIZE + current->size;
      prev->next = current->next;
    }
    else if(prev->free==0 && current->next->free==1){
      current->size += BLOCK_SIZE + current->next->size;
      current->next = current->next->next;
      current->free=1;
    }
    else{
      prev->size +=2*BLOCK_SIZE + current->size +current->next->size;
      prev->next=current->next->next;
    }
  }
  
}

void MyFree(void *ptr){
  void * actualAddress= ptr;
  actualAddress -= BLOCK_SIZE;
  t_block current=base;
  t_block prev=NULL;
  int isNotFound=TRUE;

  while(current != NULL){
    if(current==actualAddress){
      isNotFound=FALSE;
      merge_block(prev,current);
      printf("Memory freed\n");
      break;
    }
    else{
      prev=current;
      current=current->next;
    }
  }

  if(isNotFound){
    printf("No such id present\n");
  }
}


int main(void) {

  char* pl = MyMalloc(-1000);
  char* p = MyMalloc(50);
  char* lp = MyMalloc(100);
  char* llp = MyMalloc(50);
  char* lllp = MyMalloc(100);
  char* llllp = MyMalloc(500);
  char* lllllp = MyMalloc(500);

  MyFree(p);
  MyFree(lp);
  MyFree(llp);
  MyFree(lllp);
  MyFree(llllp);
  MyFree(lllllp);

  //MyFree(p);
  // MyFree(lllp);
  // char* l = MyMalloc(800);
  // char* ll = MyMalloc(26);

  print();

  return 0;
}