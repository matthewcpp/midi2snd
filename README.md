# midi2snd
A small utility to synthesize midi files to sound files.  This utility makes use of [TinySoundFont](https://github.com/schellingb/TinySoundFont) and [libsndfile](https://github.com/libsndfile/libsndfile).

### Building
Ensure that you have cloned this repo using the `--recursive` option or run `git submodule init` to ensure that the required submodules have been populated.

Then configure and build the project with cmake.  This should automatically bootstrap [vcpkg](https://vcpkg.io) and install dependencies.
Note that on linux and macOS you may need to install the `pkg-config` via brew or your package manager before running these commands.

```shell
mkdir build && cd build
cmake ..
cmake --build .
```

### Usage

Provide a path to the source midi file to convert, sf2 sound font to use for synthesis, and the target output file name.

```shell
./midi2snd /path/to/source.midi /path/to/soundfont.sf2 /path/to/result.ogg
```

### Docker

For more info on running in docker you can visit [matthewcpp/midi2snd](https://hub.docker.com/repository/docker/matthewcpp/midi2snd) on dockerhub or checkout the [source](https://github.com/matthewcpp/Dockerfiles/tree/main/midi2snd).