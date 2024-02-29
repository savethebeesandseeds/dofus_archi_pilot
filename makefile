ifeq ($(MAKEFLAGS),)  # Check if MAKEFLAGS already set from the environment
   MAKEFLAGS := -j4
endif

main:
	x86_64-w64-mingw32-g++ -Wall -o \
		./build/archi_pilot.exe main.cpp \
		-I./include -I/external/opencv/include \
		-I/external/opencv/build/ \
		-I/external/opencv/modules/core/include/ \
		-I/external/opencv/modules/imgproc/include/ \
		-I/external/opencv/modules/imgcodecs/include/ \
		-I/external/mingw-std-threads/generated_std_headers \
		-L/external/opencv/build/lib -L/external/opencv/build/3rdparty/lib \
		-lopencv_imgcodecs490 -lopencv_imgproc490 -lopencv_core490 \
		-l:libIlmImf.a -l:libzlib.a -l:liblibopenjp2.a \
		-l:liblibjpeg-turbo.a -l:liblibpng.a -l:liblibtiff.a -l:liblibwebp.a \
		-std=c++11 -static-libgcc -static-libstdc++ -lgdi32 -fopenmp -static
