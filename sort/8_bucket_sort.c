#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

//#define DEBUG

typedef struct bucket_data {
	int no;
	struct bucket_data *next;
} bucket_data;

void sort_insert( bucket_data *, int );

void bucket_sort( int *arr, int len )
{
	bucket_data *tmp_arr = ( bucket_data * )malloc( sizeof(bucket_data) * 10 );
	int i = 0, count = 0;
	
	//初始化链表头数组
	for ( i = 0; i < 10; i++ ) {
		tmp_arr[i].next = NULL;
	}

	//遍历数据并有序插入到相应链表中
	for ( i = 0; i < len; i++ ) {
		sort_insert( &tmp_arr[arr[i] / 10], arr[i] );
	}

	//从链表中取数据到数组
	count = 0;
	for ( i = 0; i < 10; i++ ) {
		bucket_data *p = tmp_arr[i].next;
		bucket_data *free_p = NULL;
		#ifdef DEBUG
			printf("bucket[%d]:\n", i);
		#endif
		while( p != NULL ) {
		#ifdef DEBUG
			printf("%d ", p->no);
		#endif
			free_p = p;
			arr[count] = p->no;
			count++;
			p = p->next;
			free( free_p );
		}
		#ifdef DEBUG
			printf("\n");
		#endif
	}

	free( tmp_arr );
}
void sort_insert( bucket_data *head, int no )
{
	bucket_data *p = head;
	bucket_data *data = ( bucket_data * )malloc( sizeof(bucket_data) );
	data->no = no;
	data->next = NULL;

	while ( p != NULL && p->next != NULL ) {
		if ( p->next->no >= no ) {
			data->next = p->next;
			p->next = data;
			break;
		}
		p = p->next;
	}

	if ( p->next == NULL ) {
		p->next = data;
	}
}
void check_is_inc_arr( 
	int *arr, 
	int len )
{
	int i = 0;
	
	for ( i = 0; i < len; i++ ) {
		if ( arr[i] < arr[i - 1] ) {
			printf("check_is_inc_arr fail.\n");
			return;
		}	
	}
}
//初始化随机数组
void initArr( int *arr, int lowV,
    int upV, int len )
{
    int i = 0;
    int size = upV - lowV;

    for ( i = 0; i < len; i++ )
    {
        arr[i] = rand() % size + lowV;
    }
}
void print_arr( 
	int *arr, 
	int len )
{
	int i = 0;

	printf("\n=============================================\n");
	for ( i = 0; i < len; i++ ) {
		printf("%d ", arr[i]);
	}
	printf("\n=============================================\n");
}
int main(
	int argc,
	char **argv )
{
	if ( argc != 4 ) {
		printf("input array length and the random value range.\n");
		exit( 0 );
	}

	srand( (int)time(NULL) );

	int length = atoi( argv[1] );
	int lowV = atoi( argv[2] );
	int upV = atoi( argv[3] );

	int *arr = ( int * )malloc( sizeof(int) * (length) );

	int i = 1000;

	for ( ; i > 0; i-- ) {
		initArr( arr, lowV, upV, length );
		//print_arr( arr, length );
		bucket_sort( arr, length );
		//print_arr( arr, length );
		check_is_inc_arr( arr, length );
	}

	return 0;
}
