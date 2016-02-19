#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, const char *argv[])
{
	int i, j, n;
	scanf("%d", &n);

	int **arr = malloc(n*sizeof(int *));
	for (i = 0; i < n; i++)
	{
		arr[i] = malloc(n*sizeof(int));
		for (j = 0; j < n; j++)
		{
			arr[i][j] = (i + 1)*(j + 1);
		}
	}

	int x1, y1, x2, y2, w;
	char str[20];
	sprintf(str, "%d", arr[n - 1][n - 1]);
	w = strlen(str) + 1;

	scanf("%d", &x1);
	while (x1)
	{
		scanf("%d%d%d", &y1, &x2, &y2);
		printf("\n");
		for (i = y1 - 1; i < y2; i++)
		{
			for (j = x1 - 1; j < x2; j++)
			{
				printf("%*d ", w, arr[i][j]);
			}
			printf("\n");
		}
		scanf("%d", &x1);
	}

	for (i = 0; i < n; i++)
	{
		free(arr[i]);
	}
	free(arr);

	return 0;
}