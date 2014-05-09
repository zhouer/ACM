#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Token {
	int type;
	char data[65536];
} Token;

typedef struct Number {
	int sign;
	int *digits;
	int length;
} Number;

enum { TOKEN_NUM, TOKEN_ADD, TOKEN_SUB, TOKEN_MUL, TOKEN_POW, TOKEN_NULL };

char line_buf[65536];
int line_buf_index;

Token token;

void show_number(Number n) {
	int i;
	if (n.sign == -1) {
		if (n.length > 1 || n.digits[0] != 0) {
			printf("-");
		}
	}
	for (i = n.length - 1; i >= 0; i--) {
		printf("%d", n.digits[i]);
	}
	printf("\n");
}

void get_token() {
	int state;
	char c;
	char data[65536];
	int data_index = 0;
	token.type = TOKEN_NULL;

	state = 0;
	while (1) {
		c = line_buf[line_buf_index];
		if (state == 0) {
			if (c >= '0' && c <= '9') {
				data[data_index++] = c;
				line_buf_index++;
				state = 1;
			} else if (c == '*') {
				line_buf_index++;
				state = 2;
			} else if (c == '+') {
				token.type = TOKEN_ADD;
				line_buf_index++;
				break;
			} else if (c == '-') {
				token.type = TOKEN_SUB;
				line_buf_index++;
				break;
			} else {
				break;
			}
		} else if (state == 1) {
			if (c >= '0' && c <= '9') {
				data[data_index++] = c;
				line_buf_index++;
			} else {
				token.type = TOKEN_NUM;
				strncpy(token.data, data, data_index);
				token.data[data_index] = 0;
				break;
			}
		} else if (state == 2) {
			if (c == '*') {
				token.type = TOKEN_POW;
				line_buf_index++;
				break;
			} else {
				token.type = TOKEN_MUL;
				break;
			}
		} else {
			break;
		}
	}
}

Number get_number(Token t) {
	int i;
	Number result;
	result.digits = (int *)malloc(sizeof(int) * 65536);
	
	for (i = 0; t.data[i]; i++);
	result.length = i;

	result.sign = 1;
	for (i = 0; i < result.length; i++) {
		result.digits[i] = t.data[result.length - 1 - i] - '0';
	}

	return result;
}

int get_int(Number n) {
	int result = 0;
	int i;

	for (i = n.length - 1; i >= 0; i--) {
		result = result * 10 + n.digits[i];
	}

	return result;
}

Number big_mul(Number x, Number y)
{
	int i, j;
	Number result;
	result.digits = (int *)malloc(sizeof(int) * 65536);

	for (i = 0; i < 65536; i++) {
		result.digits[i] = 0;
	}

	for (i = 0; i < x.length; i++) {
		for (j = 0; j < y.length; j++) {
			result.digits[i + j] += x.digits[i] * y.digits[j];
		}
	}

	for (i = 0; i < 65536; i++) {
		if (result.digits[i] >= 10) {
			result.digits[i + 1] += result.digits[i] / 10;
			result.digits[i] %= 10;
		}
	}

	for (i = 65536 - 1; i > 0 && result.digits[i] == 0; i--);
	result.length = i + 1;

	result.sign = x.sign * y.sign;

	return result;
}

Number big_pow(Number base, int exp)
{
	Number result;
	result.digits = (int *)malloc(sizeof(int) * 65536);

	if (base.length == 1 && base.digits[0] == 0) {
		result.sign = 1;
		result.length = 1;
		if (exp == 0) {
			result.digits[0] = 1;
		} else {
			result.digits[0] = 0;
		}
		return result;
	} else if (exp == 0) {
		result.sign = 1;
		result.digits[0] = 1;
		result.length = 1;
		return result;
	} else if (exp == 1) {
		return base;
	}

	result = big_pow(base, exp / 2);
	result = big_mul(result, result);

	if (exp % 2 == 1) {
		result = big_mul(result, base);
	}

	return result;
}

int big_gt(Number x, Number y)
{
	int i;

	if (x.sign > y.sign) {
		return 1;
	} else if (y.sign > x.sign) {
		return 0;
	}

	if (x.length > y.length) {
		if (x.sign == 1) {
			return 1;
		} else {
			return 0;
		}
	} else if (x.length < y.length) {
		if (x.sign == 1) {
			return 0;
		} else {
			return 1;
		}
	}

	for (i = x.length; i >= 0; i--) {
		if (x.digits[i] > y.digits[i]) {
			if (x.sign == 1) {
				return 1;
			} else {
				return 0;
			}
		} else if (x.digits[i] < y.digits[i]) {
			if (x.sign == 1) {
				return 0;
			} else {
				return 1;
			}
		}
	}

	return 0;
}

Number big_add(Number x, Number y)
{
	int i;
	int xsign, ysign;
	Number result, tmp;
	result.digits = (int *)malloc(sizeof(int) * 65536);
	tmp.digits = (int *)malloc(sizeof(int) * 65536);

	if (x.sign == y.sign) {
		for (i = 0; i < 65536; i++) {
			result.digits[i] = 0;
		}

		for (i = 0; i < x.length || i < y.length; i++) {
			result.digits[i] += x.digits[i] + y.digits[i];
			if (result.digits[i] >= 10) {
				result.digits[i + 1]++;
				result.digits[i] -= 10;
			}
		}

		for (i = 65536 - 1; i > 0 && result.digits[i] == 0; i--);
		result.length = i + 1;

		result.sign = x.sign;
	} else {
		xsign = x.sign;
		ysign = y.sign;
		x.sign = 1;
		y.sign = 1;
		if (big_gt(x, y)) {
			result.sign = xsign;
		} else {
			result.sign = ysign;
			tmp = x;
			x = y;
			y = tmp;
		}

		for (i = 0; i < 65536; i++) {
			result.digits[i] = x.digits[i] - y.digits[i];
		}

		for (i = 0; i <65536; i++) {
			if (result.digits[i] < 0) {
				result.digits[i] += 10;
				result.digits[i + 1]--;
			}
		}

		for (i = 65536 - 1; i > 0 && result.digits[i] == 0; i--);
		result.length = i + 1;
	}

	return result;
}

int accept(int type) {
	if (type == token.type) {
		return 1;
	}
	return 0;
}

Number pre1() {
	Number result;

	if (accept(TOKEN_NUM)) {
		result = get_number(token);
		get_token();
	}

	return result;
}

Number pre2() {
	Number result = pre1();
	Number n;

	if (accept(TOKEN_POW)) {
		get_token();
		n = pre2();
		result = big_pow(result, get_int(n));
	}

	return result;
}

Number pre3() {
	Number result = pre2();
	Number n;

	while (accept(TOKEN_MUL)) {
		get_token();
		n = pre2();
		result = big_mul(result, n);
	}

	return result;
}

Number pre4() {
	Number result = pre3();
	Number n;
	int type;

	while (accept(TOKEN_ADD) || accept(TOKEN_SUB)) {
		type = token.type;
		get_token();
		n = pre3();

		if (type == TOKEN_SUB) {
			n.sign *= -1;
		}

		result = big_add(result, n);
	}

	return result;
}

int main()
{
	while(fgets(line_buf, 65536, stdin)) {
		line_buf_index = 0;
		get_token();
		show_number(pre4());
	}

	return 0;
}
