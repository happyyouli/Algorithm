#include <iostream>
using namespace std;

enum RBColor{RED, BLACK};

//RBT结点定义
struct TreeNode {
	int key;
	TreeNode* left;
	TreeNode* right;
	TreeNode* parent;
	RBColor color;
	TreeNode(int x) : key(x), left(NULL), right(NULL), parent(NULL), color(BLACK) {}
};

TreeNode* nil = new TreeNode(0);

//中序遍历RBT
void RBInorderTreeWalk(TreeNode* T) {
	if (T != nil) {
		RBInorderTreeWalk(T->left);
		cout << "(" << T->key << "," << T->color << ") ";
		RBInorderTreeWalk(T->right);
	}
}

//先序遍历RBT
void RBPreorderTreeWalk(TreeNode* T) {
	if (T != nil) {
		cout << "(" << T->key << "," << T->color << ") ";
		RBPreorderTreeWalk(T->left);
		RBPreorderTreeWalk(T->right);
	}
}

//查找RBT中关键字最小的结点，返回一个指向改结点的指针
TreeNode* treeMinimum(TreeNode* T) {
	TreeNode* x = T;
	while (x->left != nil)
		x = x->left;
	return x;
}

//对x所指结点做左旋操作
void leftRotate(TreeNode* &T, TreeNode* x) {
	TreeNode* y = x->right;
	if (y == nil) {
		cout << "failed to left rotate" << endl;
		return;  //左旋操作要求右孩子不为空，否则返回报错
	}
	x->right = y->left;
	if (y->left != nil)
		y->left->parent = x;
	y->parent = x->parent;
	if (x->parent == nil)
		T = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else x->parent->right = y;
	y->left = x;
	x->parent = y;
}

//对x所指结点做右旋操作
void rightRotate(TreeNode*& T, TreeNode* x) {
	TreeNode* y = x->left;
	if (y == nil) {
		cout << "failed to right rotate" << endl;
		return;  //右旋操作要求左孩子不为空，否则返回报错
	}
	x->left = y->right;
	if (y->right != nil)
		y->right->parent = x;
	y->parent = x->parent;
	if (x->parent == nil)
		T = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else x->parent->right = y;
	y->right = x;
	x->parent = y;
}

//插入结点后的修正，被RBInsert调用
void RBInsertFixup(TreeNode* &T, TreeNode* z) {
	while (z->parent->color == RED) {  //每次插入结点为红色，父结点为红则违反第4条，否则不需要做修正
		if (z->parent == z->parent->parent->left) { //z在其祖父结点的左子树
			//只要z的父结点为红，z一定存在祖父结点
			TreeNode* y = z->parent->parent->right;  //y为z的叔结点
			if (y->color == RED) {  //case 1: z的叔结点为红
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else {
				if (z == z->parent->right) {
					//case 2: z的叔结点为黑，且z是其祖父结点的内侧孙子,转化为 case 3
					z = z->parent;
					leftRotate(T, z);
				}
				//case 3: z的叔结点为黑，且z是其祖父结点的外侧孙子
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				rightRotate(T, z->parent->parent);
			}
		}
		else {  //if的镜像情况，z在其祖父结点的右子树
			TreeNode* y = z->parent->parent->left;  //y为z的叔结点
			if (y->color == RED) {  //case 1: z的叔结点为红
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else {
				if (z == z->parent->left) {
					//case 2: z的叔结点为黑，且z是其祖父结点的内侧孙子,转化为 case 3
					z = z->parent;
					rightRotate(T, z);
				}
				//case 3: z的叔结点为黑，且z是其祖父结点的外侧孙子
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				leftRotate(T, z->parent->parent);
			}
		}
	}
	T->color = BLACK;  //修复特性2，根结点置黑
}

//在RBT中插入一个值为value的结点
void RBInsert(TreeNode*& T, int value) {
	TreeNode* z = new TreeNode(value);
	TreeNode* y = nil;
	TreeNode* x = T;
	while (x != nil) {
		y = x;
		if (value < x->key)
			x = x->left;
		else if (value > x->key)
			x = x->right;
		else {
			cout << "The element \"" << value << "\" has existed!" << endl;
			delete z;
			return;
		}
	}
	z->parent = y;
	if (y == nil)
		T = z;
	else if (value < y->key)
		y->left = z;
	else y->right = z;
	z->left = nil;
	z->right = nil;
	z->color = RED;
	RBInsertFixup(T, z);
}

//用子树v替换子树u并成为其双亲的孩子结点,被RBDelete调用
void RBTransplant(TreeNode*& T, TreeNode* u, TreeNode* v) {
	if (u->parent == nil)  //u是T的树根的情况
		T = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else u->parent->right = v;
	v->parent = u->parent;
}

//删除结点后的修正，被RBDelete调用
void RBDeleteFixup(TreeNode* &T, TreeNode* x) {
	while (x != T && x->color == BLACK) {
		if (x == x->parent->left) {  //当x为其父结点的左孩子时
			TreeNode* w = x->parent->right;  //w为x的兄弟结点
			if (w->color == RED) {  //case 1: x兄弟结点为红，转化为case2、3、4
				w->color = BLACK;
				x->parent->color = RED;
				leftRotate(T, x->parent);
				w = x->parent->right;
			}
			if (w->left->color == w->right->color) {
				//case 2: x的两侄子结点均为黑，产生新的x，继续循环
				w->color = RED;
				x = x->parent;
			}
			else {
				if (w->right->color == BLACK) {
					//case 3: x的较远侄子为黑，较近侄子为红，转化为case4
					w->left->color = BLACK;
					w->color = RED;
					rightRotate(T, w);
					w = x->parent->right;
				}
				//case 4: x的较远侄子为红，完成修复，跳至根结点修复根结点
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				leftRotate(T, x->parent);
				x = T;
			}
		}
		else {  //if的镜像情况
			TreeNode* w = x->parent->left;  //w为x的兄弟结点
			if (w->color == RED) {  //case 1: x兄弟结点为红，转化为case2、3、4
				w->color = BLACK;
				x->parent->color = RED;
				rightRotate(T, x->parent);
				w = x->parent->left;
			}
			if (w->left->color == w->right->color) {
				//case 2: x的两侄子结点均为黑，产生新的x，继续循环
				w->color = RED;
				x = x->parent;
			}
			else {
				if (w->left->color == BLACK) {
					//case 3: x的较远侄子为黑，较近侄子为红，转化为case4
					w->right->color = BLACK;
					w->color = RED;
					leftRotate(T, w);
					w = x->parent->left;
				}
				//case 4: x的较远侄子为红，完成修复，跳至根结点修复根结点
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				rightRotate(T, x->parent);
				x = T;
			}
		}
	}
	x->color = BLACK;
}

//在	RBT中删除z指向的结点
void RBDelete(TreeNode*& T, TreeNode* z) {
	TreeNode* x;  //x始终指向被删除或被移走结点的原始位置
	TreeNode* y = z;  //y指向要被删除或移走的结点
	RBColor y_original_color = y->color;  //表示要被删掉或移走结点的颜色
	if (z->left == nil) {
		x = z->right;
		//若调用前x指向nil,则调用下面的函数后x的parent指向z的父结点
		RBTransplant(T, z, z->right);
	}
	else if (z->right == nil) {
		x = z->left;
		RBTransplant(T, z, z->left);
	}
	else {
		y = treeMinimum(z->right);
		y_original_color = y->color;
		x = y->right;
		if (y->parent == z)
			x->parent = y;
		else {
			RBTransplant(T, y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		RBTransplant(T, z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
		delete z;
	}
	if (y_original_color == BLACK)
		RBDeleteFixup(T, x);
}

//在	RBT中删除z指向的结点————版本2
void RBDelete_v2(TreeNode*& T, TreeNode* z) {
	TreeNode* x;  //x始终指向被删除或被移走结点的原始位置
	TreeNode* y = z;  //y指向要被删除或移走的结点
	RBColor y_original_color = y->color;  //表示要被删掉或移走结点的颜色
	if (z->left == nil) {
		x = z->right;
		//若调用前x指向nil,则调用下面的函数后x的parent指向z的父结点
		RBTransplant(T, z, z->right);
	}
	else if (z->right == nil) {
		x = z->left;
		RBTransplant(T, z, z->left);
	}
	else {  //转化为删除z后继结点y的问题
		y = treeMinimum(z->right);
		y_original_color = y->color;
		z->key = y->key;
		x = y->right;
		RBTransplant(T, y, y->right);
		delete y;
	}
	if (y_original_color == BLACK)
		RBDeleteFixup(T, x);
}

//输出当前红黑树状态
void RBPrint(TreeNode* T) {
	cout << endl;
	cout << "当前红黑树为：" << endl;
	cout << "中序遍历： ";
	RBInorderTreeWalk(T);
	cout << endl;
	cout << "先序遍历： ";
	RBPreorderTreeWalk(T);
	cout << endl;
	cout << endl;
}

int main() {

	TreeNode* T = nil;
	RBInsert(T, 10);
	RBInsert(T, 5);
	RBInsert(T, 15);
	RBInsert(T, 20);
	RBInsert(T, 13);

	RBPrint(T);

	RBDelete(T, T);
	RBPrint(T);

	RBDelete(T, T->left);
	RBPrint(T);

	return 0;
}
