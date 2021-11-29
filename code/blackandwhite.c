//first get the black and white to work sequentialy

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

void color_to_bnw(char *picture_name){
    int width, height, channels;
    const char *fname = picture_name;

    unsigned char *img = stbi_load(fname, &width, &height, &channels, 0);

    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);

    size_t img_size = width * height * channels;
    unsigned char *new_img = (unsigned char *)malloc(img_size);


    for(unsigned char *p = img, *pg = new_img; p < img + img_size; p += channels, pg += channels) {
        
    }
    
    for(unsigned char *p = img, *pg = new_img; p < img + img_size; p += channels, pg += channels) {
        double my_value[] = {0.0,0.0,0.0};
        if((count % width == 0)||(count % width == width-1)||((count/width) % height == 0)||((count/width) % height == height-1)){ //left
            //do nothing, edge case
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
                my_value[i] = 255;
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
    stbi_write_jpg("testimageblur.png", width, height, channels, new_img, 100);
    stbi_image_free(img);stbi_image_free(new_img);
}

int main(int argc, char *argv[]){
    //runner for the black and white code
    char *filename = (char *)malloc(sizeof(char) * 100);
    while(*argv){
        if(strcmp(*argv, "-f") == 0 || strcmp(*argv, "--filename") == 0){ //filename exists in arguments
            if(*(argv+1)){ //filename is there
                filename = *(argv+1);
            }
        }
        argv++;
    }
    if(strcmp(filename,"")){
        printf("%s\n", filename);
    }
    color_to_bnw(filename);
    return 0;
}