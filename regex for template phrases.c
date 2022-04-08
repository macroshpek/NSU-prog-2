#include<stdio.h>
#include<stdlib.h>
#include <conio.h>
#include <malloc.h>
#define _CRT_SECURE_NO_WARNINGS 
#pragma warning(disable : 4996)

void sread(FILE* input, char* shablon) {
	int j = 0;
	while (1) {
		fscanf(input, "%c", &shablon[j]);
		printf("%c", shablon[j]);
		if (shablon[j] == '\n') {
			shablon[j] = '\0';
			break;
		}
		shablon = (char*)realloc(shablon, ++j);
		if (shablon == NULL) {
			printf("Error: memory weren`t alloc");
			exit(0);
		}
	}
}

void rstr(char* str, int j) {
	str = (char*)realloc(str, ++j);
	if (str == NULL) {
		printf("Error: memory weren`t alloc");
		exit(0);
	}
}

void gotcha(FILE* input, char* str, int j) {
	printf("/");
	while (1) {
		fscanf(input, "%c", &str[j]);
		printf("%c", str[j]);
		if (str[j] == '\n') {
			str = NULL;
		}
		rstr(str, j);
	}
}

int skobki(FILE* input, char* str, int j, char* shablon, int k) {
	int count = (int)shablon[k + 1] - 48;
	printf("[%d - count]", count);
	for (int i = 0; i < count; i++) {
		switch (shablon[k]) {
		case'\\':
			if (shablon[k + 1] == 'd' && '0' <= str[j] <= '9') {
				k = k + 2;
				rstr(str, j);
				j++;
			}
			else {
				if (shablon[k + 1] == 'D' && ('a' <= str[j] <= 'z' || 'A' <= str[j] <= 'Z')) {
					k = k + 2;
					rstr(str, j);
					j++;
				}
				else {
					return 0;
				}
			}
			break;
		case '~':
			if (shablon[k + 1] != str[j]) {
				k = k + 2;
				rstr(str, j);
				j++;
			}
			else {
				return 0;
			}
			break;
		case ')':
			return 1;
		default:
			if (str[j] == shablon[k]) {
				k++;
				rstr(str, j);
				j++;
			}
			else {
				return 0;
			}
		}
	}
	return 1;
}

void cmp(FILE* input, char* shablon) {
	FILE* out = fopen("output.txt", "w");
	if (out == NULL) {
		printf("Error: file weren`t create");
		exit(0);
	}
	char* str;
	int j = 0, k = 0, count, countstr = 0;
	char musor;
	fscanf(input, "%d", &count);
	fscanf(input, "%c", &musor);
	printf("%d\n", count);
	for (int i = 0; i < count; i++) {
		str = (char*)malloc(1 * sizeof(char));
		if (str == NULL) {
			printf("Error: memory weren`t alloc");
			exit(0);
		}
		while (1) {
			fscanf(input, "%c", &str[j]);
			if (str[j] == '\n') {
				countstr++;
				fprintf(out, "%d ", i);
				k = j = 0;
				break;
			}
			else {
				switch (shablon[k]) {
				case '\\':
					if (shablon[k + 1] == 'd' && '0' <= str[j] <= '9') {
						k = k + 2;
						printf("%c", str[j]);
						rstr(str, j);
						j++;
					}
					else {
						if (shablon[k + 1] == 'D' && ('a' <= str[j] <= 'z' || 'A' <= str[j] <= 'Z')) {
							k = k + 2;
							printf("%c", str[j]);
							rstr(str, j);
							j++;
						}
						else {
							gotcha(input, str, j);
						}
					}
					break;
				case '~':
					if (shablon[k + 1] != str[j]) {
						k = k + 2;
						printf("%c", str[j]);
						rstr(str, j);
						j++;
					}
					else {
						gotcha(input, str, j);
					}
					break;
				case '[':
					printf("ya rabotayu");
					if (!(skobki(input, str, j, shablon, k))) {
						gotcha(input, str, j);
					}
					break;
				default:
					if (str[j] == shablon[k]) {
						k++;
						printf("%c", str[j]);
						rstr(str, j);
						j++;
					}
					else {
						gotcha(input, str, j);
					}
					break;
				}
			}
		}
		k = j = 0;
	}
}

void main() {
	FILE* input = fopen("input.txt", "r");
	if (input == NULL) {
		printf("Error: file wasn`t open");
		exit(0);
	}
	char* shablon = (char*)malloc(1 * sizeof(char));
	if (shablon == NULL) {
		printf("Error: memory aren`t reallocated");
		exit(0);
	}
	sread(input, shablon);
	cmp(input, shablon);
}