#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

void insert_sort0( int *arr, int index )
{
        int i, tmp = arr[index];
        //printf("index:%d\n", index);
        for ( i = index - 1; i >= 0; i-- ) {
                if ( arr[i] > tmp ) {
                        arr[i + 1] = arr[i];
                } else {
                        arr[i + 1] = tmp;
                        return;
                }
        }

        arr[i + 1] = tmp;
}

void insert_sort( int *arr, int index )
{
        //printf("index:%d\n", index);
        if ( index > 0 ) {
                insert_sort( arr, index - 1 );
                //printf("index:%d\n", index);
                insert_sort0( arr, index );
        }
}
void sortLoop0( int *arr, int len )
{
        //sleep( 20 );
        insert_sort( arr, len - 1 );
}
//排序前后做时间对比，精度sec
void sortLoop( int *arr, int len )
{
        time_t tt0 = time( NULL );
        printf("\tbefore sort:%s", ctime(&tt0));
        sortLoop0( arr, len );
        time_t tt1 = time( NULL );
        printf("\tafter sort:%s", ctime(&tt1));
        printf("\tsort the array cost %d(sec),%d(min)\n",
                (int)(tt1 - tt0), (int)((tt1 - tt0) / 60));
}
//初始化随机数组
void initArr( int *arr, int lowV,
        int upV, int len )
{
        srand( (int)time(NULL) );
        int i = 0;
        int size = upV - lowV;

        for ( ; i < len; i++ )
        {
                arr[i] = rand() % size + lowV;
        }
}
//打印数组
void printArr( int *arr, int len )
{
        int i = 0;
        for ( ; i < len; i++ )
                printf("%d ", arr[i]);
        printf("\n");
}
//这里写了个简单得数组检查，检查是否
//是正确的递增序列(怕自己的代码没检查
//边界条件偶尔有错误的排序数组)
int check_arr_increase( int *arr, int len )
{
        int i = 0;

        for ( ; i < len - 1; i++ )
        {
                if ( arr[i] <= arr[i + 1] )
                        continue;
                else
                {
                        printf("array isn't increase!!!\n");
                        return -1;
                }

        }
        printf("array is increase!!!!\n");
        return 0;
}
int main( int argc, char **argv )
{
        if ( argc != 4 )
        {
                printf("usage: ./execfile lowV upV len\n");
                return 0;
        }

        int lowV = atoi( argv[1] );
        int upV = atoi( argv[2] );
        unsigned int len = atoi( argv[3] );

        int *arr = NULL;
        arr = ( int *) malloc( len * sizeof(int) );

        initArr( arr, lowV, upV, len );
        //printArr( arr, len );
        sortLoop( arr, len );
        check_arr_increase( arr, len );
        //printArr( arr, len );

        free( arr );
        arr = NULL;

        return 0;
}
