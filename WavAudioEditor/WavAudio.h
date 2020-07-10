#ifndef WAVAUDIOEDITOR_WAVAUDIO_H
#define WAVAUDIOEDITOR_WAVAUDIO_H

#include <string>
#include <cstdint>
#include <vector>

class WavAudio {
private:
    std::vector<double> samples;
    std::string fileName;
    uint32_t bytesPerSample;
    uint32_t bitsPerSample;
    uint32_t numberOfChannels;
    uint32_t sampleRate;
    uint32_t bitrate;
    uint32_t dataSize;
    char headerInfo[44]{};

    void saveToFile(const std::string &filename);

public:
    explicit WavAudio(const std::string &fileName);

    uint32_t getDuration() const;

    uint32_t getNumberOfChannels() const;

    uint32_t getSampleRate() const;

    uint32_t getBitsPerSample() const;

    uint32_t getBitrate() const;

    void printInfo() const;

    void cut(uint32_t start, uint32_t end);

    void addReverberation(uint32_t delay, double decay);

    void addVolumeAttenuation(uint32_t start, uint32_t end, double percent);

};

#endif
