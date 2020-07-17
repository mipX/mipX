# mipX
Medical Image Processing Cross-Platform Toolkit

Current versions of libs:

GDCM 2.4.4
NIFTICLIB 2.0.0
LIBMATIO 1.5 12_3
VTK 7.1


# WebAssembly compilation

## Requirements:

- Docker 
- Python 3 (for http.server)

## Setup 

Setup is modified from the [VTK Emscripten examples](https://github.com/Kitware/VTK/tree/master/Examples/Emscripten/Cxx)

#### Docker 
Install and configure Docker on your computer

#### Python3 
Install python3 (make sure it's available on bash)

#### Setup folders

- Create a folder on your computer where you will clone all repositories  
```
mkdir $HOME/mipx-wasm
```
<!--- ` -->
- Create a `build` directory and `src` directory
```
mkdir $HOME/mipx-wasm/build
mkdir $HOME/mipx-wasm/src
```
<!--- ` -->

## Compile VTK for Emscripten
```
mkdir $HOME/mipx-wasm/build/vtk-wasm
cd $HOME/mipx-wasm/

git clone https://gitlab.kitware.com/vtk/vtk.git src/vtk
cd src/vtk
git submodule update --init --recursive
```

Start Docker inside `mipx-wasm` directory 

```
cd $HOME/mipx-wasm/

docker run --rm --entrypoint /bin/bash \
  -v $HOME/mipx-wasm/build:/work/build \
  -v $HOME/mipx-wasm/src:/work/src \
  -it dockcross/web-wasm:20200416-a6b6635
```

Run from the started Docker instance

```
cd /work/build/vtk-wasm

cmake \
  -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} \
  -DBUILD_SHARED_LIBS:BOOL=OFF \
  -DCMAKE_BUILD_TYPE:STRING=Release \
  -DVTK_ENABLE_LOGGING:BOOL=OFF \
  -DVTK_ENABLE_WRAPPING:BOOL=OFF \
  -DVTK_LEGACY_REMOVE:BOOL=ON \
  -DVTK_OPENGL_USE_GLES:BOOL=ON \
  -DVTK_USE_SDL2:BOOL=ON \
  -DVTK_NO_PLATFORM_SOCKETS:BOOL=ON \
  -DVTK_MODULE_ENABLE_VTK_hdf5:STRING=NO \
  /work/src/vtk

cmake --build .
```
<!--- ` -->

## Clone and compile MIPX for Emscripten

On your host computer (not in the container): clone mipX to `src` folder 

```
cd $HOME/mipx-wasm/
git clone https://github.com/mipX/mipX.git src/mipx
```

Checkout the `wasm` branch

```
cd $HOME/mipx-wasm/src/mipx
git checkout wasm
```

Create mipx build dir

```
mkdir $HOME/mipx-wasm/build/mipx
```

Configure and compile inside the container

```
# start the container if neccessary
docker run --rm --entrypoint /bin/bash \
  -v $HOME/mipx-wasm/build:/work/build \
  -v $HOME/mipx-wasm/src:/work/src \
  -it dockcross/web-wasm:20200416-a6b6635

# Configure and compile
cd /work/build/mipx

cmake   -G Ninja  \
  -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}  \
  -DBUILD_SHARED_LIBS:BOOL=OFF  \
  -DCMAKE_BUILD_TYPE:STRING=Release  \
  -DVTK_ENABLE_LOGGING:BOOL=OFF  \
  -DMIPX_BUILD_LOADER_NIFTI:BOOL=OFF  \
  -DMIPX_BUILD_LOADER_MATLAB:BOOL=OFF  \
  -DMIPX_BUILD_LOADER_DICOM:BOOL=OFF  \
  -DVTK_DIR=/work/build/vtk-wasm  \
  -DVTK_OPENGL_USE_GLES:BOOL=ON  \
  -DVTK_USE_SDL2:BOOL=ON   \
  /work/src/mipx 

cmake --build .
```







