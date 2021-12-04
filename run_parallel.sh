path=`pwd`
color_nums=2
# echo $path
gcc $path/code/color/separate_color_channels.c -lm -o separate_color_channels
gcc $path/code/color/generalize_color.c -lm -o generalize_color
gcc $path/code/color/merge_colors.c -lm -o merge_colors
# run separate_all_color_channels
$path/separate_color_channels -f $path/data/testimage.png -o $path/redchanel.png -c 0 &
redid=$!
$path/separate_color_channels -f $path/data/testimage.png -o $path/greenchanel.png -c 1 &
greenid=$!
$path/separate_color_channels -f $path/data/testimage.png -o $path/bluechanel.png -c 2 &
blueid=$!
wait $redid
wait $blueid
wait $greenid

# now for the generalize_color, this will be paralell processed
$path/generalize_color -f $path/redchanel.png -o $path/redchanelgen.png -c $color_nums -h 0 &
redid=$!
$path/generalize_color -f $path/greenchanel.png -o $path/greenchanelgen.png -c $color_nums -h 1 &
greenid=$!
$path/generalize_color -f $path/bluechanel.png -o $path/bluechanelgen.png -c $color_nums -h 2 &
blueid=$!
wait $redid
wait $greenid
wait $blueid
# now merge all the files
$path/merge_colors -f $path/redchanelgen.png $path/greenchanelgen.png $path/bluechanelgen.png -o generalize.png
# now remove all of the files associated with it
rm $path/redchanel.png $path/bluechanel.png $path/greenchanel.png $path/redchanelgen.png $path/bluechanelgen.png $path/greenchanelgen.png
rm $path/generalize_color $path/merge_colors $path/separate_color_channels
# done