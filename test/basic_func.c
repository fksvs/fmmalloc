#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char *ptr1 = malloc(100);
	if (!ptr1) {
		fprintf(stdout, "failed to malloc() 100 bytes of memory\n");
		return 0;
	}
	fprintf(stdout, "allocated 100 bytes of memory at address %p\n", (void *)ptr1);

	char *ptr2 = calloc(10, sizeof(char));
	if (!ptr2) {
		fprintf(stdout, "failed to calloc() 10 bytes of memory\n");
		return 0;
	}
	fprintf(stdout, "allocated 10 bytes of memory at address %p\n\n", (void *)ptr2);

	fprintf(stdout, "reallocating %p with 150 bytes of memory using realloc()\n", (void *)ptr1);
	ptr1 = realloc(ptr1, 150);
	if (!ptr1) {
		fprintf(stdout, "realloc() failed for %p\n\n", (void *)ptr1);
		return 0;
	}
	fprintf(stdout, "reallocation successfull, new address: %p\n\n", (void *)ptr1);

	fprintf(stdout, "reallocating %p with 20 bytes of memory using reallocarray()\n", (void *)ptr2);
	ptr2 = reallocarray(ptr2, 20, sizeof(char));
	if (!ptr2) {
		fprintf(stdout, "reallocarray() faileod for %p\n\n", (void *)ptr2);
		return 0;
	}
	fprintf(stdout, "reallocating) successfull, new address : %p\n\n", (void *)ptr2);

	memcpy(ptr1, "hello, world!", strlen("hello, world!") + 1);
	fprintf(stdout, "%p contains: %s\n", (void *)ptr1, ptr1);

	memcpy(ptr2, "fmmalloc is awesome", strlen("fmmalloc is awesome") + 1);
	fprintf(stdout, "%p contains: %s\n\n", (void *)ptr2, ptr2);

	char *ptr3 = malloc(100);
	char *ptr4 = malloc(200);
	char *ptr5 = malloc(400);
	char *ptr6 = malloc(50);

	if (!ptr3 || !ptr4 || !ptr5 || !ptr6) {
		fprintf(stdout, "memory allocation failed\n");
		return 0;
	}
	fprintf(stdout, "allocated memories: \n100 bytes at %p\n200 bytes at %p\n400 bytes at %p\n50 bytes at %p\n",
			(void *)ptr3, (void *)ptr4, (void *)ptr5, (void *)ptr6);

	free(ptr4);
	free(ptr5);

	// reused freed memories that were merged
	char *ptr7 = malloc(500);
	if (!ptr7) {
		fprintf(stdout, "memory allocation failed\n");
		return 0;
	}
	fprintf(stdout, "allocated 500 bytes of memory at address %p\n\n", (void *)ptr7);

	free(ptr1);
	free(ptr2);
	free(ptr3);
	free(ptr6);
	free(ptr7);

	fprintf(stdout, "freed all allocated memories\n");

	return 0;
}
