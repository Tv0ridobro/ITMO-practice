# WavAudioEditor
Simple program for working with wav audio files

Only PCM format supported
Try to avoid too large files

## Functionality
1. Cut audio file
2. Add reverberation effect
3. Add volume attenuation effect

## How to build
### Windows
For this project you need Cmake and some c++ compiler that cmake supports (for example MinGW or Visual Studio)
1. Clone this repository
2. Set some compiler as basic generator in cmake
3. In command prompt go to directory path and print
`cmake .`
and then
`cmake --build .`

### Linux
For this project you need Cmake
1. Clone this repository
2. In terminal print
`cmake <path to directory>`
and then
`make <path to directory/build>`

