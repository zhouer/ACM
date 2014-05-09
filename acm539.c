#include <stdio.h>

#define MAX 25

int node[MAX];
int edge[MAX][MAX];
int used[MAX][MAX];

int walk(int n)
{
	int i;
	int result, max = 0;

	for (i = 0; i < MAX; i++)
	{
		if (edge[n][i] != 0 && used[n][i] == 0)
		{
			used[n][i] = used[i][n] = 1;
			result = walk(i) + 1;
			if (result > max)
				max = result;
			used[n][i] = used[i][n] = 0;
		}
	}

	return max;
}

int main()
{
	int i, j, m, n;
	int from, to, result, max;

	while (1)
	{
		scanf("%d %d", &n, &m);
		if (m == 0 && n == 0)
			break;

		max = 0;
		for (i = 0; i < MAX; i++)
		{
			node[i] = 0;
			for (j = 0; j < MAX; j++)
			{
				edge[i][j] = 0;
				used[i][j] = 0;
			}
		}

		for (i = 0; i < m; i++)
		{
			scanf("%d %d", &from, &to);
			node[from] = node[to] = 1;
			edge[from][to] = edge[to][from] = 1;
		}

		for (i = 0; i < MAX; i++)
		{
			if (node[i] != 0)
			{
				result = walk(i);
				if (result > max)
					max = result;
			}
		}

		printf("%d\n", max);
	}

	return 0;
}
