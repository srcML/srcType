Lightweight tool for static type resolution

To build srcType:

Clone the repository with 'git clone --recursive' into the desired directory. Make sure you include the --recursive as srcType includes a submodule that must also be cloned.

Outside of the cloned directory, create a new directory for the build. (This guide assumes the new directory is at the same directory tree level as the cloned directory)

Enter the new directory (not the cloned one) and type 'cmake ../{cloned directory}'

After cmake runs, simply type 'make' and all files should be built.

Once everything is built, go into the 'bin' folder and that's where the executable will be.

If you'd like to run the tests, you will need to have srcML installed on your computer. It can be obtained here: http://www.srcml.org/downloads.html

***NOTICE***

When you run srcml to create an archive that will be used with srcType, make sure you run srcml with the --position option.  srcType requires position information.

i.e., ./srcml {filename} --position
