#include <iostream>
#include <fstream>
#include <cmath>
#include "WavAudio.h"

WavAudio::WavAudio(const std::string &fileName) : fileName(fileName) {
    std::ifstream stream = std::ifstream(fileName, std::ios_base::binary | std::ios_base::in);
    stream.read(headerInfo, 44);
    numberOfChannels = *reinterpret_cast<uint16_t *>(&headerInfo[22]);
    bitsPerSample = *reinterpret_cast<uint16_t *>(&headerInfo[34]);
    sampleRate = *reinterpret_cast<uint32_t *>(&headerInfo[24]);
    dataSize = *reinterpret_cast<uint32_t *>(&headerInfo[40]);
    bitrate = sampleRate * bitsPerSample * numberOfChannels;
    bytesPerSample = bitsPerSample / 8;
    while (stream.good()) {
        char buffer[4]{};
        stream.read(buffer, bytesPerSample);
        switch (bytesPerSample) {
            case 1:
                samples.push_back(*reinterpret_cast<int8_t *>(&buffer[0]));
                break;
            case 2:
                samples.push_back(*reinterpret_cast<int16_t *>(&buffer[0]));
                break;
            case 4:
                samples.push_back(*reinterpret_cast<int32_t *>(&buffer[0]));
                break;
        }
    }
}

uint32_t WavAudio::getSampleRate() const {
    return sampleRate;
}

uint32_t WavAudio::getDuration() const {
    return dataSize / (bitrate / 8);
}

uint32_t WavAudio::getBitrate() const {
    return bitrate;
}

uint32_t WavAudio::getBitsPerSample() const {
    return bitsPerSample;
}

uint32_t WavAudio::getNumberOfChannels() const {
    return numberOfChannels;
}

void WavAudio::printInfo() const {
    std::cout << "Audio info: \n"
              << "Number of channels: " << getNumberOfChannels() << "\n"
              << "Sample rate: " << getSampleRate() << "\n"
              << "Bits per sample: " << getBitsPerSample() << "\n"
              << "Bitrate: " << getBitrate() << "\n"
              << "Duration: ";
    uint32_t duration = getDuration();
    if (duration / 3600) {
        std::cout << duration / 3600 << " h ";
    }
    std::cout << (duration % 3600) / 60 << " m " <<
              ((duration % 60)) << " s\n";
}

void WavAudio::cut(uint32_t start, uint32_t end) {
    uint32_t startPosition = (bitrate / bitsPerSample * start / 1000);
    uint32_t endPosition = (bitrate / bitsPerSample * end / 1000);
    samples.erase(samples.begin(), std::next(samples.begin(), startPosition));
    samples.resize(endPosition - startPosition);
    dataSize = samples.size() * bytesPerSample;
    *reinterpret_cast<uint32_t *>(headerInfo + 40) = dataSize;
    *reinterpret_cast<uint32_t *>(headerInfo + 4) = dataSize + 36;
    if (fileName.size() >= 4 && fileName.substr(fileName.size() - 4, 4) == ".wav") {
        saveToFile(fileName.substr(0, fileName.size() - 4) + ".cut.wav");
    } else {
        saveToFile(fileName + ".cat.wav");
    }
}

void WavAudio::addReverberation(uint32_t delay, double decay) {
    delay = bitrate / bitsPerSample * delay / 1000;
    samples.resize(samples.size() + delay, 0);
    for (uint32_t i = 0; i < dataSize / bytesPerSample; ++i) {
        samples[i + delay] += decay * samples[i];
    }
    dataSize += delay * bytesPerSample;
    *reinterpret_cast<uint32_t *>(headerInfo + 40) = dataSize;
    *reinterpret_cast<uint32_t *>(headerInfo + 4) = dataSize + 36;
    if (fileName.size() >= 4 && fileName.substr(fileName.size() - 4, 4) == ".wav") {
        saveToFile(fileName.substr(0, fileName.size() - 4) + ".reverberation.wav");
    } else {
        saveToFile(fileName + ".reverberation.wav");
    }
}

void WavAudio::addVolumeAttenuation(uint32_t start, uint32_t end, double percent) {
    uint32_t startPosition = bitrate / bitsPerSample * start / 1000;
    uint32_t endPosition = bitrate / bitsPerSample * end / 1000;
    double diff = log(2);
    double force;
    double k = 1.0;
    for (int i = startPosition; i < endPosition; i++) {
        force = exp(
                static_cast<double > (i - startPosition) / static_cast<double > (endPosition - startPosition) * diff) -
                1.0;
        samples[i] *= percent * std::abs(k - force);
    }
    if (fileName.size() >= 4 && fileName.substr(fileName.size() - 4, 4) == ".wav") {
        saveToFile(fileName.substr(0, fileName.size() - 4) + ".volume.wav");
    } else {
        saveToFile(fileName + ".volume.wav");
    }
}

void WavAudio::saveToFile(const std::string &name) {
    std::ofstream stream = std::ofstream(name, std::ios_base::binary | std::ios_base::out);
    stream.write(headerInfo, 44);
    for (auto &i : samples) {
        switch (bytesPerSample) {
            case 1: {
                auto num = static_cast<int8_t >(std::round(i));
                stream.write(reinterpret_cast<char *>(&num), bytesPerSample);
            }
                break;
            case 2: {
                auto num = static_cast<int16_t >(std::round(i));
                stream.write(reinterpret_cast<char *>(&num), bytesPerSample);
            }
                break;
            case 4: {
                auto num = static_cast<int32_t >(std::round(i));
                stream.write(reinterpret_cast<char *>(&num), bytesPerSample);
            }
                break;
        }
    }
}