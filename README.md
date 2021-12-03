# Color Generalization
Stencil outline final project for parallel programming.

# Steps for final project
Get the black and white color code done
Got the color combination blocks done

# Compiling
(11/28/21) - STB libariay not working unless you add -lm in the compile statment EG: "gcc blackandwhite.c -lm" | This is for the linking of the librairy
(11/30/21) - to run a command in linux and have it not wait on it is run with the & symbol attached. this is useful for unning multi processing events
(12/1/21) - i copied the blur.cu and added arguments and had it transposed to c instead of cu. the stack smashing is still occuring but i am looking into it with the "-fno-stack-protector". this turns off the protection for stack smashing or canaries
(12/2/21) - things left to do : debug the blur stack smashing error. timing of the whole thing in order. eparate the channels and get a merge function going. The paralell version with threading.
(12/3/21) - to run the generalized command "gcc generalize.c generalize_runner.c -lm -pthread" then "./a.out -f ../../data/testimage.png -o ../../data/generlized.png -c 3"