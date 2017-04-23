
        find_max_crossing_subarray( arr, low, mid, high );
        // printf("%d %d %d/%d %d %d/%d %d %d\n",
        //     lLow, lHigh, lSum,
        //     rLow, rHigh, rSum,
        //     crossLow, crossHigh, crossSum);
        if ( lSum >= rSum && lSum >= crossSum ) {
            finalLeftIndex = lLow;
            finalRightIndex = lHigh;
            finalSubArraySum = lSum;
        }
        else if ( rSum >= lSum && rSum >= crossSum ) {
            finalLeftIndex = rLow;
            finalRightIndex = rHigh;
            finalSubArraySum = rSum;
        }
        else {
            finalLeftIndex = crossLow;
            finalRightIndex = crossHigh;
            finalSubArraySum = crossSum;
        }
        // printf("------%d %d %d\n", finalLeftIndex, finalRightIndex, finalSubArraySum);
    }
}
//暴力破解法求最长子数组，不过这里用来测试我写的分治求法结果是否正确
int check_result( int *arr, int len, int lIndex, int rIndex, int Sum )
{
    int maxSum = -100000000;
    int lIndex1;
    int rIndex1;
    int i = 0, j = 0;

    for ( i = 0; i < len; i++ ) {
        int tmpSum = 0;
        for ( j = i; j < len; j++ ) {
            tmpSum += arr[j];
            if ( tmpSum > maxSum ) {
                maxSum = tmpSum;
                lIndex1 = i;
                rIndex1 = j;
            }
        }
    }
    if ( Sum == maxSum ) {
        if ( lIndex == lIndex1 && rIndex == rIndex1 ) {
            return 0;
        }
        printf("equal!!\n");
        printf("exhaustivly_find_result:lIndex->%d,rIndex->%d,Sum->%d\n",
            lIndex1, rIndex1, maxSum);
        printf("divide_and_conquer_result:lIndex->%d,rIndex->%d,Sum->%d\n",
            lIndex, rIndex, Sum);
        return 1;
    }
    // else {
    //     printf("not equal!!!!!!!!!");
    // }
    return -1;
}
void mainLoop( int *arr, int len )
{
    find_maximum_subarray( arr, 0, len - 1 );
    printf("----------------------------------------------------\n");
    printf("lIndex:%d,rIndex:%d,subArrSum:%d\n",
        finalLeftIndex, finalRightIndex, finalSubArraySum);
    printf("----------------------------------------------------\n");
}
void stepLoop( int *arr, int len )
{
    time_t tt0 = time( NULL );
    printf("before:%s", ctime(&tt0));
    mainLoop( arr, len );
    time_t tt1 = time( NULL );
    printf("after:%s", ctime(&tt1));
    printf("cost %d(sec),%d(min)\n",
        (int)(tt1 - tt0), (int)((tt1 - tt0) / 60));
}
//初始化随机数组
void initArr( int *arr, int lowV,
    int upV, int len )
{
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
int main( int argc, char **argv )
{
    srand( (int)time(NULL) );
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

    // int len = 10;
    // int arr[10] = {7, 0, -86, 61, -72, 50, -38, -25, -70, -76};

    int i = 0;

    //随机10000个数组求最大子数组，然后检测结果是否正确
    for ( i = 0; i < 10000; i++ ) {
        initArr( arr, lowV, upV, len );
        printArr( arr, len );
        // stepLoop( arr, len );
        usleep(1000);
        find_maximum_subarray( arr, 0, len - 1 );
        int ret = check_result( arr, len,
                    finalLeftIndex, finalRightIndex, finalSubArraySum );
        if ( ret < 0 ) {
            printf("failed!!\n");
            return 0;
        }
        printf("-------------equal:%d\n", i);
    }

    free( arr );
    arr = NULL;

    return 0;
}
