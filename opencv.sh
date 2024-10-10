wget -O opencv.zip https://github.com/opencv/opencv/archive/3.3.0.zip
unzip opencv.zip

mkdir -p build && cd build
 

cmake  ../opencv
 
# Build
make
sudo make install
