#include <iostream>
using namespace std;

//BST结点定义
struct TreeNode {
	int key;
	TreeNode* left;
	TreeNode* right;
	TreeNode* parent;
	TreeNode(int x) : key(x), left(NULL), right(NULL), parent(NULL) {}
};

//BST中插入一个关键值为value的结点
//注意：参数必须使用根结点指针的引用，否则当为空树时，无法改变原函数中的根结点
void treeInsert(TreeNode* &T, int value) { 
	TreeNode* z = new TreeNode(value);
	TreeNode* x = T;
	TreeNode* y = NULL;
	while (x != NULL) {
		y = x; 
		if (z->key < x->key)
			x = x->left;
		else if (z->key > x->key)
			x = x->right;
		else {  //BST中有值相同的结点时不插入
			delete z;
			return;
		}
	}
	z->parent = y;
	if (y == NULL)
		T = z;  //树为空的情况
	else if (z->key < y->key)
		y->left = z;
	else y->right = z;
}

//查找BST中关键字最小的结点，返回一个指向改结点的指针
TreeNode* treeMinimum(TreeNode* T) {
	TreeNode* x = T;
	while (x->left != NULL)
		x = x->left;
	return x;
}

//查找BST中关键字最大的结点，返回一个指向改结点的指针
TreeNode* treeMaximum(TreeNode* T) {
	TreeNode* x = T;
	while (x->right != NULL)
		x = x->right;
	return x;
}

//查找一个关键字为k的结点，返回一个指向该结点的指针
TreeNode* treeSearch(TreeNode* T, int k) {
	TreeNode* x = T;
	while (x != NULL && x->key != k)
		if (k < x->key)
			x = x->left;
		else x = x->right;
	return x;
}

//查找给定结点的后继结点，并返回指向后继结点的指针
TreeNode* treeSuccessor(TreeNode* x) {
	if (x->right != NULL)
		return treeMinimum(x->right);
	TreeNode* y = x->parent; 
	while (y != NULL && x == y->right) {
		x = y;
		y = x->parent;
	}
	return y;
}

//查找给定结点的前驱结点，并返回指向前驱结点的指针
TreeNode* treePrecursor(TreeNode* x) {
	if (x->left != NULL)
		return treeMaximum(x->left);
	TreeNode* y = x->parent;
	while (y != NULL && x == y->left) {
		x = y;
		y = x->parent;
	}
	return y;
}

//中序遍历BST
void inorderTreeWalk(TreeNode* T) {
	if (T != NULL) {
		inorderTreeWalk(T->left);
		cout << T->key << " ";
		inorderTreeWalk(T->right);
	}
}

//用子树v替换子树u并成为其双亲的孩子结点,被treeDelete调用
void transplant(TreeNode* &T, TreeNode* u, TreeNode* v) {
	if (u->parent == NULL)  //u是T的树根的情况
		T = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else u->parent->right = v;
	if (v != NULL)
		v->parent = u->parent;
}

//在BST中删除z指向的结点
//删除结点为根结点时，调用transplant函数时对T的指向做了修改，所以参数列表要用T引用
void treeDelete(TreeNode* &T, TreeNode* z) {
	if (z->left == NULL)
		transplant(T, z, z->right);
	else if (z->right == NULL)
		transplant(T, z, z->left);
	else {  //当z有左右孩子时
		TreeNode* y = treeMinimum(z->right);  //z的右子树中的最小结点为z的后继结点
		if (y->parent != z) {
			transplant(T, y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		transplant(T, z, y);
		y->left = z->left;
		y->left->parent = y;
	}
	delete z;
}

//在BST中删除z指向的结点 ———— version_2
void treeDelete_v2(TreeNode* &T, TreeNode* z) {
	if (z->left == NULL)
		transplant(T, z, z->right);
	else if (z->right == NULL)
		transplant(T, z, z->left);
	else {  //当z有左右孩子时
		TreeNode* p = treeMinimum(z->right);  //z的右子树中的最小结点为z的后继结点
		z->key = p->key;  //用z后继结点的关键字替代z的关键字，转化为删除p的问题
		transplant(T, p, p->right);  //p->left一定为NULL
		z = p;
	}
	delete z;
}

//对x所指结点做左旋操作,可用于平衡树或红黑树
void leftRotate(TreeNode*& T, TreeNode* x) {
	TreeNode* y = x->right;
	if (y == NULL) {
		cout << "failed to left rotate" << endl;
		return;  //左旋操作要求右孩子不为空，否则返回报错
	}
	x->right = y->left;
	if (y->left != NULL)
		y->left->parent = x;
	y->parent = x->parent;
	if (x->parent == NULL)
		T = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else x->parent->right = y;
	y->left = x;
	x->parent = y;
}

//对x所指结点做右旋操作,可用于平衡树或红黑树
void rightRotate(TreeNode*& T, TreeNode* x) {
	TreeNode* y = x->left;
	if (y == NULL) {
		cout << "failed to right rotate" << endl;
		return;  //右旋操作要求左孩子不为空，否则返回报错
	}
	x->left = y->right;
	if (y->right != NULL)
		y->right->parent = x;
	y->parent = x->parent;
	if (x->parent == NULL)
		T = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else x->parent->right = y;
	y->right = x;
	x->parent = y;
}

int main() {
	TreeNode* T = NULL;
	treeInsert(T, 10);
	treeInsert(T, 5);
	treeInsert(T, 15);
	treeInsert(T, 20);
	treeInsert(T, 13);
	treeInsert(T, 13);

	inorderTreeWalk(T);
	cout << endl;

	cout << treeSearch(T, 10)->key << endl;

	cout << treeMinimum(T)->key << endl;
	cout << treeMaximum(T)->key << endl;

	cout << treeSuccessor(T->left)->key << endl;
	cout << treePrecursor(T->right)->key << endl;

	treeDelete(T, T);
	inorderTreeWalk(T);
	cout << endl;

	treeDelete_v2(T, T);
	inorderTreeWalk(T);
	cout << endl;

	cout << T->key << endl;
	leftRotate(T, T);
	inorderTreeWalk(T);
	cout << endl;
	cout << T->key << endl;
	leftRotate(T, T);

	rightRotate(T, T);
	inorderTreeWalk(T);
	cout << endl;
	cout << T->key << endl;

	return 0;
}
