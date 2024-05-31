#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALLOCS 100000

int main()
{
	int **int_arr = malloc(ALLOCS * sizeof(int *));
	if (!int_arr) {
		fprintf(stdout, "failed to allocate memory for int_arr\n");
		return 0;
	}
	fprintf(stdout, "allocated %ld bytes of memory at address %p for int_arr\n",
			ALLOCS * sizeof(int *), (void *)int_arr);

	for (int i = 0; i < ALLOCS; i++) {
		int_arr[i] = malloc(sizeof(int));

		if (!int_arr[i]) {
			fprintf(stdout, "failed to allocate memory for int_arr[%d]\n", i);
			return 0;
		}

		*int_arr[i] = i;
	}

	for (int i = 0; i < ALLOCS; i++) {
		if (*(int_arr[i]) != i) {
			fprintf(stdout, "memory check failed at int_arr[%d], expected %d but got %d\n", i, i, *(int_arr[i]));
			return 0;
		}
	}
	fprintf(stdout, "memory check passed, all values are correct\n");

	for (int i = 0; i < ALLOCS; i++) {
		free(int_arr[i]);
	}

	free(int_arr);
	fprintf(stdout, "successfully freed all allocated memory\n");

	return 0;
}
