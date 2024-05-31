#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char *ptr1 = malloc(0);
	if (!ptr1) {
		fprintf(stdout, "failed successfully to allocate 0 bytes of memory\n");
	} else {
		fprintf(stdout, "allocated 0 bytes of memory at address %p\n", (void *)ptr1);
	}

	char *ptr2 = calloc(10000000000000, 100000000000);
	if (!ptr2) {
		fprintf(stdout, "integer overflow detected, allocation failed\n");
	} else {
		fprintf(stdout, "allocated a large block of memory at address %p\n", (void *)ptr2);
	}

	char *ptr3 = malloc(10);
	if (!ptr3) {
		fprintf(stdout, "failed to allocate 10 bytes of memory\n");
		return 0;
	}
	fprintf(stdout, "allocated 10 bytes of memory at address %p\n", (void *)ptr3);

	fprintf(stdout, "freeing memory at address %p\n", (void *)ptr3);
	free(ptr3);
	fprintf(stdout, "freed memory, trying to free it again\n");
	free(ptr3);

	return 0;
}
