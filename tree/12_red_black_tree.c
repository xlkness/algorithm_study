#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

//#define COLOR_RED   1;
//#define COLOR_BLACK 2;



typedef enum color {
	COLOR_RED,
	COLOR_BLACK
}color;

typedef struct rbTreeNode {
	int key;
	color color;
	struct rbTreeNode *parent;
	struct rbTreeNode *left;
	struct rbTreeNode *right;
}rbTreeNode;

typedef struct rbTree {
	int node_num;
	int height;
	rbTreeNode *root;
	rbTreeNode *null;
}rbTree;

void print_tree( rbTree * );

rbTreeNode *tree_minimum(
	rbTreeNode *root,
	rbTreeNode *null )
{
	while ( root->left != null ) {
		root = root->left;
	}

	return root;
}

rbTreeNode *tree_maximum(
	rbTreeNode *root,
	rbTreeNode *null )
{
	while ( root->right != null ) {
		root = root->right;
	}

	return root;
}


void left_rotate(
	rbTree *tree,
	rbTreeNode *node )
{
	rbTreeNode *p = node->right;
	node->right = p->left;

	if ( p->left != tree->null )
		p->left->parent = node;

	p->parent = node->parent;

	if ( node->parent == tree->null )
		tree->root = p;
	else if ( node == node->parent->left )
		node->parent->left = p;
	else
		node->parent->right = p;

	p->left = node;
	node->parent = p;
}
void right_rotate(
	rbTree *tree,
	rbTreeNode *node )
{
	rbTreeNode *p = node->left;
	node->left = p->right;

	if ( p->right != tree->null )
		p->right->parent = node;

	p->parent = node->parent;

	if ( node->parent == tree->null )
		tree->root = p;
	else if ( node == node->parent->right )
		node->parent->right = p;
	else
		node->parent->left = p;

	p->right = node;
	node->parent = p;
}
void rb_insert_fixup(
	rbTree *tree,
	rbTreeNode *node )
{
	rbTreeNode *p = tree->null;

	while ( node->parent->color == COLOR_RED ) {
		if ( node->parent == node->parent->parent->left ) {

			p = node->parent->parent->right;
			if ( p->color == COLOR_RED ) {
				node->parent->color = COLOR_BLACK;
				p->color = COLOR_BLACK;
				node->parent->parent->color = COLOR_RED;
				node = node->parent->parent;
			} else if ( node == node->parent->right ) {
				node = node->parent;
				left_rotate( tree, node );
			} else {
				node->parent->color = COLOR_BLACK;
				node->parent->parent->color = COLOR_RED;
				right_rotate( tree, node->parent->parent );
			}

		} else {

		 	p = node->parent->parent->left;
			if ( p->color == COLOR_RED ) {
				node->parent->color = COLOR_BLACK;
				p->color = COLOR_BLACK;
				node->parent->parent->color = COLOR_RED;
				node = node->parent->parent;
			} else if ( node == node->parent->left ) {
				node = node->parent;
				right_rotate( tree, node );
			} else {
				node->parent->color = COLOR_BLACK;
				node->parent->parent->color = COLOR_RED;
				left_rotate( tree, node->parent->parent );
			}

		}
	}

	tree->root->color = COLOR_BLACK;
}
void rb_insert(
	rbTree *tree,
	rbTreeNode *node )
{
	rbTreeNode *p = tree->null;
	rbTreeNode *q = tree->root;

	while ( q != tree->null ) {
		p = q;
		if ( node->key < q->key )
			q = q->left;
		else
			q = q->right;
	}

	node->parent = p;

	if ( p == tree->null )
		tree->root = node;
	else if ( node->key < p->key )
		p->left = node;
	else
		p->right = node;

	node->left = tree->null;
	node->right = tree->null;
	node->color = COLOR_RED;
	rb_insert_fixup( tree, node );
}
void rb_transplant(
	rbTree *tree,
	rbTreeNode *node_a,
	rbTreeNode *node_b )
{
	if ( node_a->parent == tree->null )
		tree->root = node_b;
	else if ( node_a == node_a->parent->left )
		node_a->parent->left = node_b;
	else
		node_a->parent->right = node_b;

	node_b->parent = node_a->parent;
	free( node_a );
}
void rb_delete_fixup(
	rbTree *tree,
	rbTreeNode *node )
{
	rbTreeNode *p = tree->null;

	while ( node != tree->null && node->color == COLOR_BLACK ) {
		if ( node == node->parent->left ) {
			p = node->parent->right;
			if ( p->color == COLOR_RED ) {
				p->color = COLOR_BLACK;
				node->parent->color = COLOR_RED;
				left_rotate( tree, node->parent );
				p = node->parent->right;
			} else if ( p->left->color == COLOR_BLACK
				&& p->right->color == COLOR_BLACK ) {
				p->color = COLOR_RED;
				node = node->parent;
			} else if ( p->right->color == COLOR_BLACK ) {
				p->left->color = COLOR_BLACK;
				p->color = COLOR_RED;
				right_rotate( tree, p );
				p = node->parent->right;
			} else {
				p->color = node->parent->color;
				node->parent->color = COLOR_BLACK;
				left_rotate( tree, node->parent );
				node = tree->root;
			}
		} else {
			p = node->parent->left;
			if ( p->color == COLOR_RED ) {
				p->color = COLOR_BLACK;
				node->parent->color = COLOR_RED;
				right_rotate( tree, node->parent );
				p = node->parent->left;
			} else if ( p->left->color == COLOR_BLACK
				&& p->right->color == COLOR_BLACK ) {
				p->color = COLOR_RED;
				node = node->parent;
			} else if ( p->left->color == COLOR_BLACK ) {
				p->right->color = COLOR_BLACK;
				p->color = COLOR_RED;
				left_rotate( tree, p );
				p = node->parent->left;
			} else {
				p->color = node->parent->color;
				node->parent->color = COLOR_BLACK;
				right_rotate( tree, node->parent );
				node = tree->root;
			}
		}
	}
	node->color = COLOR_BLACK;
}
void rb_delete(
	rbTree *tree,
	rbTreeNode *node )
{
	rbTreeNode *p = node;
	rbTreeNode *q = tree->null;
	color origin_color = p->color;

	if ( node->left == tree->null ) {
		q = node->right;
		rb_transplant( tree, node, node->right );
	} else if ( node->right == tree->null ) {
		q = node->left;
		rb_transplant( tree, node, node->left );
	} else {
		p = tree_minimum( node->right, tree->null );
		origin_color = p->color;
		q = p->right;

		if ( p->parent == node )
			q->parent = p;
		else {
			rb_transplant( tree, p, p->right );
			p->right = node->right;
			p->right->parent = p;
		}

		rb_transplant( tree, node, p );
		p->left = node->left;
		p->left->parent = p;
		p->color = node->color;
	}

	if ( origin_color == COLOR_BLACK )
		rb_delete_fixup( tree, q );
}
int random_num()
{
	int a = 1;
	int b = 100;

	return rand() % ( b - a ) + a;
}
void postorder_tree_free(
	rbTreeNode *root,
	rbTreeNode *null )
{
	if ( root != null ) {
		postorder_tree_free( root->left, null );
		postorder_tree_free( root->right, null );
		free( root );
	}
}
void inorder_tree_walk(
	rbTreeNode *root,
	rbTreeNode *null )
{
	if ( root != null ) {
		inorder_tree_walk( root->left, null );
		printf("%d(%d) ", root->key, root->color);
		inorder_tree_walk( root->right, null );
	}
}
void preorder_tree_walk(
	rbTreeNode *root,
	rbTreeNode *null )
{
	if ( root != null ) {
		printf("%d(%d) ", root->key, root->color);
		preorder_tree_walk( root->left, null );
		preorder_tree_walk( root->right, null );
	}
}
void postorder_tree_walk(
	rbTreeNode *root,
	rbTreeNode *null )
{
	if ( root != null ) {
		postorder_tree_walk( root->left, null );
		postorder_tree_walk( root->right, null );
		printf("%d(%d) ", root->key, root->color);
	}
}
void print_tree(
	rbTree *tree )
{
	printf("中序:");
	inorder_tree_walk( tree->root, tree->null );
	printf("\n");
	printf("前序:");
	preorder_tree_walk( tree->root, tree->null );
	printf("\n");
	printf("后序:");
	postorder_tree_walk( tree->root, tree->null );
	printf("\n\n");
}
rbTreeNode *tree_search_recursion(
	rbTreeNode *root,
	int key,
	rbTreeNode *null)
{
	if ( root == null || key == root->key ) {
		return root;
	}

	if ( key < root->key )
		return tree_search_recursion( root->left, key, null );
	else
		return tree_search_recursion( root->right, key, null );
}

rbTreeNode *tree_search(
	rbTreeNode *root,
	int key,
	rbTreeNode *null )
{
	while ( root != null && key != root->key ) {
		if ( key < root->key )
			root = root->left;
		else
			root = root->right;
	}

	return root;
}
// 找前驱 即中序遍历的前一个位置值
rbTreeNode *tree_predecessor(
	rbTreeNode *node,
	rbTreeNode *null )
{
	if ( node->left != null )
		return tree_maximum( node->left, null );

	rbTreeNode *p = node->parent;

	while ( p != null && node == p->left ) {
		node = p;
		p = p->parent;
	}

	return p;
}
// 找后继 即中序遍历的后一个位置值
rbTreeNode *tree_successor(
	rbTreeNode *node,
	rbTreeNode *null )
{
	if ( node->right != null )
		return tree_minimum( node->right, null );

	rbTreeNode *p = node->parent;

	while ( p != null && node == p->right ) {
		node = p;
		p = p->parent;
	}

	return p;
}
void free_tree(
	rbTree *tree )
{
	rbTreeNode *root = tree->root;

	postorder_tree_free( root, tree->null );

	free( tree );
}
int main(
	int argc,
	char **argv )
{
	srand((int)time(NULL));

	int i = 10;
	int len = 10;
	int arr[10] = {10,4,15,14,5, 2,8,13,1,19};

	rbTree *tree = ( rbTree * )malloc( sizeof(rbTree) );
	tree->node_num = 0;
	tree->height = 0;
	tree->null = ( rbTreeNode * )malloc( sizeof(rbTreeNode) );
	tree->null->color = COLOR_BLACK;
	tree->root = tree->null;

	for ( i = 0; i < len; i++ ) {
		rbTreeNode *node =
			( rbTreeNode * )malloc( sizeof(rbTreeNode) );
		//node->key = random_num();
		node->key = arr[i];
		node->parent = tree->null;
		node->left = tree->null;
		node->right = tree->null;
		rb_insert( tree, node );
		printf("insert %d successfully\n", arr[i]);
	}

	print_tree( tree );

	rbTreeNode *delete_node = tree_search( tree->root, 4, tree->null );

	if ( delete_node != tree->null ) {
		rb_delete( tree, delete_node );
	}
	printf("\n删除%d的节点后遍历顺序为:\n", 4);
	print_tree( tree );
	printf("\n");

	delete_node = tree_search( tree->root, 15, tree->null );
	if ( delete_node != tree->null ) {
		rb_delete( tree, delete_node );
	}
	printf("\n删除%d的节点后中序遍历顺序为:\n", 15);
	print_tree( tree );
	printf("\n");

	free_tree( tree );

	return 0;
}
