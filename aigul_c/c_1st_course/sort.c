#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

int moves = 0, comparisons = 0;

static int power_of_minus_one(int n){
    if(n % 2) return -1;
    return 1;
}

//creating nondecreasing array
static void NonDecArray(int n, int *a){
    srand(time(NULL));
    int start, finish, step;
    step = RAND_MAX/(n+1); //step for changing ranges
    start = 0;
    finish = step - 1;
    for(int i=0; i<n; i++){
        a[i] = (power_of_minus_one(i))*(start + rand()%(finish - start + 1));
        start += step; //changing ranges of start and finish to generate nondecreasing array
        finish += step;
    }
}

//create nonincreasing array
static void NonIncArray(int n, int *a){
    srand(time(NULL));
    int start, finish, step;
    step = RAND_MAX/(n+1); //step for changing ranges
    start = RAND_MAX;
    finish = RAND_MAX - step + 1;
    for(int i=0; i<n; i++){
        a[i] = (power_of_minus_one(i))*(start + rand()%(finish - start + 1));
        start -= step;//changing ranges of start and finish to generate nonincreasing array
        finish -= step;
    }
}

//create random array
static void RandArray(int n, int *a){
    srand(time(NULL));
    for(int i=0; i<n; i++){
        a[i] = (power_of_minus_one(i))*rand();
    }
}

//function for comparing, returns True if x > y, false if x <= y
static int compare(int x, int y){
    comparisons++;
    if(abs(x) > abs(y)) return 1;
    else return 0;
}

static void swap(int *a, int i, int j){
    moves++;
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;
}

static void SelectionSort(int n, int *a){
    int min_pos;
    moves = 0;
    comparisons = 0;
    //for loop gives us a starting index of array's unsorted part
    for(int i=0; i<n-1; i++){
        min_pos = i; //min_pos stores an index of the minimum element

        //find minimum element of the unsorted part
        for(int j=i+1; j<n; j++){
            if(compare(a[min_pos], a[j]))
                min_pos = j;
        }

        //if min_pos was changed, swap the first element of unsorted part with a[min_pos]
        if(min_pos != i)
            swap(a, min_pos, i);
    }
}

//heapify function puts the greatest element amongst parent and 2 children on top
static void heapify(int n, int *a, int i){
    int largest = i, l = 2*i+1, r = 2*i+2;

    //if left child is larger than the root
    if(l < n && compare(a[l], a[largest]))
        largest = l;

    //if right child is larger than the largest
    if(r < n && compare(a[r], a[largest]))
        largest = r;

    //if largest is not root
    if(largest != i){
        swap(a, largest, i);

        //calling heapify function for the sub-tree that was affected by swapping
        heapify(n, a, largest);
    }
}

static void HeapSort(int n, int *a){
    comparisons = 0;
    moves = 0;

    //create a heap
    for(int i = n/2-1; i >= 0; i--)
        heapify(n, a, i);

    //"delete" elements from heap
    for(int i = n-1; i >= 0; i--){
        //move current root to end
        swap(a, 0, i);

        //call heapify function for remaining heap
        heapify(i, a, 0);
    }
}

//comparator function that compares absolute values for qsort
static int abscmp(const void *a, const void *b) {
    return (abs(*(int*)a) - abs(*(int*)b));
}

//comparator function for qsort
static int cmp(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

//testing function
static int test(int *arr, int *tested_arr, int n){

    //1st type of error

    //using qsort with comparator function abscmp (that compares absolute values) to sort the array arr
    qsort(arr, n, sizeof(int), abscmp);
    for(int i = 0; i < n; i++){
        if(abs(arr[i]) != abs(tested_arr[i])){
            return 1;
        }
    }

    //2nd type of error

    //sorting both arrays with qsort function with comparator function cmp
    qsort(arr, n, sizeof(int), cmp);
    qsort(tested_arr, n, sizeof(int), cmp);

    //comparing the elements of arrays
    for (int i = 0; i < n; i++) {
        if (arr[i] != tested_arr[i]) {
            return 2;
        }
    }

    return 0;
}

static void print(int status, int n, int *a, int *copy, int *testcopy1, int *testcopy2){
    int test_result;
    printf("%d.\n", status);
    if(status == 1) NonDecArray(n, a);
    else if(status == 2) NonIncArray(n, a);
    else RandArray(n, a);

    memcpy(copy, a, n*sizeof(int)); //creating a copy of an array a for heap sort
    memcpy(testcopy1, a, n*sizeof(int)); //creating a copy of an array a for test function for selection sort
    memcpy(testcopy2, a, n*sizeof(int)); //creating a copy of an array a for test function for heap sort

    SelectionSort(n, a);
    printf("Selection sort: comparisons=%d, moves=%d, ", comparisons, moves);
    test_result = test(testcopy1, a, n);
    if(test_result == 0)
        printf("test passed\n");
    else
        printf("test failed with exit code %d\n", test_result);

    HeapSort(n, copy);
    printf("Heap sort: comparisons=%d, moves=%d, ", comparisons, moves);
    test_result = test(testcopy2, copy, n);
    if(test_result == 0)
        printf("test passed\n");
    else
        printf("test failed with exit code %d\n", test_result);
}

int main(void){
    int n, *a, *copy, *testcopy1, *testcopy2;
    if(scanf("%d", &n) == EOF){
        printf("input error occured\n");
        return 1;
    }
    a = (int*)malloc(n*sizeof(int));
    copy = (int*)malloc(n*sizeof(int));
    testcopy1 = (int*)malloc(n*sizeof(int));
    testcopy2 = (int*)malloc(n*sizeof(int));

    print(1, n, a, copy, testcopy1, testcopy2);
    print(2, n, a, copy, testcopy1, testcopy2);
    print(3, n, a, copy, testcopy1, testcopy2);
    print(4, n, a, copy, testcopy1, testcopy2);

    free(a); //freeing dynamic memory
    free(copy);
    free(testcopy1);
    free(testcopy2);
}
