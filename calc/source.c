#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define EXPR_LEN 1024

typedef struct pool {
	int num;
	int pools[100]; // ??
} Pool;

void eval(Pool *pool, char ch) {
	char now = ch;
	int num = pool->num;
	if(now == '+') {
		pool->pools[num - 2] += pool->pools[num - 1];
	}
	else if(now > '-') {
		if(now == '-') {
			pool->pools[num - 2] -= pool->pools[num - 1];
		}
		else if(now == '/') {
			pool->pools[num - 2] /= pool->pools[num - 1];
		}
	}
	else if(now == '*') {
		pool->pools[num - 2] *= pool->pools[num - 1];
	}
	pool->num--;
}

int parse_expr(char *expr, Pool *pool) {
	char *expr_temp = expr;
	Pool *pool2 = pool;
	char *expr_temp2 = expr_temp;
	int now_temp = 0;
	int temp[100];
	bzero(temp, 100);
	int now = 0;
	while(true) {
		if(expr_temp[now] - '0' <= 9) {
			int size = expr_temp + now - expr_temp2;
			char *ptr = (char *)malloc(size + 1);
			memcpy(ptr, expr_temp2, size);
			ptr[size] = '\0';
			if(strcmp(ptr, "0") != 0) {
				int digit = atoi(ptr);
				if(digit > 0) {
					pool2->pools[pool2->num++] = digit;
				}
				if(expr_temp[now] != '\0') {
					if(expr_temp[now + 1] - '0' > 9) {
						printf("expression error!");
						fflush(stdout);
						return 0;
					}
				}
				expr_temp2 = expr_temp + now + 1;
				if(temp[now_temp] != '\0') {
					switch(expr_temp[now] - '%') {
						case '%': case '/': case '*':
							if(temp[now_temp] == '+' || temp[now_temp] == '-') {
								now_temp++;
								temp[now_temp] = expr_temp[now];
							}
							else {
								eval(pool2, temp[now_temp]);
								temp[now_temp] = expr_temp[now];
							}
							break;
						case '+': case '-':
							eval(pool2, temp[now_temp]);
							temp[now_temp] = expr_temp[now];
							break;
						default:
							eval(pool2, temp[now_temp]);
							now_temp--;
							break;
					}
				}
				else {
					temp[now_temp] = expr_temp[now];
				}
				if(expr_temp[now] == '\0') {
					while(now_temp >= 0) {
						eval(pool2, temp[now_temp]);
						now_temp--;
						return 1;
					}
				}
			}
			else {
				printf("prevent division by zero");
				fflush(stdout);
				return 0;
			}
		}
		now++;
	}
}

void init_pool(Pool *pool) {
	pool->num = 0;
	for(int i = 0; i <= 99; i++) {
		pool->pools[i] = 0;
	}
}

int getexpr(char *expr, int len) {
	int now = 0;
	while(true) {
		char c = getchar();
		if(now >= len || c == EOF || c == '\n') {
			expr[now] = '\0';
			return now;
		}
		if(c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || (c > '/' && c <= '9' )) {
			expr[now++] = c;
		}
	}
}

void calc() {
	char expr[EXPR_LEN];
	Pool pool; 
	while(true) {
		bzero(expr, EXPR_LEN);
		if(getexpr(expr, EXPR_LEN) == 0) {
			break;
		}
		init_pool(&pool);
		if(parse_expr(expr, &pool) == 0) {
			printf("%d\n", pool.pools[pool.num - 1]);
			fflush(stdout);
		}
	}
}	

void timeout(int signo) {
	printf("No time to waste!");
	exit(0);
}

int main() {
	ssignal(SIGALRM, timeout);
	alarm(60);
	printf("=== Welcome to SECPROG calculator ===\n");
	fflush(stdout);
	calc();
	printf("Merry Christmas!\n");
	return 0;
}
