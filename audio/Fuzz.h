#pragma once
#include <vector>

class Fuzz {
public:
    Fuzz(float sample_rate, int num_channels,
         float default_fuzz, float default_tone, float default_level,
         float default_dry_wet, int default_fuzz_type);
    ~Fuzz();

    void process(float *const *buffer, int numSamples, int numChannels,
                 float fuzz, float tone, float level, float dry_wet, int fuzz_type);
    void setState(float fuzz, float tone, float level, float dry_wet, int fuzz_type);

private:
    float waveshape(float x, float fuzz_norm, int fuzz_type) const;
    float toTonePole(float tone_normalized) const;

    float smooth_fuzz, smooth_tone, smooth_level, smooth_dry_wet;
    const float smooth_pole;
    const float sample_rate;
    const float dc_pole;

    std::vector<float> dc_x, dc_y, tone_state;
};
