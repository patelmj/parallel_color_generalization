#ifdef GENERALIZE_H
#define GENERALIZE_H

void separate_cannels(void *args);

struct pixel* mergeSort(struct pixel *input, int size);
void generalize(char *picture_name, char *output_filename, int number_of_colors, int color_channel);
void merge(char *redpicture_name, char *greenpicture_name, char *bluepicture_name, char *output_filename);
void generalize_img_parallel(char *input_filename, char *output_filename, int colors_per_channel);
void generalize_img_sequencial(char *input_filename, char *output_filename, int colors_per_channel);

#endif
