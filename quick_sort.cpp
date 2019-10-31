#include <iostream>
using namespace std;

void exchange(int& a, int& b) {
	int temp = a;
	a = b;
	b = temp;
}

int partition(int A[], int p, int r) {
	int pivot = A[r];
	int i = p - 1;
	for (int j = p; j <= r - 1; j++)
		if (A[j] <= pivot) {
			i++;
			exchange(A[i], A[j]);
		}
	exchange(A[i+1], A[r]);
	return i + 1;
}

void quickSort(int A[], int p, int r) {
	if (p < r) {
		int q = partition(A, p, r);
		quickSort(A, p, q - 1);
		quickSort(A, q + 1, r);
	}
}


//划分函数的另一种方法
int partition_v2(int A[], int p, int r) {
	int pivot = A[r];
	int i = p, j = r;
	while (i < j) {
		while (i < j && A[i] <= pivot)
			i++;
		A[j] = A[i];
		while (i < j && A[j] >= pivot)
			j--;
		A[i] = A[j];
	}
	A[i] = pivot;
	return i;
}

void quickSort_v2(int A[], int p, int r) {
	if (p < r) {
		int q = partition_v2(A, p, r);
		quickSort(A, p, q - 1);
		quickSort(A, q + 1, r);
	}
}


int main() {
	int A[10] = { 1, 4, 5, 8, 3, 8, 7, 6, 0, 2 };
	quickSort(A, 0, 9);
	for (int i = 0; i < 10; i++)
		cout << A[i] << " ";
	cout << endl;

	int B[10] = { 1, 4, 5, 8, 3, 8, 7, 6, 0, 2 };
	quickSort_v2(B, 0, 9);
	for (int i = 0; i < 10; i++)
		cout << B[i] << " ";
	cout << endl;

	return 0;
}
