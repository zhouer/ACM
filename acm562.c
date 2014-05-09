#include <stdio.h>
#include <stdlib.h>

int main()
{
	int i, j, n, m, sum;
	int coins[100];
	int table[50001];

	scanf("%d", &n);
	while (n--)
	{
		sum = 0;
		for (i = 0; i < 100; i++)
			coins[i] = 0;
		table[0] = 1;
		for (i = 1; i < 50001; i++)
			table[i] = 0;

		scanf("%d", &m);
		for (i = 0; i < m; i++)
		{
			scanf("%d", &coins[i]);
			sum += coins[i];
		}

		for (i = 0; i < m; i++)
		{
			for (j = sum; j >= 0; j--)
			{
				if (table[j] != 0)
					table[j + coins[i]] = 1;
			}
		}

		for (i = sum / 2; i >= 0; i--)
		{
			if (table[i] != 0)
			{
				printf("%d\n", sum - i * 2);
				break;
			}
		}
	}

	return 0;
}
