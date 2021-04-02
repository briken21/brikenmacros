## INSTALLATION INSTRUCTION

Installation of OnlineLibraries developed by Phong using root.6.20.00 (It should work the same for other versions).
NOTE: It is necessary to install gsl libraries in our root. Tutorial for this part: https://techforcurious.website/cern-root-installation-ubuntu-18-04-enabling-libraries/.


Installation of onlinelib:

1) First we must download the directory using for example: git clone https://techforcurious.website/cern-root-installation-ubuntu-18-04-enabling-libraries/

2) Then we should have a main directory called brikenmacros that contain the directory onlinelib. We need to create the build directory the execute the cmake command. In my case I created this directory inside onlinelib:

cd  onlinelib
mkdir build_onlinelib
cd build_onlinelib

3) Then we run the cmake (or cmake3): cmake -DCMAKE_INSTALL_PREFIX=/path/to/installation_dir /path/to/brikenmacros/onlinelib.

I decided to create the installation directory inside the brikenmacros directory. So the command in my PC should be:
cmake3 -DCMAKE_INSTALL_PREFIX=/home/maxpallas/data/briken2021/brikenmacros/OnlineLibraries_Test ../
NOTE: Some errors related to Boost and cppzmq appear but you can ignore them.

make install

4) The last step is to set the environment variables using:

source /path/to/installation_dir/bin/setuponline.sh

In my case: source /home/maxpallas/data/briken2021/brikenmacros/OnlineLibraries_Test/bin/setuponline.sh
5) Now everything should work properly.  Using myanalysis.cc some warnings appear but the code seems to run properly. 


