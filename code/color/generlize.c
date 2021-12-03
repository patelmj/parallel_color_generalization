//this is for the generalization of the bnw image

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb/stb_image_write.h"

//old way of doing it. specify args when running the code
// #define NUMBER_OF_COLORS 3
#define MAX_COLOR 255
#define MIN_COLOR 0

struct pixel{
    int value;
    int index;
};

struct pixel* mergeSort(struct pixel *input, int size){
    //need to get the list size down to to and then sort that
    if(size >= 2){
        //list is not small yet keep going down
        //now i need to split the list
        struct pixel *leftlist, *rightlist;
        leftlist = (struct pixel *)malloc(sizeof(struct pixel) * (size/2));
        rightlist = (struct pixel *)malloc((sizeof(struct pixel) * (size/2)) + sizeof(struct pixel));
        
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

        struct pixel *return_list = (struct pixel *)malloc(sizeof(struct pixel) * size);
        count = 0;
        while(rightlist->value || leftlist->value){
            if(rightlist->value >= leftlist->value){
                *(return_list+count) = *rightlist;
                rightlist++;
            }else{
                *(return_list+count) = *leftlist;
                leftlist++;
            }
            count++;
            // printf("%i\n", return_list->value);
        }
        return return_list;
    }else if(size == 1){
        //if it is one then return the list
        return input;
    }
}

void generlize(char *picture_name, char *output_filename, int number_of_colors, int color_channel){
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
        mypixels[index].value = *(p + color_channel);
        // printf("%i\n", *);
        index++;
    }

    //sort the list according to value
    //FIXME - this can be improved with a diffrent type sort
    //not completly correct anyway. the begining index is not sorted
    // struct pixel temp;
    // for(int i = 1; i <= (width*height); i++){
    //     for(int j = i; j >= 0; j--){
    //         if(mypixels[j-1].index){ //if value can be compared
    //             if(mypixels[j].value < mypixels[j-1].value){ //swap values
    //                 temp = mypixels[j];
    //                 mypixels[j] = mypixels[j-1];
    //                 mypixels[j-1] = temp;
    //             }
    //         }
    //     }
    // }
    struct pixel *sortedmypixels = mergeSort(mypixels, width*height);

    // for(int i = 0; i < width*height; i++){
    //     printf("index : %i | value : %i\n",sortedmypixels[i].index, sortedmypixels[i].value);
    // }

    int max_value = sortedmypixels[(height*width)-1].value;
    int min_value = sortedmypixels[0].value;
    int interable_value = (width*height)/number_of_colors;
    int grouped_values[number_of_colors];

    int count = 0;
    int average = 0;
    int color_count = 0;
    for(int i = 0; i < (width*height); i++){
        //i need to sum up all of the values in the color range
        average = average + sortedmypixels[i].value;
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
        new_img[(sortedmypixels[i].index*channels) + color_channel] = grouped_values[count];
        if(channels == 4){
            new_img[(sortedmypixels[i].index*channels) + 3] = img[(sortedmypixels[i].index*channels) + 3];
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
    int numberofcolors, channel;
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
        }else if(strcmp(*argv, "-h") == 0 || strcmp(*argv, "--channel") == 0){ //filename exists in arguments
            if(*(argv+1)){ //filename is there
                channel = atoi(*(argv+1));
            }
        }

        argv++;
    }
    if(strcmp(filename,"") && strcmp(output_filename,"") && numberofcolors > 0 && channel >= 0 && channel<= 2){
        generlize(filename, output_filename, numberofcolors, channel);
    }else{
        //filename not given or number of colors is not right
        printf("args are not correct");
    }
    return 0;
}