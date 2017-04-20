#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>


int partition( int *arr, int p, int r )
{
	int x = arr[r];
	int i = p - 1;
	int j = 0;
	int temp = 0;

	for ( j = p; j < r; j++ ) {
		if ( arr[j] <= x ) {
			i += 1;
			temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
		}
	}
	temp = arr[i + 1];
	arr[i + 1] = arr[r];
	arr[r] = temp;


	return i + 1;
}

void quick_sort( int *arr, int p, int r )
{
	int q = 0;

	if ( p < r ) {
		q = partition( arr, p, r );
		
		//printf("p->%d, q->%d, r->%d\n", p, q, r);
		
		quick_sort( arr, p, q - 1 );

		quick_sort( arr, q + 1, r );
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

	initArr( arr, lowV, upV, length );

	print_arr( arr, length );
	quick_sort( arr, 0, length - 1 );
	print_arr( arr, length );

	check_is_inc_arr( arr, length );

	return 0;
}
