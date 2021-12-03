#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "generalize.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb/stb_image_write.h"

#define CHANNEL_COUNT 3
#define MAX_COLOR 255
#define MIN_COLOR 0

struct img{
    // unsigned char *img;
    int width;
    int height;
    int channels;
};

struct separate_chanels_args{
    char *picture_name;
    char *output_filename;
    int colorchanel;
    struct img results;
};

struct pixel{
    int value;
    int index;
};

struct generalize_args{
    struct img imgarg;
    char *outputfilename;
    char *inputfilename;
    int color_nums;
    int color_channel;
};

struct merge_args{
    char *redfilename;
    char *greenfilename;
    char *bluefilename;
    struct img redchan;
    struct img greenchan;
    struct img bluechan;
    char *output_filename;
};



void separate_cannels(void *args){
    struct separate_chanels_args *myargs = (struct separate_chanels_args *)args;
    // printf("%i\n", myargs->colorchanel);
    int width, height, channels;
    const char *fname = myargs->picture_name;

    unsigned char *img = stbi_load(fname, &width, &height, &channels, 0);

    //printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);

    size_t img_size = width * height * channels;
    unsigned char *new_img = (unsigned char *)malloc(img_size);

    for(unsigned char *p = img, *pg = new_img; p < img + img_size; p += channels, pg += channels) {
        for(int i = 0; i < channels; i++){ //this is for the grey scale version. this will change with the addition of multiple channels
            if(myargs->colorchanel == i){
                *(pg + i) = *(p + i);
            }else{
                *(pg + i) = 0;
            }
            
        }
        if(channels == 4){
            *(pg + 3) = *(p + 3);
        }
    }

    stbi_write_jpg(myargs->output_filename, width, height, channels, new_img, 100);
    stbi_image_free(img);
    stbi_image_free(new_img);
}

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

void generalize(void *args){
    // int width, height, channels;
    //const char *fname = args->picture_name;
    struct generalize_args *myargs = (struct generalize_args *)args;
    unsigned char *img = stbi_load(myargs->inputfilename, &myargs->imgarg.width, &myargs->imgarg.height, &myargs->imgarg.channels, 0);

    //printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);
    ;
    
    size_t img_size = myargs->imgarg.width * myargs->imgarg.height * myargs->imgarg.channels;
    // struct separate_chanels_args *myargs = (struct separate_chanels_args *)args;
    
    unsigned char *new_img = (unsigned char *)malloc(img_size);

    struct pixel mypixels[myargs->imgarg.width*myargs->imgarg.height];
    
    //set of range of colors on what it should be.
    //two ways i can see on doing this,
    //set the ranges automaticly and have the color set in that range
    //set the ranges as it sees the colors. like if there are only 6 colors on the image

    int index = 0;
    // for(int i = 0; i < myargs->inputimg.height*myargs->inputimg.width; i++){
    //     printf("index : %i | value : %i\n",mypixels[i].index, mypixels[i].value);
    // }
    // printf("%s\n", myargs->inputimg.img);
    printf("%i\n", myargs->imgarg.width*myargs->imgarg.height);
    for(unsigned char *p = img; p < img + img_size; p += myargs->color_channel) {
        //new plan, get all the colors in a sorted list with indexes
        mypixels[index].index = index;
        mypixels[index].value = *(p + myargs->color_channel);
        printf("%i\n", mypixels[index].index);
        index++;
    }
    //sort the list according to value


    struct pixel *sortedmypixels = mergeSort(mypixels, myargs->imgarg.width*myargs->imgarg.height);

    // printf("%i\n", )

    int max_value = sortedmypixels[(myargs->imgarg.height*myargs->imgarg.width)-1].value;
    // printf("%i\n", max_value);
    int min_value = sortedmypixels[0].value;
    int interable_value = (myargs->imgarg.width*myargs->imgarg.height)/myargs->color_nums;
    int grouped_values[myargs->color_nums];
    int count = 0;
    int average = 0;
    int color_count = 0;


    for(int i = 0; i < (myargs->imgarg.width*myargs->imgarg.height); i++){
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
    for(int i = 0; i < (myargs->imgarg.width*myargs->imgarg.height); i++){
        new_img[(sortedmypixels[i].index*myargs->imgarg.channels) + myargs->color_channel] = grouped_values[count];
        if(myargs->imgarg.channels == 4){
            new_img[(sortedmypixels[i].index*myargs->imgarg.channels) + 3] = img[(sortedmypixels[i].index*myargs->imgarg.channels) + 3];
        }
        if(i%interable_value == 0 && i != 0){
            count++;
        }
    }

    stbi_write_jpg(myargs->outputfilename, myargs->imgarg.width, myargs->imgarg.height, myargs->imgarg.channels, new_img, 100);
    stbi_image_free(img);
    stbi_image_free(new_img);
}

void merge(void *args){
    struct merge_args *myargs = (struct merge_args *)args;
    int width, height, channels;

    unsigned char *redimg = stbi_load(myargs->redfilename, &width, &height, &channels, 0);
    unsigned char *greenimg = stbi_load(myargs->greenfilename, &width, &height, &channels, 0);
    unsigned char *blueimg = stbi_load(myargs->bluefilename, &width, &height, &channels, 0);

    //printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);

    size_t img_size = myargs->redchan.width * myargs->redchan.height * myargs->redchan.channels;
    unsigned char *new_img = (unsigned char *)malloc(img_size);

    for(unsigned char *r = redimg, *g = greenimg, *b = blueimg, *pg = new_img; r < redimg + img_size;
    r += myargs->redchan.channels, g += myargs->redchan.channels, b += myargs->redchan.channels, pg += myargs->redchan.channels) {
        //this will only work if the other chanels of the color picutres are arrays of 0's
        *(pg + 0) = *(r + 0);
        *(pg + 1) = *(g + 1);
        *(pg + 2) = *(b + 2);
        // printf("red : %i | green : %i | blue : %i\n", *(r+0), *(g+1) ,*(b+2));
        if(myargs->redchan.channels == 4){ //freak out!!!
            *(pg + 3) = *(r + 3);
        }
    }

    stbi_write_jpg(myargs->output_filename, myargs->redchan.width, myargs->redchan.height, myargs->redchan.channels, new_img, 100);
    stbi_image_free(new_img);
}

void generalize_img_parallel(char *input_filename, char *output_filename, int colors_per_channel){
    //complete runner for generlizing a color 
    
    pthread_t separate_cannels_pthread[CHANNEL_COUNT];
    pthread_t generalize_pthread[CHANNEL_COUNT];

    struct separate_chanels_args struct_args[CHANNEL_COUNT];
    struct generalize_args gargs[CHANNEL_COUNT];
    struct merge_args *margs;

    char *outputfilenames[] = {"../../data/redchanel.png", "../../data/greenchanel.png", "../../data/bluechanel.png"};
    char *outputfilenames2[] = {"../../data/redchanelgen.png", "../../data/greenchanelgen.png", "../../data/bluechanelgen.png"};
    printf("separate channels\n");
    for(int i = 0; i < CHANNEL_COUNT; i++){
        struct_args[i].picture_name = input_filename;
        struct_args[i].output_filename = outputfilenames[i];
        struct_args[i].colorchanel = i;
        if(pthread_create(&separate_cannels_pthread[i], NULL, (void *)separate_cannels, (void*)&struct_args[i]) != 0){
            //thread creation did not happen;
            printf("error in thread cereation\n");
        }
    }
    for(int i = 0; i < CHANNEL_COUNT; i++){
        pthread_join(separate_cannels_pthread[i], NULL);
    }
    printf("generalize\n");
    for(int i = 0; i < CHANNEL_COUNT; i++){
        gargs[i].inputfilename = output_filename[i];
        gargs[i].outputfilename = outputfilenames2[i];
        gargs[i].color_channel = i;
        gargs[i].color_nums = colors_per_channel;
        gargs[i].imgarg = struct_args[i].results;
        printf("result %i : width : %i : height : %i\n", i, gargs[i].imgarg.width, gargs[i].imgarg.height);
        if(pthread_create(&generalize_pthread[i], NULL, (void *)generalize, (void*)&gargs[i]) != 0){
            //thread creation did not happen;
            printf("error in thread cereation\n");
        }
        // pthread_join(generalize_pthread[i], NULL);
    }
    for(int i = 0; i < CHANNEL_COUNT; i++){
        pthread_join(generalize_pthread[i], NULL);
    }
    // printf("merge\n");
    // margs->redchan = gargs[0].outputimg;
    // margs->greenchan = gargs[1].outputimg;
    // margs->bluechan = gargs[2].outputimg;
    // margs->output_filename = output_filename;
    // merge(margs);
}