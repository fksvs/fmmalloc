#include <unistd.h>
#include <sys/types.h>
#include "fmmalloc.h"

#define ALLOCATED 1
#define FREED 0

struct block_metadata {
	size_t block_size;
	int block_status;
	struct block_metadata *prev;
	struct block_metadata *next;
};

#define METADATA_SIZE sizeof(struct block_metadata)

void *memory_base = NULL;
void *last_block = NULL;

/* search for a specific block */
struct block_metadata *search_block(void *ptr)
{
	struct block_metadata *temp = (struct block_metadata *)memory_base;
	struct block_metadata *block = (struct block_metadata *)ptr - 1;

	while (temp != NULL) {
		if (temp == block) {
			return block;
		}

		temp = temp->next;
	}

	return NULL;
}

/* search free block */
struct block_metadata *search_free_block(size_t size)
{
	struct block_metadata *temp = (struct block_metadata *)memory_base;

	while (temp != NULL) {
		if (temp->block_status == FREED && temp->block_size >= size) {
			return temp;
		}

		temp = temp->next;
	}

	return NULL;
}

/* allocate new space */
struct block_metadata *allocate_block(size_t size)
{
	struct block_metadata *block = sbrk(METADATA_SIZE + size);
	struct block_metadata *prev = NULL;

	if (block == (void *)-1) {
		return NULL;
	}

	if (last_block) {
		prev = (struct block_metadata *)last_block;
		prev->next = block;
	}

	block->block_size = size;
	block->block_status = ALLOCATED;
	block->prev = prev;
	block->next = NULL;
	last_block = block;

	return block;
}

/* algorithm: check previous and next memory blocks, if one or two of them is
 * FREED and they are in sequential order, combine them. */
void combine_free_blocks(void *ptr)
{
	struct block_metadata *block = (struct block_metadata *)ptr - 1;
	struct block_metadata *prev_block = block->prev;
	struct block_metadata *next_block = block->next;

	if (next_block && next_block->block_status == FREED &&
		next_block == (ptr + block->block_size)) {
		if (last_block == next_block) last_block = block;

		block->block_size += next_block->block_size;
		block->next = next_block->next;
		if (next_block->next) next_block->next->prev = block;
	}
	if (prev_block && prev_block->block_status == FREED &&
		block == (prev_block + prev_block->block_size)) {
		if (last_block == block) prev_block = last_block;

		prev_block->block_size += block->block_size;
		prev_block->next = block->next;
		if (block->next) block->next->prev = prev_block;
	}
}

/* algorithm: iterate over memory block list, if ptr is found and block_status
 * is ALLOCATED set block_status to FREED, if ptr is found and block_status is
 * FREE, throw error. if ptr is not found or if ptr is NULL do nothing. after 
 * freeing memory block, check previous and next memory block. if one or two 
 * of them is free, combine them. */
void fmfree(void *ptr)
{
	if (!ptr) {
		return;
	}

	struct block_metadata *block = search_block(ptr);
	if (!block) {
		return;
	}

	if (block->block_status == FREED) {
		return; /* give error */
	}

	block->block_status = FREED;
	combine_free_blocks(ptr);
}

/* algorithm: iterate over memory block list, if block_status is FREED and block 
 * size is equel to or bigger then size, set block_status to ALLOCATED and
 * return it. if no memory block found that fits to size, allocate new block and
 * return it. if found memory blocks block_size is bigger than size, split it to
 * requested size. */
void *fmmalloc(size_t size)
{
	if (size <= 0) {
		return NULL;
	}

	struct block_metadata *block;

	/* first allocation */
	if (!memory_base) {
		block = allocate_block(size);
		if (block) {
			memory_base = block;
			return block + 1;
		}

		return NULL;
	}

	/* search for free block */
	block = search_free_block(size);
	if (block) {
		block->block_status = ALLOCATED;
		return block + 1;
	}

	/* there is no free block for requested size, allocate new memory */
	block = allocate_block(size);
	if (block) {
		return block + 1;
	}

	return NULL;
}

/* TODO */
void *fmcalloc(size_t nmemb, size_t size);
void *fmrealloc(void *ptr, size_t size);
void *fmreallocarray(void *ptr, size_t nmemb, size_t size);
