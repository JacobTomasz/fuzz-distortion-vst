#include "Fuzz.h"
#include "../Util/Util.h"
#include <algorithm>
#include <cmath>

static constexpr float PI_F           = 3.14159265358979323846f;
static constexpr float TONE_MIN_HZ    = 400.0f;
static constexpr float TONE_MAX_HZ    = 6000.0f;
static constexpr float PARAM_SMOOTH_TAU = 0.05f;
static constexpr float DC_BLOCK_HZ   = 20.0f;

Fuzz::Fuzz(float sample_rate, int num_channels,
           float default_fuzz, float default_tone, float default_level,
           float default_dry_wet, int default_fuzz_type)
    : smooth_pole(nthn_utils::tau2pole(PARAM_SMOOTH_TAU, sample_rate)),
      sample_rate(sample_rate),
      dc_pole(std::exp(-2.0f * PI_F * DC_BLOCK_HZ / sample_rate)),
      dc_x(num_channels, 0.0f),
      dc_y(num_channels, 0.0f),
      tone_state(num_channels, 0.0f) {
    setState(default_fuzz, default_tone, default_level, default_dry_wet, default_fuzz_type);
}

Fuzz::~Fuzz() {}

float Fuzz::toTonePole(float tone_normalized) const {
    float fc = TONE_MIN_HZ * std::pow(TONE_MAX_HZ / TONE_MIN_HZ, tone_normalized);
    return std::exp(-2.0f * PI_F * fc / sample_rate);
}

// Each type uses its own gain curve so their shapes stay perceptually distinct.
// smooth_fuzz holds the raw normalized value (0–1), not the expanded gain.
float Fuzz::waveshape(float x, float fuzz_norm, int fuzz_type) const {
    switch (fuzz_type) {
        case 0: { // Soft — tanh, 1–2000x gain
            float g = std::pow(2000.0f, fuzz_norm);
            return std::tanh(x * g);
        }
        case 1: { // Sgn — sgn(x)*(1-e^{-|x|*g}), 1–50x gain keeps curve shape audible
            float g = std::pow(50.0f, fuzz_norm);
            return std::copysign(1.0f - std::exp(-std::abs(x) * g), x);
        }
        case 2: { // Hard — hard clamp, 1–2000x gain
            float g = std::pow(2000.0f, fuzz_norm);
            return std::clamp(x * g, -1.0f, 1.0f);
        }
        case 3: { // Atan — arctangent, 1–2000x gain
            float g = std::pow(2000.0f, fuzz_norm);
            return (2.0f / PI_F) * std::atan(x * g);
        }
        case 4: { // Fold — foldback at ±1, 1–2000x gain
            float g = std::pow(2000.0f, fuzz_norm);
            float d = x * g;
            float sign = (d >= 0.0f) ? 1.0f : -1.0f;
            float t = std::fmod(std::abs(d), 4.0f);
            float y;
            if      (t < 1.0f) y = t;
            else if (t < 3.0f) y = 2.0f - t;
            else               y = t - 4.0f;
            return sign * y;
        }
        case 5: { // Asym — softer negative half, 1–2000x gain
            float g = std::pow(2000.0f, fuzz_norm);
            return (x >= 0.0f) ? std::tanh(x * g)
                               : std::tanh(x * g * 0.65f);
        }
        default: {
            float g = std::pow(2000.0f, fuzz_norm);
            return std::tanh(x * g);
        }
    }
}

void Fuzz::process(float *const *buffer, int numSamples, int numChannels,
                   float fuzz, float tone, float level, float dry_wet, int fuzz_type) {
    float local_fuzz   = smooth_fuzz;   // normalized 0–1
    float local_tone   = smooth_tone;
    float local_level  = smooth_level;
    float local_drywet = smooth_dry_wet;

    // fuzz target is the raw normalized value; gain is computed per-sample inside waveshape
    const float tone_target   = toTonePole(tone);
    const float level_target  = level;
    const float drywet_target = dry_wet;

    for (int i = 0; i < numSamples; ++i) {
        local_fuzz   = nthn_utils::lerp(fuzz,         local_fuzz,   smooth_pole);
        local_tone   = nthn_utils::lerp(tone_target,  local_tone,   smooth_pole);
        local_level  = nthn_utils::lerp(level_target, local_level,  smooth_pole);
        local_drywet = nthn_utils::lerp(drywet_target,local_drywet, smooth_pole);

        for (int c = 0; c < numChannels; ++c) {
            const float dry = buffer[c][i];

            // DC block: first-order HPF removes offset before clipping
            float dc_blocked = dry - dc_x[c] + dc_pole * dc_y[c];
            dc_x[c] = dry;
            dc_y[c] = dc_blocked;

            float shaped = waveshape(dc_blocked, local_fuzz, fuzz_type);

            // Tone: one-pole LPF shapes harmonic content
            tone_state[c] = nthn_utils::lerp(shaped, tone_state[c], local_tone);

            const float wet = tone_state[c] * local_level;
            buffer[c][i] = dry * (1.0f - local_drywet) + wet * local_drywet;
        }
    }

    smooth_fuzz    = local_fuzz;
    smooth_tone    = local_tone;
    smooth_level   = local_level;
    smooth_dry_wet = local_drywet;
}

void Fuzz::setState(float fuzz, float tone, float level, float dry_wet, int /*fuzz_type*/) {
    smooth_fuzz    = fuzz;  // store normalized, gain computed in waveshape
    smooth_tone    = toTonePole(tone);
    smooth_level   = level;
    smooth_dry_wet = dry_wet;
}
