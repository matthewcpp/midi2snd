# midi2snd
A small utility to synthesize midi files to sound files

### Building
Ensure that you have cloned this repo using the `--recursive` option or run `git submodule init` to ensure that the required submodules have been populated.

Begin by bootstrapping vcpkg and installig dependencies:
```shell
lib/vcpkg/bootstrap-vcpkg.sh
```
or on windows:

```shell
.\lib\vcpkg\bootstrap-vcpkg.bat
```
Then configure and build the project with cmake:

```shell
mkdir build && cd build
cmake ..
cmake --build .
```