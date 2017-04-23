#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void merge( int *arr, int p,
    int q, int r )
{
    int arrlen = r - p + 1;
    int *tmparr = (int *)malloc( sizeof(int) * arrlen );
    int i, j, k, tmp;

    for ( i = 0; i < arrlen; i++ )
        tmparr[i] = arr[i + p];

    j = 0;
    k = q - p + 1;

    for ( i = p; i < r + 1; i++ )
    {
        if ( tmparr[j] > tmparr[k] )
        {
            arr[i] = tmparr[k];
            k++;
        }
        else
        {
            arr[i] = tmparr[j];
            j++;
        }
        if ( j == q - p + 1 || k == r - p + 1 )
        {
            i++;
            break;
        }

    }
    for ( ; i < r + 1; i++ )
    {
        if ( j < q - p + 1 )
        {
            arr[i] = tmparr[j];
            j++;
            continue;
        }
        if ( k < r - p + 1 )
        {
            arr[i] = tmparr[k];
            k++;
            continue;
        }
    }

    free( tmparr );
    tmparr = NULL;
}
void mergeSort( int *arr, int p, int r )
{
    int q;
    if ( p < r )
    {
        q = ( p + r ) / 2;
        mergeSort( arr, p, q );
        mergeSort( arr, q + 1, r );
        merge( arr, p, q, r );
    }
}
void sortLoop0( int *arr, int len )
{
    mergeSort( arr, 0, len - 1 );
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
