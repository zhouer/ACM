#include <stdio.h>

void print_array(char arr[8], int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		printf("%c", arr[i]);
		if (i != len - 1)
		{
			printf(",");
		}
	}
}

void work(char sorted[8], int len1, char unsorted[8], int len2)
{
	if (len2 == 0)
	{
		int i;
		for (i = 0; i < len1; i++)
		{
			printf("  ");
		}
		printf("writeln(");
		print_array(sorted, len1);
		printf(")\n");
	}
	else
	{
		char ins = unsorted[len2 - 1];
		char buf[8];
		int i;
		for (i = 0; i <= len1; i++)
		{
			int j;
			if (len1 > 0)
			{
				for (j = 0; j < len1; j++)
				{
					printf("  ");
				}

				if (i == 0)
				{
					printf("if %c < %c then\n", ins, sorted[i]);
				}
				else if (i == len1)
				{
					printf("else\n");
				}
				else
				{
					printf("else if %c < %c then\n", ins, sorted[i]);
				}
			}

			for (j = 0; j <= len1; j++)
			{
				if (j < i)
				{
					buf[j] = sorted[j];
				}
				else if (j == i)
				{
					buf[j] = ins;
				}
				else
				{
					buf[j] = sorted[j - 1];
				}
			}

			work(buf, len1 + 1, unsorted, len2 - 1);
		}
	}
}

int main()
{
	char sorted[8];
	char unsorted[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
	int m, len;

	scanf("%d", &m);
	while (m--)
	{
		scanf("%d", &len);

		printf("program sort(input,output);\n");
		printf("var\n");
		print_array(unsorted, len);
		printf(" : integer;\n");
		printf("begin\n");
		printf("  readln(");
		print_array(unsorted, len);
		printf(");\n");

		work(sorted, 0, unsorted, len);

		printf("end.\n");
		if (m != 0)
		{
			printf("\n");
		}
	}

	return 0;
}
