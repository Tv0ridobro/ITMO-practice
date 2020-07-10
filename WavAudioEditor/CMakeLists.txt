cmake_minimum_required(VERSION 3.10)
project(WavAudioEditor)
set(CMAKE_CXX_STANDARD 11)

add_executable(WavAudioEditor main.cpp WavAudio.cpp WavAudio.h)