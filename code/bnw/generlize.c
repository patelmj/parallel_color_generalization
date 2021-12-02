//this is for the generalization of the bnw image

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

//old way of doing it. specify args when running the code
// #define NUMBER_OF_COLORS 3
#define MAX_COLOR 255
#define MIN_COLOR 0

struct pixel{
    int value;
    int index;
};

void generlize(char *picture_name, char *output_filename, int number_of_colors){
    int width, height, channels;
    const char *fname = picture_name;

    unsigned char *img = stbi_load(fname, &width, &height, &channels, 0);

    //printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);

    size_t img_size = width * height * channels;
    unsigned char *new_img = (unsigned char *)malloc(img_size);


    struct pixel mypixels[width*height];
    //set of range of colors on what it should be.
    //two ways i can see on doing this,
    //set the ranges automaticly and have the color set in that range
    //set the ranges as it sees the colors. like if there are only 6 colors on the image

    int index = 0;
    for(unsigned char *p = img; p < img + img_size; p += channels) {
        //new plan, get all the colors in a sorted list with indexes
        mypixels[index].index = index;
        mypixels[index].value = *p;
        index++;
    }

    //sort the list according to value
    //FIXME - this can be improved with a diffrent type sort
    //not completly correct anyway. the begining index is not sorted
    struct pixel temp;
    for(int i = 1; i <= (width*height); i++){
        for(int j = i; j >= 0; j--){
            if(mypixels[j-1].index){ //if value can be compared
                if(mypixels[j].value < mypixels[j-1].value){ //swap values
                    temp = mypixels[j];
                    mypixels[j] = mypixels[j-1];
                    mypixels[j-1] = temp;
                }
            }
        }
    }

    int max_value = mypixels[(height*width)-1].value;
    int min_value = mypixels[0].value;
    int interable_value = (width*height)/number_of_colors;
    int grouped_values[number_of_colors];

    int count = 0;
    int average = 0;
    int color_count = 0;
    for(int i = 0; i < (width*height); i++){
        //i need to sum up all of the values in the color range
        average = average + mypixels[i].value;
        count++;
        if(count == interable_value){
            //stop counting and average
            average = average / interable_value;
            grouped_values[color_count] = average;
            color_count++;
            average = 0;
            count = 0;
        }
    }

    //now write to the image
    count = 0;
    for(int i = 0; i < (height*width); i++){
        for(int j = 0; j < 3; j++){
            new_img[(mypixels[i].index*channels) + j] = grouped_values[count];
        }
        if(channels == 4){
            new_img[(mypixels[i].index*channels) + 3] = img[(mypixels[i].index*channels) + 3];
        }
        if(i%interable_value == 0 && i != 0){
            count++;
        }
    }

    stbi_write_jpg(output_filename, width, height, channels, new_img, 100);
    stbi_image_free(img);
    stbi_image_free(new_img);
}


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
        }else if(strcmp(*argv, "-c") == 0 || strcmp(*argv, "--colornums") == 0){ //filename exists in arguments
            if(*(argv+1)){ //filename is there
                numberofcolors = atoi(*(argv+1));
            }
        }else if(strcmp(*argv, "-o") == 0 || strcmp(*argv, "--outputfilename") == 0){ //filename exists in arguments
            if(*(argv+1)){ //filename is there
                output_filename = *(argv+1);
            }
        }
        argv++;
    }
    if(strcmp(filename,"") && strcmp(output_filename,"") && numberofcolors > 0){
        generlize(filename, output_filename, numberofcolors);
    }else{
        //filename not given or number of colors is not right
        printf("args are not correct");
    }
    return 0;
}