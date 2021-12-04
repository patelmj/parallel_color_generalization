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

void separate_color_channels(char *picture_name, char *output_filename, int colorchanel){
    int width, height, channels;
    const char *fname = picture_name;

    unsigned char *img = stbi_load(fname, &width, &height, &channels, 0);

    //printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);

    size_t img_size = width * height * channels;
    unsigned char *new_img = (unsigned char *)malloc(img_size);

    for(unsigned char *p = img, *pg = new_img; p < img + img_size; p += channels, pg += channels) {
        for(int i = 0; i < channels; i++){ //this is for the grey scale version. this will change with the addition of multiple channels
            if(colorchanel == i){
                *(pg + i) = *(p + i);
            }else{
                *(pg + i) = 0;
            }
            
        }
        if(channels == 4){
            *(pg + 3) = *(p + 3);
        }
    }

    stbi_write_jpg(output_filename, width, height, channels, new_img, 100);
    stbi_image_free(img);
    stbi_image_free(new_img);
}


int main(int argc, char *argv[]){
    //runner for the black and white code
    char *filename = (char *)malloc(sizeof(char) * 100);
    char *output_filename = (char *)malloc(sizeof(char) * 100);
    int colorchanel;
    while(*argv){
        if(strcmp(*argv, "-f") == 0 || strcmp(*argv, "--filename") == 0){ //filename exists in arguments
            if(*(argv+1)){ //filename is there
                filename = *(argv+1);
            }
        }else if(strcmp(*argv, "-o") == 0 || strcmp(*argv, "--outputfilename") == 0){ //output file name
            if(*(argv+1)){ //filename is there
                output_filename = *(argv+1);
            }
        }else if(strcmp(*argv, "-c") == 0 || strcmp(*argv, "--colorchanel") == 0){ //0 = red | 1 == blue | 2 == green
            if(*(argv+1)){ //filename is there
                colorchanel = atoi(*(argv+1));
            }
        }
        argv++;
    }
    if(strcmp(filename,"") && strcmp(output_filename, "") && colorchanel >= 0 && colorchanel <= 2){
        separate_color_channels(filename, output_filename, colorchanel);
    }else{
        printf("args are not correct");
    }
    return 0;
}
