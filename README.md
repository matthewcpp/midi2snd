# midi2snd
A small utility to synthesize midi files to sound files

### Building
```shell
# setup vcpkg
lib/vcpkg/bootstrap-vcpkg.sh

# install and build libsndfile dependency
lib/vcpkg/vcpkg install libsndfile

# configure cmake
mkdir build && cd build
cmake ..
```