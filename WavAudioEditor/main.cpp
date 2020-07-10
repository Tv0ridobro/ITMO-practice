#include <iostream>
#include <cstdlib>
#include <string>
#include "WavAudio.h"

int main(int argc, char **argv) {
     if (std::string(argv[2]) == "-c" && argc == 5) {
         WavAudio audio = WavAudio(argv[1]);
         audio.cut(std::strtoul(argv[3], nullptr, 0), std::strtoul(argv[4], nullptr, 0));
     } else if (std::string(argv[2]) == "-i" && argc == 3) {
         WavAudio audio = WavAudio(argv[1]);
         audio.printInfo();
     } else if (std::string(argv[2]) == "-r" && argc == 5) {
         WavAudio audio = WavAudio(argv[1]);
         audio.addReverberation(std::strtoul(argv[3], nullptr, 0), std::stod(argv[4], nullptr));
     } else if (std::string(argv[2]) == "-v" && argc == 6) {
         WavAudio audio = WavAudio(argv[1]);
         audio.addVolumeAttenuation(std::strtoul(argv[3], nullptr, 0), std::strtoul(argv[4], nullptr, 0),
                                    std::stod(argv[4], nullptr));
     } else {
         std::cout << "Print info: -i\n"
                   << "Add reverberation: -r <delay in milliseconds> <decay in percent from 0 to 1>\n"
                   << "Cut: -c <start point in milliseconds> <end point in milliseconds>\n"
                   << "Add volume attenuation: -v <start point in milliseconds> <end point in milliseconds> <expected percent from 0 to 1>\n";
     }
}