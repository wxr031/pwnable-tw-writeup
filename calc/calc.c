#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define MAX_LEN 1024

typedef struct {
	int count;
	int nums[100];
} Pool;

void timeout() {
	puts("No time to waste!");
	exit(0);
}

Pool *eval(Pool *pool, char operator) {
	if (operator == '+') {
		pool->nums[pool->count - 2] += pool->nums[pool->count - 1];
	}
	else if (operator > '+') {
		if (operator == '-') {
			pool->nums[pool->count - 2] -= pool->nums[pool->count - 1];
		}
		else if (operator == '/') {
			pool->nums[pool->count - 2] /= pool->nums[pool->count - 1];
		}
	}
	else if (operator == '*') {
		pool->nums[pool->count - 2] *= pool->nums[pool->count - 1];
	}
	--pool->count;
	return pool;
}

int parse_expr(char *s, Pool *pool) {
	char *s_base = s;
	int operator_n = 0;
	char operators[100] = {0};
	for (int i = 0; s[i] != '\0'; ++i) {
		if ((s[i] - '0') > 9) {
			int offset = (&s[i] - s_base);
			char *num_ptr = malloc(offset + 1);
			memcpy(num_ptr, s_base, offset);
			num_ptr[offset] = '\0';
			if (!strcmp(num_ptr, "0")) {
				puts("prevent division by zero");
				fflush(stdout);
				return 0;
			}
			int num = atoi(num_ptr);
			if (num > 0) {
				pool->nums[pool->count++] = num;
			}
			if (s[i] && (s[i + 1] - '0') > 9) {
				puts("expression error!");
				fflush(stdout);
				return 0;
			}
			s_base = &s[i + 1];
			if (operators[operator_n] != '\0') {
				switch (s[i]) {
					case '%':
					case '*':
					case '/':
						if (operators[operator_n] != '+' && operators[operator_n] != '-') {
							eval(pool, operators[operator_n]);
							operators[operator_n] = s[i];
						}
						else {
							operators[++operator_n] = s[i];
						}
						break;
					case '+':
					case '-':
						eval(pool, operators[operator_n]);
						operators[operator_n] = s[i];
						break;
					default:
						eval(pool, operators[operator_n--]);
						break;
				}
			}
			else {
				operators[operator_n] = s[i];
			}
		}
	}
	while (operator_n >= 0) {
		eval(pool, operators[operator_n--]);
	}
	return 1;
}

int get_expr(char *s, int max_len) {
	char c;
	int id = 0;
	while (id < max_len && read(0, &c, 1) != -1 && c != '\n') {
		if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c > '/' && c <= '9') {
			s[id++] = c;
		}
	}
	s[id] = '\0';
	return id;
}
Pool *init_pool(Pool *pool) {
	pool->count = 0;
	for (int i = 0; i <= 99; ++i) {
		result = pool;
		pool->nums[i] = 0;
	}
	return pool;
}
int calc() {
	Pool pool;
	char s[MAX_LEN];
	while (1) {
		bzero(s, MAX_LEN);
		if (!get_expr(s, MAX_LEN)) {
			break;
		}
		init_pool(&pool);
		if (parse_expr(s, &pool)) {
			printf("%d\n", pool.nums[pool.count - 1]);
			fflush(stdout);
		}
	}
	return 0;
}
int main(int argc, char **argv) {
	ssignal(14, timeout);
	alarm(60);
	puts("=== Welcome to SECPROG calculator ===");
	fflush(stdout);
	calc();
	return puts("Merry Christmas!");
}
