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
	double temp_greyscale = 0.0;
	for(int i = 0; i < channels; i++){
	    temp_greyscale = temp_greyscale + *(p + i);
	}
	temp_greyscale = temp_greyscale/3.0;
	printf("%i\n",(int)temp_greyscale);
	for(int i = 0; i < channels; i++){
	    *(pg + i) = (int)temp_greyscale;
	}
	if(channels == 4){
	    *(pg + 3) = *(p + 3);
	}
    }

    stbi_write_jpg("bnwimage.png", width, height, channels, new_img, 100);
    stbi_image_free(img);
    stbi_image_free(new_img);
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
        color_to_bnw(filename);
    }

    return 0;
}
