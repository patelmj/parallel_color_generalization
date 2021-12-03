#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb/stb_image_write.h"

#define CHANNEL_COUNT 3

struct separate_chanels_args{
    char *picture_name;
    char *output_filename;
    int colorchanel;
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

int main(int argc, char *argv[]){
    //get the filename. same as blackandwhite.c
    char *filename = (char *)malloc(sizeof(char) * 100);
    // char *output_filename = (char *)malloc(sizeof(char) * 100);
    // int numberofcolors;
    while(*argv){
        if(strcmp(*argv, "-f") == 0 || strcmp(*argv, "--filename") == 0){ //filename exists in arguments
            if(*(argv+1)){ //filename is there
                filename = *(argv+1);
            }
        }
        argv++;
    }
    if(strcmp(filename,"")){
        //pthreads go here

        pthread_t separate_cannels_pthread[CHANNEL_COUNT];
        struct separate_chanels_args struct_args[3];

        char *outputfilenames[] = {"../../data/redchanel.png", "../../data/greenchanel.png", "../../data/bluechanel.png"};
        for(int i = 0; i < CHANNEL_COUNT; i++){
            struct_args[i].picture_name = filename;
            struct_args[i].output_filename = outputfilenames[i];
            struct_args[i].colorchanel = i;
            if(pthread_create(&separate_cannels_pthread[i], NULL, (void *)separate_cannels, (void*)&struct_args[i]) != 0){
                //thread creation did not happen;

                printf("error in thread cereation\n");
            }
        }

        for(int i = 0; i < CHANNEL_COUNT; i++){
            pthread_join(separate_cannels_pthread[i],NULL);
        }

    }else{
        //filename not given or number of colors is not right
        printf("args are not correct");
    }
    return 0;
}