#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_CHILD_NUM 52
#define UPPER_LOW 65
#define UPPER_UP 90
#define LOWER_LOW 97
#define LOWER_UP 122

#define PRINT(format, arg...) \
do { \
	printf("[%s/%d]:", __func__, __LINE__); \
	printf(format, ##arg); \
	printf("\n"); \
}while(0) 

typedef struct trieTreeNode {
	char letter;
	int count;
	int is_word;
	struct trieTreeNode *next[MAX_CHILD_NUM];
} trieTreeNode;

typedef struct trieTree {
	trieTreeNode *root;
} trieTree;

typedef struct count_data {
	int order;
	int count;
	char string[20];
	struct count_data *next;
} count_data;

int trans_letter_2_index(
	char letter )
{
	int index = -1;
	if ( letter >= LOWER_LOW 
		&& letter <= LOWER_UP ) {
		index = letter - LOWER_LOW + 26;
	} else if ( letter >= UPPER_LOW 
		&& letter <= UPPER_UP ) {
		index = letter - UPPER_LOW;	
	} else {
		PRINT("error letter input:%c", letter);
		exit( 0 );
	}

	return index;


}
trieTreeNode *create_node(
	char letter )
{
	trieTreeNode *node = 
		( trieTreeNode * )calloc( 1, sizeof(trieTreeNode) );
	node->letter = letter;
	node->count = 0;
	node->is_word = 0;
}
void insert(
	trieTreeNode *root,
	char *word )
{
	if ( root == NULL ) {
		PRINT("root node is null.");
		return;
	}
	int i = 0;
	trieTreeNode *cur = root;

	for ( i; word[i] != '\0'; i++ ) {
		int next_index = trans_letter_2_index(word[i]);
		//PRINT("letter:%c, index:%d", word[i], next_index);
		if ( cur->next[next_index] == NULL ) {
			cur->next[next_index] = create_node( word[i] );
		} else {
			//cur->next[next_index]->count += 1;
		}
		if ( word[i+1] == '\0' ) {
			cur->next[next_index]->count += 1;
			cur->next[next_index]->is_word = 1;
		}
		cur = cur->next[next_index];
	}
}
// 删除链表所有结点
void delete_list_all_node(
	count_data *node )
{
	count_data *p = NULL;
	while ( node ) {
		p = node;
		node = node->next;
		free( p );
	}
}
void print_list_all_node(
	count_data *node )
{
	printf("\n");
	node = node->next;
	while ( node ) {
		printf("[%d],count:%d\tword:%s\n", 
			node->order, node->count, node->string);
		node = node->next;
	}
	printf("\n");
}
void update_insert_node(
	count_data *insert_node )
{
	if ( !insert_node->next )
		return;
	count_data *print_p = insert_node; 

	if ( insert_node->order == 1 ) {
		delete_list_all_node( insert_node->next );
		insert_node->next = NULL;
	} else if ( insert_node->order < 1 ) {
		PRINT("ERROR!!!!!");
		exit( 0 );
	} else {
		count_data *p = insert_node;
		insert_node = insert_node->next;
		while ( insert_node ) {
			if ( insert_node->count < p->count ) {
				insert_node->order = p->order - 1;
			} else if ( insert_node->count > p->count ) {
				PRINT("ERROR!!!cur->count:%d, pre->count:%d", 
					insert_node->count, p->count);
				exit( 0 );
			} else {
				insert_node->order = p->order;
			}
			if ( insert_node->order < 1 ) {
				delete_list_all_node( insert_node );
				p->next = NULL;
				break;
			}
			p = insert_node;
			insert_node = insert_node->next;
		}
	}
}
void list_insert( 
	char *tmp_word,
	int cur_count,
	int tail,
	count_data *head,
	int top_num )
{
	tmp_word[tail] = '\0';
	count_data *new_data = ( count_data * )malloc( sizeof(count_data) );
	new_data->count = cur_count;
	memcpy( new_data->string, tmp_word, tail + 1 );
	new_data->next = NULL;

	//PRINT("count:%d\ttmp_word:%s, string:%s", cur_count, tmp_word, new_data->string);

	if ( head->next == NULL ) {
		head->next = new_data;
		new_data->order = top_num;
	} else if ( cur_count > head->next->count ) {
		new_data->order = head->next->order;
		new_data->next = head->next;
		head->next = new_data;
		update_insert_node( new_data );
	} else {
		while ( 1 ) {
			head = head->next;
			if ( head->next == NULL ) {
				// top_num 要大于1，即出现次数最多的前num(num>1)个单词	
				// ，没做判断
				if ( head->order > 1 ) {
					head->next = new_data;
					if ( head->count == new_data->count ) 
						new_data->order = head->order;
					else
						new_data->order = head->order - 1;

					head->next = new_data;
				} else if ( head->count > new_data->count ) {
					// 不插入
					free( new_data );
				} else if ( head->count == new_data->count ) {
					head->next = new_data;
					new_data->order = head->order;
				} else if ( head->count < new_data->count ) {
					// 此种情况只有求出现次数最多的前1个单词时有
					head->count = new_data->count;
					free( new_data );
				}
				break;
			} else if ( head->count >= cur_count 
				&& head->next->count < cur_count ) {
				new_data->next = head->next;
				head->next = new_data;
				new_data->order = head->order;
				update_insert_node( new_data );
				break;
			}
		}
	}
}
void find_top_count1(
	trieTreeNode *root,
	char *tmp_word,
	int tail,
	count_data *head, 
	int top_num )
{
	if ( !root )
		return;

	tmp_word[tail] = root->letter;
	tail++;

	if ( root->is_word ) {

		/*
		printf("\n--------------before delete------------------\n");
		print_list_all_node( head );
		printf("\n--------------------------------------------\n");
		*/

		list_insert( tmp_word, root->count, tail, head, top_num );
		
		/*
		printf("\n--------------------after delete----------------------------\n");
		print_list_all_node( head );
		printf("\n-----------------------------------------------------------\n");
		*/
	}

	int i = 0;
	for ( i; i < MAX_CHILD_NUM; i++ ) {
		find_top_count1( root->next[i], tmp_word, tail, head, top_num );
	}
}
void find_top_count(
	trieTreeNode *root,
	int top_num )
{
	if ( !root )
		return;
	
	int i = 0;

	count_data *head = ( count_data * )malloc( sizeof(count_data) );

	for ( i; i < MAX_CHILD_NUM; i++ ) {
		char tmp_word[20] = {0};
		find_top_count1( root->next[i], tmp_word, 0, head, top_num );
	}

	printf("出现次数最大前%d次的单词:\n", top_num);
	count_data *p = head->next;
	count_data *free_p = NULL;
	while ( p != NULL ) {
		free_p = p;
		printf("前%d,count:%d\t%s\n", p->order, p->count, p->string);
		p = p->next;
		free( free_p );
	}
	free( head );
}

void tree_walk1(
	trieTreeNode *root,
	char *tmp_word,
	int tail )
{
	if ( !root )
		return;

	tmp_word[tail] = root->letter;
	tail++;
	//printf("%c\n", root->letter);
	if ( root->is_word ) {
		int j = 0;
		printf("count:%d\t", root->count);
		for ( j; j < tail; j++ ) {
		 	printf("%c", tmp_word[j]);
		}
		printf("\n");
	}

	int i = 0;
	for ( i; i < MAX_CHILD_NUM; i++ ) {
		tree_walk1( root->next[i], tmp_word, tail );
	}
}
void tree_walk(
	trieTreeNode *root )
{
	if ( !root ) 
		return;

	int i = 0;

	for ( i; i < MAX_CHILD_NUM; i++ ) {
		char tmp_word[20] = {0};
		tree_walk1( root->next[i], tmp_word, 0 );
	}
}
int main(
	int argc,
	char **argv )
{
	if ( argc != 3 ) {
		PRINT("USAGE: please input words file & top number");
		exit( 0 );
	}

	char *file_name = argv[1];
	int top_num = atoi( argv[2] );

	trieTree *tree = ( trieTree * )malloc( sizeof(trieTree) );

	tree->root = create_node( -1 );

	int fd = open(file_name, O_RDONLY);
	if ( fd < 0 ) {
		PRINT("OPEN FILE %s ERROR!!!(%s)", file_name, (char *)strerror(errno));
		exit( 0 );
	}
	// 每次读取文件的缓冲区
	char buf[1024 * 10] = {0};

	// 每次读取的大小
	int read_len = 1024;

	// 读取的返回值
	int read_bytes = 0;

	// 从读取的缓冲区每次提取'\n' - '\n'之间的单词
	char tmp_word[20] = {0};

	// 读取文件缓冲区如果出现单词隔断，剩余部分在下一次
	// read才能读到，这个index做单词继续拼接
	int tmp_index = 0;

	while ( 1 ) {
		memset( buf, 0, read_len );
		read_bytes = read( fd, buf, read_len );
		if ( read_bytes <= 0 )
			break;
		//printf("readbytes:%d------\n%s\n", read_bytes, buf);
		int cur = 0;
		while ( cur < read_bytes ) {
			// 单词文件最后一个单词末尾一定要有'\n'
			if ( buf[cur] == '\n' ) {
				tmp_word[tmp_index] = '\0';
				//printf("insert word:%s\n", tmp_word);
				insert( tree->root, tmp_word );
				memset( tmp_word, 0, 20 );
				tmp_index = 0;
			} else {
				tmp_word[tmp_index] = buf[cur];
				tmp_index++;
			}
			cur++;
		}
	}
	printf("\n========================================\n");
	tree_walk( tree->root );
	
	find_top_count( tree->root, top_num );

	close( fd );

	return 0;
}
