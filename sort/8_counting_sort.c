

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>


void counting_sort( 
	int *in_arr, 
	int *out_arr, 
	int arr_len,
	int k )
{
	int *tmp_arr = ( int * )malloc( sizeof(int) * (k + 1) );
	int i = 0, j = 0;

	for ( i = 0; i < k; i++ ) {
		tmp_arr[i]  = 0;
	}

	for ( j = 0; j < arr_len; j++ ) {
		tmp_arr[in_arr[j]] = tmp_arr[in_arr[j]] + 1;
	
	}

	for ( i = 1; i < k + 1; i++ ) {
		tmp_arr[i] = tmp_arr[i] + tmp_arr[i - 1];
	}

	for ( j = arr_len - 1; j >= 0; j-- ) {
		//printf("j:%d, in_arr[j]:%d, tmp_arr[in_arr[j]]:%d\n",
		//	j, in_arr[j], tmp_arr[in_arr[j]]);
		out_arr[tmp_arr[in_arr[j]] - 1] = in_arr[j];
		tmp_arr[in_arr[j]] = tmp_arr[in_arr[j]] - 1;
	}

	free( tmp_arr );
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
	int *new_arr = ( int * )malloc( sizeof(int) * length );
	

	int i = 1000;

	for ( ; i > 0; i-- ) {
		initArr( arr, lowV, upV, length );
		//print_arr( arr, length );
		counting_sort( arr, new_arr, length, upV );
		//print_arr( new_arr, length );
		check_is_inc_arr( new_arr, length );
	}

	return 0;
}
