# Set the system name to Windows
set(CMAKE_SYSTEM_NAME Windows)

# ensure standard flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify the cross-compiler locations
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

# Specify where to look for the target environment binaries (this might need to be adjusted for your system)
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

# Adjust the default behaviour of the FIND_ commands: search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# For libraries and headers in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Specify the processor
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# pthread flgas
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DWIN32_LEAN_AND_MEAN")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWIN32_LEAN_AND_MEAN")
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_INCLUDE_PATH "/external/mingw-std-threads;${CMAKE_INCLUDE_PATH}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/external/mingw-std-threads")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/external/mingw-std-threads/generated_std_headers" CACHE STRING "" FORCE)
