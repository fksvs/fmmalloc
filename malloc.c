#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <assert.h>

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

void merge_free_blocks(void *ptr)
{
	struct block_metadata *block = (struct block_metadata *)ptr - 1;
	struct block_metadata *prev_block = block->prev;
	struct block_metadata *next_block = block->next;

	if (next_block) {
		if (next_block->block_status == FREED) {
			if (last_block == next_block) last_block = block;

			block->block_size += next_block->block_size;
			block->next = next_block->next;
			if (next_block->next) next_block->next->prev = block;
	       }
	}
	if (prev_block) {
		if (prev_block->block_status == FREED) {
			if (last_block == block) prev_block = last_block;

			prev_block->block_size += block->block_size;
			prev_block->next = block->next;
			if (block->next) block->next->prev = prev_block;
		}
	}
}

void free(void *ptr)
{
	if (!ptr) {
		return;
	}

	struct block_metadata *block = search_block(ptr);
	if (!block) {
		return;
	}

	assert(block->block_status == ALLOCATED);

	block->block_status = FREED;
	merge_free_blocks(ptr);
}

void *malloc(size_t size)
{
	if (size <= 0) {
		return NULL;
	}

	struct block_metadata *block;

	if (!memory_base) {
		block = allocate_block(size);
		if (block) {
			memory_base = block;
			return block + 1;
		}

		return NULL;
	}

	block = search_free_block(size);
	if (block) {
		block->block_status = ALLOCATED;
		return block + 1;
	}

	block = allocate_block(size);
	if (block) {
		return block + 1;
	}

	return NULL;
}

void *calloc(size_t nmemb, size_t size)
{
	if (nmemb != 0 && size > SIZE_MAX / nmemb) {
		return NULL;
	}

	void *block = malloc(nmemb * size);
	if (!block) {
		return NULL;
	}

	unsigned char *ptr = (unsigned char *)block;
	unsigned char zero = (unsigned char)0;

	for (size_t i = 0; i < nmemb * size; i++) {
		*(ptr + i) = zero;
	}

	return block;
}

void *realloc(void *ptr, size_t size)
{
	void *block = NULL;

	if (ptr) {
		free(ptr);
		block = malloc(size);
	} else if (!ptr) {
		block = malloc(size);
	}

	return block;
}

void *reallocarray(void *ptr, size_t nmemb, size_t size)
{
	if (nmemb != 0 && size > SIZE_MAX / nmemb) {
		return NULL;
	}

	return realloc(ptr, nmemb * size);
}
