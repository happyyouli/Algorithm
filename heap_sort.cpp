#include <iostream>
using namespace std;

struct Heap {		// 堆结构定义
	int heap_size;	// 堆元素个数
	int length;		// 数字长度
	int* array;		// 数组指针
};

//====================最大堆操作====================

void swap(int& a, int& b) {
	int temp = a;
	a = b;
	b = temp;
}

int parent(int i) {	// 返回堆元素父节点下标
	return (i - 1) / 2;
}

int left(int i) {	// 返回堆元素左孩子下标
	return 2 * i + 1;
}

int right(int i) {	// 返回堆元素右孩子下标
	return 2 * i + 2;
}

void printHeap(Heap A) {	// 输出堆
	cout << "length = " << A.length << endl;
	cout << "heap_size = " << A.heap_size << endl;
	cout << "The elements in the heap : ";
	for (int i = 0; i < A.heap_size; i++)
		cout << A.array[i] << "  ";
	cout << endl;
	cout << "The elements in the array : ";
	for (int i = 0; i < A.length; i++)
		cout << A.array[i] << "  ";
	cout << endl;
	cout << endl;
}

void maxHeapify(Heap& A, int i) {	// 维护最大堆
	int l = left(i);
	int r = right(i);
	int largest = i;
	if (l < A.heap_size && A.array[l] > A.array[largest])
		largest = l;
	if (r < A.heap_size && A.array[r] > A.array[largest])
		largest = r;
	if (largest != i) {
		swap(A.array[i], A.array[largest]);
		maxHeapify(A, largest);
	}
}

Heap buildMaxHeap(int A[], int len) {	// 建立最大堆
	Heap max_heap;
	max_heap.array = A;
	max_heap.length = len;
	max_heap.heap_size = len;
	for (int i = len / 2 - 1; i >= 0; i--)
		maxHeapify(max_heap, i);
	return max_heap;
}

Heap heapSort(int A[], int len) {	// 堆排序，该操作后数组A中元素被排序为从小到大
	Heap max_heap = buildMaxHeap(A, len);
	if (len <= 1)
		return max_heap;
	for (int i = len - 1; i >= 1; i--) {
		swap(max_heap.array[i], max_heap.array[0]);
		max_heap.heap_size--;
		maxHeapify(max_heap, 0);
	}
	return max_heap;
}

//====================优先队列====================

int heapMaximum(Heap A) {		// 返回优先队列中具有最大键字的元素
	return A.array[0];
}

int heapExtractMax(Heap& A) {	//去掉并返回优先队列中具有最大键字的元素	
	if (A.heap_size < 1) {
		cout << "Heap underflow!" << endl;
		exit(1);
	}
	int max = A.array[0];
	swap(A.array[0], A.array[A.heap_size - 1]);
	A.heap_size--;
	maxHeapify(A, 0);
	return max;
}

void heapIncreaseKey(Heap& A, int i, int key) { // 将第i个元素关键字值增加到k
	if (key < A.array[i]) {
		cout << "New key is smaller than current key, the current key is not changed!" << endl;
		return;
	}
	A.array[i] = key;
	while (i > 0 && A.array[parent(i)] < A.array[i]) {
		swap(A.array[parent(i)], A.array[i]);
		i = parent(i);
	}
}

void maxHeapInsert(Heap& A, int key) {	// 在优先队列中插入一个关键字值为key的元素
	if (A.heap_size == A.length) {
		// 注：可以额外申请空间，本程序不再具体展开
		cout << "There is no place for insertion." << endl;
		return;
	}
	A.heap_size++;
	A.array[A.heap_size - 1] = 0xcfcfcfcf;
	heapIncreaseKey(A, A.heap_size - 1, key);
}



int main() {
	int A1[5] = { 1, 3, 4, 2, 5 };
	int lenA1 = sizeof(A1) / sizeof(int);
	Heap H1 = buildMaxHeap(A1, lenA1);
	cout << "建堆:" << endl;
	printHeap(H1);

	int A2[10] = { 3, 8, 56, 12, 56, 42, 78, 7, 9, 45 };
	int lenA2 = sizeof(A2) / sizeof(int);
	Heap H2 = heapSort(A2, lenA2);
	cout << "堆排序：" << endl;
	printHeap(H2);

	int max1 = heapExtractMax(H1);
	cout << "优先队列取出最大键字元素:" << endl;
	printHeap(H1);

	heapIncreaseKey(H1, 2, 6);
	cout << "将优先队列H1中第2个关键字值增加到6" << endl;
	printHeap(H1);

	maxHeapInsert(H1, 8);
	cout << "在优先队列H1中插入关键字值为8的元素" << endl;
	printHeap(H1);
	
	return 0;
}