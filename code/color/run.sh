gcc separate_all_color_channels.c -lm -pthread -o separate_all_color_channels
gcc generalize_color.c -lm -o generalize_color
gcc merge_colors.c -lm -o merge_colors

path=`pwd`
echo $path

# run separate_all_color_channels
# this is with threading so no paralell processing required
separate_all_color_channels -f $pathdata/testimage.png
