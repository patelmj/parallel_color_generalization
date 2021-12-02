//blur example in cuda
//first need a sample image 1200*1200

//#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

double kernel[] = {1.0/9.0,1.0/9.0,1.0/9.0,1.0/9.0,1.0/9.0,1.0/9.0,1.0/9.0,1.0/9.0,1.0/9.0};

int blur(char *filename, char* output_filename){
    
    int width, height, channels;
    unsigned char *img = stbi_load(filename, &width, &height, &channels, 0);

    // printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);

    size_t img_size = width * height * channels;
    unsigned char *new_img = (unsigned char *)malloc(img_size);
    int count = 0;

    double my_value[3];
    for(unsigned char *p = img, *pg = new_img; p < img + img_size; p += channels, pg += channels) {
        
        for(int i= 0; i < 3; i++){
            my_value[i] = 0.0; //reset value
        }
        
        if((count % width == 0)||(count % width == width-1)||((count/width) % height == 0)||((count/width) % height == height-1)){ //left
            //do nothing, edge case
            //print the origional pixels?
        }else{
            //interior
            for(int i = 0; i < sizeof(my_value); i++){
                my_value[i] = my_value[i] + *(p + i - (width * channels) - channels) * kernel[0];
                my_value[i] = my_value[i] + *(p + i - (width * channels)) * kernel[1];
                my_value[i] = my_value[i] + *(p + i - (width * channels) + channels) * kernel[2];
                my_value[i] = my_value[i] + *(p + i - channels) * kernel[3];
                my_value[i] = my_value[i] + *(p + i) * kernel[4];
                my_value[i] = my_value[i] + *(p + i + (width * channels)) * kernel[5];
                my_value[i] = my_value[i] + *(p + i + (width * channels) - channels) * kernel[6];
                my_value[i] = my_value[i] + *(p + i + (width * channels)) * kernel[7];
                my_value[i] = my_value[i] + *(p + i - (width * channels) + channels) * kernel[8];
            }
        }

        for(int i = 0; i < sizeof(my_value); i++){
            if(my_value[i] > 255.0){
                my_value[i] = 255.0;
            }else if(my_value[i] < 0.0){
                my_value[i] = 0.0;
            }
        }
        
        *(pg + 0) = (uint8_t)(my_value[0]);//red
        *(pg + 1) = (uint8_t)(my_value[1]);//blue
        *(pg + 2) = (uint8_t)(my_value[2]);//green
    
        //printf("|| Count: %i || Red: %i | Blue: %i | Green: %i\n", count, *pg, *(pg+1), *(pg+2));
        if(channels == 4) {
            *(pg + 3) = *(p + 3);
        }
        count++;
    }
    printf("here\n");
    

    stbi_write_jpg(output_filename, width, height, channels, new_img, 100);
    stbi_image_free(img);
    stbi_image_free(new_img);
    return 0;
}

int main(int argc, char *argv[]){
    //get the filename. same as blackandwhite.c
    char *filename = (char *)malloc(sizeof(char) * 100);
    char *output_filename = (char *)malloc(sizeof(char) * 100);
    while(*argv){
        if(strcmp(*argv, "-f") == 0 || strcmp(*argv, "--filename") == 0){ //filename exists in arguments
            if(*(argv+1)){ //filename is there
                filename = *(argv+1);
            }
        }else if(strcmp(*argv, "-o") == 0 || strcmp(*argv, "--outputfilename") == 0){ //filename exists in arguments
            if(*(argv+1)){ //filename is there
                output_filename = *(argv+1);
            }
        }
        argv++;
    }
    if(strcmp(filename,"") && strcmp(output_filename,"")){
        blur(filename, output_filename);
    }else{
        //filename not given or number of colors is not right
        printf("args are not correct");
    }
    return 0;
}
