#include <stdio.h>
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
int randomized_partition( int *arr, int p, int r ) 
{
	
	int i = rand() % (r - p) + p;
	int temp = arr[r];
	arr[r] = arr[i];
	arr[i] = temp;

	return partition( arr, p, r );
}
int randomized_select( 
	int *arr, 
	int p,
	int r,
	int i )
{
	if ( p == r ) 
		return arr[p];
	
	int q = 0, k = 0;

	q = randomized_partition( arr, p, r );
	k = q - p + 1;
	if ( i == k ) 
		return arr[q];
	else if ( i < k ) 
		return randomized_select( arr, p, q - 1, i );
	else 
		return randomized_select( arr, q + 1, r, i - k );
}
int main()
{
	int arr[10] = {1,2,3,4,5,6,7,8,9,10};
	int len = 10;
	int i = 8;

	printf("第%d小的数是:%d\n", i, randomized_select(arr, 0, 9, i));

	return 0;
}
