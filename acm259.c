#include <stdio.h>

char vertex_type[256];
char vertex_name[256];
char vertex_from[256];
char vertex_to[256];
int vertex_count;
int edge[256][256];
int app_count;

char queue[10000];
int head, tail;

int bfs(int source, int sink)
{
	int flow;
	int i, j, found;
	char v;

	queue[0] = source;
	head = 0;
	tail = 1;

	while(head != tail)
	{
		v = queue[head++];

		if (v == sink)
			break;

		for (i = 0; i < vertex_count; i++)
		{
			found = 0;
			j = v;
			while(j != -1)
			{
				if (j == i) {
					found = 1;
					break;
				}
				j = vertex_from[j];
			}
			if (found)
				continue;

			if (edge[v][i] > 0) {
				vertex_from[i] = v;
				queue[tail++] = i;
			}
		}
	}

	if (v != sink)
		return 0;

	flow = 1000;
	for (i = 1; i != 0; i = vertex_from[i])
	{
		if (edge[vertex_from[i]][i] < flow)
			flow = edge[vertex_from[i]][i];
	}

	for (i = 1; i != 0; i = vertex_from[i])
	{
		vertex_to[vertex_from[i]] = i;
		edge[vertex_from[i]][i] -= flow;
		edge[i][vertex_from[i]] += flow;
	}

	return flow;
}

void maxflow()
{
	int total = 0;
	int i, j, c, f;

	while((f = bfs(0, 1)) != 0)
	{
		total += f;
	}

	if (total < app_count) {
		printf("!\n");
		return;
	}

	for (c = '0'; c <= '9'; c++)
	{
		for (i = 0; i < vertex_count; i++)
		{
			if (vertex_type[i] != 4 || vertex_name[i] != c)
				continue;

			for (j = 0; j < vertex_count; j++)
			{
				if (vertex_type[j] == 3 && vertex_to[j] == i) {
					printf("%c", vertex_name[j]);
					break;
				}
			}
			if (j == vertex_count)
				printf("_");

			break;
		}

		if (i == vertex_count)
			printf("_");
	}
	printf("\n");

}

int main()
{
	char *s;
	char buf[16];
	int v_begin, v_end;
	int i, j, k;

	vertex_type[0] = 1; /* source */
	vertex_name[0] = 's';
	vertex_from[0] = -1;
	vertex_type[1] = 2; /* sink */
	vertex_name[1] = 't';
	vertex_count = 2;
	app_count = 0;

	while(1)
	{
		s = fgets(buf, 16, stdin);

		if (buf[0] == '\n' || s == NULL)
		{
			/*
			for (i = 0; i < vertex_count; i++)
			{
				for (j = 0; j < vertex_count; j++)
					printf("%c%c %d, ", vertex_name[i], vertex_name[j], edge[i][j]);
				printf("\n");
			}
			*/

			maxflow();

			vertex_count = 2;
			app_count = 0;

			for (i = 0; i < 256; i++)
				for (j = 0; j < 256; j++)
					edge[i][j] = 0;

			if (s == NULL)
				break;

			continue;
		}

		v_begin = vertex_count;
		for (i = 0; i < buf[1] - '0'; i++)
		{
			vertex_type[vertex_count] = 3; /* application */
			vertex_name[vertex_count] = buf[0];
			edge[0][vertex_count] = 1;
			vertex_count++;
			app_count++;
		}
		v_end = vertex_count;

		for (i = 3; buf[i] != ';'; i++)
		{
			for (j = 0; j < vertex_count; j++)
			{
				if (vertex_type[j] == 4 && vertex_name[j] == buf[i]) {
					edge[j][1] = 1;
					break;
				}
			}

			if (j == vertex_count) {
				vertex_type[vertex_count] = 4; /* computer */
				vertex_name[vertex_count] = buf[i];
				edge[vertex_count][1] = 1;
				vertex_count++;
			}

			for (k = v_begin; k < v_end; k++)
			{
				edge[k][j] = 1;
			}
		}
	}

	return 0;
}
