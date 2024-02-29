# Note
You don't need to build, the file *./build/archi_pilot.exe* should work on Windows. 

If you still want or need to build from source, here are the instructions. 

# Build from source
It is compiled in Linux, but targeted to work with windows 10 or higher. 

### Requirements
Install Docker Desktop, reboot your machine.

### Download or clone this Repo
> https://github.com/savethebeesandseeds/dofus_archi_pilot


### Create the build docker enviroment
Open CMD and type these commands:

> cd /path/to/dofus_archi_pilot

> docker pull debian:11

> docker run -it --name=archi_pilot -v .:/src debian:11 

> docker exec -it archi_pilot /bin/bash

### install linux dependencies
Now you are no longer in Windows, but inside a Linux Docker. Type these commands.

> apt update

> apt install mingw-w64 make cmake g++ git --no-install-recommends -y

> apt install --reinstall ca-certificates --no-install-recommends -y

> mkdir /external

> cd /external

### Download mingw-std-threads (as MinGW does not support linux threads)

> git clone https://github.com/meganz/mingw-std-threads

> cp /src/resources/generate-std-like-headers.sh /external/mingw-std-threads

> 

> chmod +x generate-std-like-headers.sh

> ./generate-std-like-headers.sh

### Download OpenCV source

> git clone https://github.com/opencv/opencv.git

### Build OpenCV from source to target a Windows enviroment

> cp /src/resources/mingw64_toolchain.cmake /external/opencv/

> cd /external/opencv/

> mkdir build

> cd /external/opencv/build

> cmake -DCMAKE_CXX_STANDARD=11 \
      -DBUILD_SHARED_LIBS=OFF \
      -DBUILD_opencv_calib3d=OFF \
      -DBUILD_opencv_calib3d=OFF \
      -DBUILD_opencv_dnn=OFF \
      -DBUILD_opencv_features2d=OFF \
      -DBUILD_opencv_flann=OFF \
      -DBUILD_opencv_gapi=OFF \
      -DBUILD_opencv_highgui=OFF \
      -DBUILD_opencv_java=OFF \
      -DBUILD_opencv_js=OFF \
      -DBUILD_opencv_ml=OFF \
      -DBUILD_opencv_objc=OFF \
      -DBUILD_opencv_objdetect=OFF \
      -DBUILD_opencv_photo=OFF \
      -DBUILD_opencv_python3=OFF \
      -DBUILD_opencv_python2=OFF \
      -DBUILD_opencv_stitching=OFF \
      -DBUILD_opencv_ts=OFF \
      -DBUILD_opencv_video=OFF \
      -DBUILD_opencv_videoio=OFF \
      -DBUILD_opencv_world=OFF \
      -DBUILD_PROTOBUF=OFF \
      -DCMAKE_TOOLCHAIN_FILE=/external/opencv/mingw64_toolchain.cmake \
      /external/opencv

> make -j$(nproc)

### Validate 
Make sure the installation works. It should, that's a nice thing about Docker. 

### Compile the auto_pilot code
> cd /src

> make main -k$(nproc)

### Run
Check insturctions on *./README.md*