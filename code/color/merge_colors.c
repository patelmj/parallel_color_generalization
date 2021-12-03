//first get the black and white to work sequentialy

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb/stb_image_write.h"

//this whole thing can be done in parallel. somthing to lookino with pthreads

void merge(char *redpicture_name, char *greenpicture_name, char *bluepicture_name, char *output_filename){
    int width, height, channels;

    unsigned char *redimg = stbi_load(redpicture_name, &width, &height, &channels, 0);
    unsigned char *greenimg = stbi_load(greenpicture_name, &width, &height, &channels, 0);
    unsigned char *blueimg = stbi_load(bluepicture_name, &width, &height, &channels, 0);

    //printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);

    size_t img_size = width * height * channels;
    unsigned char *new_img = (unsigned char *)malloc(img_size);

    for(unsigned char *r = redimg, *g = greenimg, *b = blueimg, *pg = new_img; r < redimg + img_size;
    r += channels, g += channels, b += channels, pg += channels) {
        //this will only work if the other chanels of the color picutres are arrays of 0's
        *(pg + 0) = *(r + 0);
        *(pg + 1) = *(g + 1);
        *(pg + 2) = *(b + 2);
        // printf("red : %i | green : %i | blue : %i\n", *(r+0), *(g+1) ,*(b+2));
        if(channels == 4){ //freak out!!!
            *(pg + 3) = *(r + 3);
        }
    }

    stbi_write_jpg(output_filename, width, height, channels, new_img, 100);
    stbi_image_free(redimg);
    stbi_image_free(greenimg);
    stbi_image_free(blueimg);
    stbi_image_free(new_img);
}


int main(int argc, char *argv[]){
    //runner for the black and white code
    char *redfilename = (char *)malloc(sizeof(char) * 100);
    char *greenfilename = (char *)malloc(sizeof(char) * 100);
    char *bluefilename = (char *)malloc(sizeof(char) * 100);
    char *output_filename = (char *)malloc(sizeof(char) * 100);
    while(*argv){
        if(strcmp(*argv, "-f") == 0 || strcmp(*argv, "--filenames") == 0){ //filename exists in arguments
            if(*(argv+1)&&*(argv+2)&&*(argv+3)){ //filename is there
                redfilename = *(argv+1);
                greenfilename = *(argv+2);
                bluefilename = *(argv+3);
            }
        }else if(strcmp(*argv, "-o") == 0 || strcmp(*argv, "--outputfilename") == 0){ //filename exists in arguments
            if(*(argv+1)){ //filename is there
                output_filename = *(argv+1);
            }
        }
        argv++;
    }
    if(strcmp(redfilename,"") && strcmp(greenfilename,"") && strcmp(bluefilename,"") && strcmp(output_filename, "")){
        merge(redfilename, greenfilename, bluefilename, output_filename);
    }else{
        printf("args are not correct");
    }
    return 0;
}
