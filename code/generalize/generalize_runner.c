#include "generalize.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
// #include <pthread.h>

int main(int argc, char *argv[]){
    //get the filename. same as blackandwhite.c
    char *filename = (char *)malloc(sizeof(char) * 100);
    char *output_filename = (char *)malloc(sizeof(char) * 100);
    int numberofcolors;
    while(*argv){
        if(strcmp(*argv, "-f") == 0 || strcmp(*argv, "--filename") == 0){ //filename exists in arguments
            if(*(argv+1)){ //filename is there
                filename = *(argv+1);
            }
        }else if(strcmp(*argv, "-o") == 0 || strcmp(*argv, "--outputfilename") == 0){ //filename exists in arguments
            if(*(argv+1)){ //filename is there
                output_filename = *(argv+1);
            }
        }else if(strcmp(*argv, "-c") == 0 || strcmp(*argv, "--color_nums") == 0){ //filename exists in arguments
            if(*(argv+1)){ //filename is there
                numberofcolors = atoi(*(argv+1));
            }
        }
        argv++;
    }
    if(strcmp(filename,"") && strcmp(output_filename,"") && numberofcolors > 0){
        printf("it works\n");
        generalize_img_parallel(filename, output_filename, numberofcolors);

    }else{
        //filename not given or number of colors is not right
        printf("args are not correct\n");
    }
    return 0;
}