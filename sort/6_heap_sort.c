#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int parent( int );
int left( int );
int right( int );

inline int parent( int index )
{
	return index / 2;
}
inline int left( int index )
{
	return index * 2;
}
inline int right( int index )
{
	return index * 2 + 1;
}

void max_heapify( 
	int *arr, 
	int heap_size, 
	int index )
{
	int l = left( index );	
	int r = right( index );
	int largest = 0;

	if ( l <= heap_size && arr[l] > arr[index] ) {
		largest = l;
	} else {
		largest = index;
	}
	if ( r <= heap_size && arr[r] > arr[largest] ) {
		largest = r;
	}
	if ( largest != index ) {
		int temp = arr[index];
		arr[index] = arr[largest];
		arr[largest] = temp;
		max_heapify( arr, heap_size, largest );
	} 
}
void build_max_heap( 
	int *arr,
	int length,
	int heap_size )
{
	int i = 0;

	for ( i = length / 2; i >= 1; i-- ) {
		max_heapify( arr, heap_size, i );
	}
}
void heap_sort(
	int *arr, 
	int length,
	int heap_size )
{
	int i = 0;

	build_max_heap( arr, length, heap_size );

	for ( i = heap_size; i >= 2; i-- ) {
		int temp = arr[1];
		arr[1] = arr[i];
		arr[i] = temp;
		heap_size -= 1;
		max_heapify( arr, heap_size, 1 );
	}
}
void check_is_inc_arr( 
	int *arr, 
	int len )
{
	int i = 0;
	
	for ( i = 1; i <= len; i++ ) {
		if ( arr[i] < arr[i - 1] ) {
			printf("check_is_inc_arr fail.\n");
			return;
		}	
	}
	printf("check_is_inc_arr ok.\n");
}
//初始化随机数组
void initArr( int *arr, int lowV,
    int upV, int len )
{
    int i = 0;
    int size = upV - lowV;

    for ( i = 1; i < len + 1; i++ )
    {
        arr[i] = rand() % size + lowV;
    }
	arr[0] = 0;
}
void print_arr( 
	int *arr, 
	int len )
{
	int i = 0;

	printf("\n=============================================\n");
	for ( i = 0; i <= len; i++ ) {
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

	int *arr = ( int * )malloc( sizeof(int) * (length + 1) );

	initArr( arr, lowV, upV, length );

	//print_arr( arr, length );
	heap_sort( arr, length, length );
	//print_arr( arr, length );

	check_is_inc_arr( arr, length );

	return 0;
}
