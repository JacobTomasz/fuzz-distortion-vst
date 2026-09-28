#pragma once
#include <juce_core/juce_core.h>
enum PARAM {
	FUZZ,
	TONE,
	LEVEL,
	DRY_WET,
	FUZZ_TYPE,
	TOTAL_NUMBER_PARAMETERS
};
static const std::array<juce::Identifier, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_IDS{
	"FUZZ",
	"TONE",
	"LEVEL",
	"DRY_WET",
	"FUZZ_TYPE",
};
static const std::array<juce::String, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_NAMES{
	"FUZZ",
	"TONE",
	"LEVEL",
	"DRY_WET",
	"FUZZ_TYPE",
};
static const std::array<juce::NormalisableRange<float>, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_RANGES {
	juce::NormalisableRange<float>(0.0f, 100.0f, 0.0f, 1.0f),
	juce::NormalisableRange<float>(0.0f, 100.0f, 0.0f, 1.0f),
	juce::NormalisableRange<float>(0.0f, 100.0f, 0.0f, 1.0f),
	juce::NormalisableRange<float>(0.0f, 100.0f, 0.0f, 1.0f),
	juce::NormalisableRange<float>(0.0f, 5.0f, 1.0f, 1.0f),
};
static const std::array<float, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_DEFAULTS {
	50.0f,
	50.0f,
	75.0f,
	100.0f,
	0.0f,
};
static const std::array<bool, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_AUTOMATABLE {
	true,
	true,
	true,
	true,
	true,
};
static const std::array<juce::String, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_NICKNAMES{
	"Fuzz",
	"Tone",
	"Level",
	"Mix",
	"Type",
};
static const std::array<juce::String, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_SUFFIXES {
	"%",
	"%",
	"%",
	"%",
	"",
};
static const std::array<juce::String, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_TOOLTIPS {
	"Amount of fuzz distortion",
	"Tonal brightness",
	"Output volume",
	"Dry/wet blend",
	"Fuzz algorithm",
};
static const std::array<std::vector<juce::String>, PARAM::TOTAL_NUMBER_PARAMETERS> PARAMETER_TO_STRING_ARRS {
	std::vector<juce::String>{},
	std::vector<juce::String>{},
	std::vector<juce::String>{},
	std::vector<juce::String>{},
	std::vector<juce::String>{"Soft", "Sgn", "Hard", "Atan", "Fold", "Asym", },
};
