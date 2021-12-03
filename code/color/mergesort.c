//merge sort with pthreads

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 262144

int* mergeSort(int* input, int size){
    //need to get the list size down to to and then sort that
    if(size >= 2){
        //list is not small yet keep going down
        //now i need to split the list
        int *leftlist, *rightlist;
        leftlist = (int *)malloc(sizeof(int) * (size/2));
        rightlist = (int *)malloc((sizeof(int) * (size/2)) + sizeof(int));
        
        int count = 0;
        int index_count = 0;
        while(index_count <= size-1){
            if(index_count > (size/2)){
                //count is bigger or equal to than half of the list
                rightlist[count] = *input;
            }else if(index_count == (size/2)){
                count = 0;
                rightlist[count] = *input;
            }else{
                leftlist[count] = *input;
            }
            input++;
            index_count++;
            count++;
        }

        if(size % 2 == 1){
            //odd number when you devide by 2
            //right list will always be greater than the left list
            rightlist = mergeSort(rightlist, ((size/2)+1));
            leftlist = mergeSort(leftlist, (size/2));
        }else{
            //very simmilar to above but with one size instead of 2
            rightlist = mergeSort(rightlist, (size/2));
            leftlist = mergeSort(leftlist, (size/2));
        }
        //now merge the two lists

        int* return_list = (int *)malloc(sizeof(int) * size);
        count = 0;
        while(*rightlist || *leftlist){
            if(*rightlist >= *leftlist){
                *(return_list+count) = *rightlist;
                rightlist++;
            }else{
                *(return_list+count) = *leftlist;
                leftlist++;
            }
            
            count++;
        }
        return return_list;
    }else if(size == 1){
        //if it is one then return the list
        return input;
    }
}

int main(){
    int *testinput = (int *)malloc((sizeof(int) * ARRAY_SIZE));
    time_t t;

    srand((unsigned) time(&t));

    printf("before\n");
    for(int i = 0; i < ARRAY_SIZE; i++){
        testinput[i] = (rand() % ARRAY_SIZE) + 1;
        printf("%i\n", testinput[i]);
    }
    //tester
    testinput = mergeSort(testinput, ARRAY_SIZE);
    printf("after\n");
    for(int i = 0; i < ARRAY_SIZE; i++){
        printf("%i\n", testinput[i]);
    }
    return 0;
}


