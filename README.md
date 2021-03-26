## How to

### Install online library for raw-data decoding

mkdir build_onlinelib

cd build_onlinelib

cmake -DCMAKE_INSTALL_PREFIX=/path/to/installation_dir /path/to/brikenmacros/onlinlib

source /path/to/installation_dir/bin/setuponline.sh

*Note: to add to .bashrc: echo /path/to/installation_dir/bin/setuponline.sh >> ~/.bashrc*

###  Run your useranalysis code (example: brikenmacros/useranalysis

mkdir build_userana

cd build_userana

cmake /path/to/your/userana

make
