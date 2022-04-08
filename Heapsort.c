#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <conio.h>
#include <malloc.h>
#define _CRT_SECURE_NO_WARNINGS 
#pragma warning(disable : 4996)

typedef struct ListMatrx {
	int size;
	int det;
	int** matrx;
}ListMatrx;

ListMatrx InitList(int size) {
	ListMatrx tmp;
	tmp.size = size;
	tmp.det = 0;
	tmp.matrx = (int**)malloc(sizeof(int*) * tmp.size);
	if (tmp.matrx == NULL) {
		printf("Ошибка памяти!");
		exit(0);
	}
	for (int i = 0; i < tmp.size; i++) {
		tmp.matrx[i] = (int*)malloc(sizeof(int) * tmp.size);
		if (tmp.matrx[i] == NULL) {
			printf("Ошибка памяти!");
			exit(0);
		}
	}
	return tmp;
}

void PrintMatrx(int** Matrx, int size, FILE* f)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			fprintf(f, "%d ", Matrx[i][j]);
			printf("%d ", Matrx[i][j]);
		}
		fprintf(f, "\n");
		printf("\n");
	}
}

void HeapSort(int* a, int kolmatrx) {
	int step = 0;
	int bool = 0;
	int temp;
	while (1){
		for (int i = 0; i < kolmatrx; ++i){
			bool = 0;
			if (2 * i + 2 + step < kolmatrx){
				if (a[i + step] > a[2 * i + 1 + step] || a[i + step] > a[2 * i + 2 + step]){
					if (a[2 * i + 1 + step] < a[2 * i + 2 + step]){
						temp = a[i + step];
						a[i + step] = a[2 * i + 1 + step];
						a[2 * i + 1 + step] = temp;
						bool = 1;
					}
					else if (a[2 * i + 2 + step] < a[2 * i + 1 + step])	{
						temp = a[i + step];
						a[i + step] = a[2 * i + 1 + step];
						a[2 * i + 1 + step] = temp;
						bool = 1;
					}
				}
			}
			else if (2 * i + 1 + step < kolmatrx){
				if (a[i + step] > a[2 * i + 1 + step]){
					temp = a[i + step];
					a[i + step] = a[2 * i + 1 + step];
					a[2 * i + 1 + step] = temp;
					bool = 1;
				}
			}
		}
		if (!bool) step++;
		if (step + 2 >= kolmatrx) break;
	}
}

int** ReadMatrx(FILE* f, int k) {
	int kar = 0;
	int** matrx = (int**)malloc(sizeof(int*) * k);
	if (matrx == NULL) {
		printf("Ошибка памяти!");
		exit(0);
	}
	for (int i = 0; i < k; i++) {
		matrx[i] = (int*)malloc(sizeof(int) * k);
		if (matrx[i] == NULL) {
			printf("Ошибка памяти!");
			exit(0);
		}
	}
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < k; j++) {
			fscanf(f, "%d", &kar);
			matrx[i][j] = kar;
			kar = 0;
		}
	}
	return matrx;
}

int** Delitij(int** matrx, int k, int nstr, int nstl, int** newmath) {
	int str = 0;
	int stl = 0;
	for (int i = 0; i < k - 1; i++) {
		if (i == nstr) {
			str = 1;
		}
		stl = 0;
		for (int j = 0; j < k - 1; j++) {
			if (j == nstl) {
				stl = 1;
			}
			newmath[i][j] = matrx[i + str][j + stl];
		}
	}
	return newmath;
}

int CountDet(int** matrx, int k) {
	int det = 0;
	int deg = 1;//степень
	if (k == 1) {
		det = matrx[0][0];
		return det;
	}
	if (k == 2) {
		det = matrx[0][0] * matrx[1][1] - matrx[1][0] * matrx[0][1];
		return det;
	}
	else {
		int** newmath = (int**)malloc(sizeof(int*) * (k - 1));
		if (newmath == NULL) {
			printf("Ошибка памяти!");
			exit(0);
		}
		for (int q = 0; q < (k - 1); q++) {
			newmath[q] = (int*)malloc(sizeof(int) * (k - 1));
			if (newmath[q] == NULL) {
				printf("Ошибка памяти!");
				exit(0);
			}
		}
		for (int j = 0; j < k; j++) {
			newmath = Delitij(matrx, k, 0, j, newmath);
			det = det + (deg * matrx[0][j] * CountDet(newmath, k - 1));
			deg = -deg;
		}
	}
	return det;
}

void main() {
	ListMatrx* ArrMat;
	FILE* f;
	f = fopen("input.txt", "r");
	if (f == NULL) {
		printf("Ошибка памяти!");
		exit(0);
	}
	int kolmatrx = 0;
	fscanf(f, "%d", &kolmatrx);

	ArrMat = (ListMatrx*)malloc(sizeof(ListMatrx) * kolmatrx);
	if (ArrMat == NULL) {
		printf("Ошибка памяти!");
		exit(0);
	}
	int k;

	for (int i = 0; i < kolmatrx; ++i) {
		k = 0;
		fscanf(f, "%d", &k);
		ArrMat[i] = InitList(k);
		ArrMat[i].size = k;
		ArrMat[i].matrx = ReadMatrx(f, k);
		ArrMat[i].det = CountDet(ArrMat[i].matrx, ArrMat[i].size);
	}

	fclose(f);

	int* a = (int*)malloc(sizeof(int) * kolmatrx);
	if (a == NULL) {
		printf("Ошибка памяти!");
		exit(0);
	}
	for (int n = 0; n < kolmatrx; n++) {
		a[n] = ArrMat[n].det;
	}
	HeapSort(a, kolmatrx);
	f = fopen("output.txt", "w");
	for (int i = 0; i < kolmatrx; i++) {
		printf("%d", ArrMat[i].det);
		int n = 0;
		while (a[i] != ArrMat[n].det) {
			++n;
		}
		PrintMatrx(ArrMat[n].matrx, ArrMat[n].size, f);
		ArrMat[n].det = -10000;
	}
	fclose(f);
}
