#include <stdio.h>
#include "fmmalloc.h"

int main()
{
	int *data = fmmalloc(10 * sizeof(int));

	for (int i = 0; i < 10; i++) {
		data[i] = i;
	}

	for (int i = 0; i < 10; i++) {
		printf("%d\n", data[i]);
	}

	fmfree(data);

	return 0;
}
