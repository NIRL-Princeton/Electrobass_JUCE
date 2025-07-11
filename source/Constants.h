/*
  ==============================================================================

    Constants.h
    Created: 9 Mar 2021 11:58:01am
    Author:  Matthew Wang

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "leaf-global.h"
#include <float.h>
#define VERSION_NUMBER_MAJOR 1
#define VERSION_NUMBER_MINOR 1
#define ESTEEL 1
#define EDITOR_WIDTH 900.0f
#define EDITOR_HEIGHT 700.0f
#define NUM_GENERIC_MACROS 8
#define NUM_MIDI_NOTES 128
#define NUM_UNIQUE_MACROS 15
#define NUM_MACROS (NUM_GENERIC_MACROS + NUM_UNIQUE_MACROS)
#define PEDAL_MACRO_ID (NUM_MACROS)

#define MAX_NUM_VOICES 12
#define NUM_CHANNELS (MAX_NUM_VOICES+1)

#define NUM_OSCS 3
#define INV_NUM_OSCS 0.333333f
#define NUM_FILT 2
#define NUM_ENVS 4
#define NUM_LFOS 4
#define NUM_FX 4
#define OVERSAMPLE 2
#define EXP_BUFFER_SIZE 2048
#define DECAY_EXP_BUFFER_SIZE 2048

#define MAX_NUM_UNIQUE_SKEWS 10

#define INV_127 0.007874015748031f
#define INV_4095 0.0002442002442f
#define INV_16383 0.000061038881768f
#define PI_DIV_2 1.570796326794897f


//==============================================================================
static const std::vector<std::string> cUniqueMacroNames = {
    "A",
    "B",
    "X",
    "Y",
    "Ped",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "K1",
    "K2",
    "K3",
    "K4",
    "K5"
};

//==============================================================================

typedef enum _OscParam
{
    OscPitch = 0,
    OscFine,
    OscFreq,
    OscShape,
    OscAmp,
    OscHarm,
    OscParamNil
} OscParam;
static const StringArray cOscParams = {
    "Pitch",
    "Fine",
    "Freq",
    "Shape",
    "Amp",
    "Harmonics"
};
static const std::vector<std::vector<float>> vOscInit = {
    { -24.0f, 24.0f, 0.0f, 0.0f }, //Pitch
    { -100.f, 100.f, 0.0f, 0.0f }, //Fine
    { -2000.f, 2000.f, 0.0f, 0.0f }, //Freq
    { 0.0f, 1.0f, 0.5f, 0.5f },  //Shape
    { 0.0f, 1.0f, 1.0f, 0.5f },  //Amp
    { -15, 15, 0.0f, 0.0f} //harmonics
};

typedef enum _OscShapeSet
{
    SawPulseOscShapeSet = 0,
    SineTriOscShapeSet,
    SawOscShapeSet,
    PulseOscShapeSet,
    SineOscShapeSet,
    TriOscShapeSet,
   // UserOscShapeSet,
    OscShapeSetNil
} OscShapeSet;
static const StringArray oscShapeSetNames = {
    "Saw-Square",
    "Sine-Tri",
    "Saw",
    "Pulse",
    "Sine",
    "Tri"
    //"Select file..."
};

//==============================================================================

typedef enum _LowFreqParam
{
    LowFreqRate = 0,
    LowFreqShape,
    LowFreqPhase,
    LowFreqParamNil
} LowFreqParam;
static const StringArray cLowFreqParams = {
    "Rate",
    "Shape",
    "Phase"
};
static const std::vector<std::vector<float>> vLowFreqInit = {
    { 0.0f, 1.f, 0.5f, 0.5f },  //Rate
    { 0.0f, 1.0f, 0.0f, 0.5f },  //Shape
    { 0.0f, 1.0f, 0.0f, 0.5f } // Phase Offset
};

typedef enum _LFOShapeSet
{
    SineTriLFOShapeSet = 0,
    SawPulseLFOShapeSet,
    SineLFOShapeSet,
    TriLFOShapeSet,
    SawLFOShapeSet,
    PulseLFOShapeSet,
    LFOShapeSetNil
} LFOShapeSet;
static const StringArray lfoShapeSetNames = {
    "Sine-Tri",
    "Saw-Square",
    "Sine",
    "Tri",
    "Saw",
    "Pulse"
};

//==============================================================================

typedef enum _NoiseParam
{
    NoiseTilt= 0,
    NoiseGain,
    NoiseFreq,
    NoiseAmp,
} NoiseParam;
static const StringArray cNoiseParams = {
    "Tilt",
    "PeakGain",
    "PeakFreq",
    "Amp"
};
static const std::vector<std::vector<float>> vNoiseInit = {
    { 0.0f, 1.f, 0.5f, 0.5f },   //tilt
    { 0.0f, 1.f, 0.5f, 0.5f },  //Gain
    { 0.0f, 1.f, 0.5f, 0.5f },   //Freq
    { 0.0f, 1.0f, 0.f, 0.5f }   //Amp
};

//==============================================================================

typedef enum _FilterParam
{
    FilterCutoff = 0,
    FilterGain,
    FilterResonance,
    FilterKeyFollow,
    FilterParamNil
} FilterParam;
static const StringArray cFilterParams = {
    "Cutoff",
    "Gain",
    "Resonance",
    "KeyFollow"
};
static const std::vector<std::vector<float>> vFilterInit = {
    { 0.0f, 127.f, 72.f, 63.5f },   //Cutoff
    { 0.0f, 1.f, 0.5f, 0.5f }, //Gain
    { 0.0f, 1.0f, 0.5f, 0.5f },   //Resonance
    { 0.0f, 1.f, 0.5f, 0.5f }  //KeyFollow
    
};

typedef enum _FilterType
{
    LowpassFilter = 0,
    HighpassFilter,
    BandpassFilter,
    DiodeLowpassFilter,
    VZPeakFilter,
    VZLowshelfFilter,
    VZHighshelfFilter,
    VZBandrejectFilter,
    LadderLowpassFilter,
    FilterTypeNil
} FilterType;

static const StringArray filterTypeNames = {
    "Lowpass",
    "Highpass",
    "Bandpass",
    "DiodeLowpass",
    "Peak",
    "Lowshelf",
    "Highshelf",
    "Notch",
    "LadderLowpass"
};

//==============================================================================

typedef enum _EnvelopeParam
{
    EnvelopeAttack = 0,
    EnvelopeDecay,
    EnvelopeSustain,
    EnvelopeRelease,
    EnvelopeLeak,
    EnvelopeParamNil
} EnvelopeParam;
static const StringArray cEnvelopeParams = {
    "Attack",
    "Decay",
    "Sustain",
    "Release",
    "Leak"
};
static const std::vector<std::vector<float>> vEnvelopeInit = {
    { 0.0f, 1.f, 0.01f, 0.5f },   //Attack
    { 0.0f, 1.f, 0.2f, 0.5f },  //Decay
    { 0.0f, 1.f, 0.5f, 0.5f },   //Sustain
    { 0.0f, 1.f, 0.05f, 0.5f },   //Release
    { 0.0f, 1.0f, 0.f, 0.5f },   //Leak
};

//==============================================================================

typedef enum _OutputParam
{
    OutputAmp,
    OutputTone,
    OutputParamNil
} OutputParam;
static const StringArray cOutputParams = {
    "Amp",
    "Tone"
};
static const std::vector<std::vector<float>> vOutputInit = {
    { 0.0f, 1.0f, 0.0f, 0.5f },   //Amp
    { 0.0f, 1.0, 1.0f, 0.5f },   //Tone
};


//==============================================================================

typedef enum _CopedentColumn
{
    CopedentString = 0,
    CopedentLKL,
    CopedentLKV,
    CopedentLKR,
    CopedentP1,
    CopedentP2,
    CopedentP3,
    CopedentP4,
    CopedentP5,
    CopedentRKL,
    CopedentRKR,
    CopedentColumnNil
} CopedentColumn;

static const std::vector<std::string> cCopedentColumnNames = {
    "Strings",
    "LKL",
    "LKV",
    "LKR",
    "P1",
    "P2",
    "P3",
    "P4",
    "P5",
    "RKL",
    "RKR"
};

static const std::vector<std::vector<float>> cCopedentArrayInit = {
    { 66.f, 63.f, 68.f, 64.f, 59.f, 56.f, 54.f, 52.f, 50.f, 47.f, 40.f, 35.f },
    
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
};



typedef enum _FXParam
{
    Param1 = 0,
    Param2,
    Param3,
    Param4,
    Param5,
    Mix,
    PostGain,
    ParamNil
} FXParam;
static const StringArray cFXParams = {
    "Param1",
    "Param2",
    "Param3",
    "Param4",
    "Param5",
    "Mix",
    "PostGain"
};
static const std::vector<std::vector<float>> vFXInit = {
    { 0.0f, 1.0f, 0.0f, 0.5f },   //
    { 0.0f, 1.0f, 0.0f, 0.5f },   //
    { 0.0f, 1.0f, 0.0f, 0.5f },   //
    { 0.0f, 1.0f, 0.0f, 0.5f },   //
    { 0.0f, 1.0f, 0.0f, 0.5f },   //
    { 0.0f, 1.0f, 1.0f, 0.5f },   // mix
    { 0.0f, 1.0f, 0.5f, 0.5f }   // postgain
};

typedef enum _FXType
{
    None = 0,
    Softclip,
    Hardclip,
    PolynomialShaper,
    ABSaturator,
    Tanh,
    Shaper2,
    Compressor,
    Chorus,
    Bitcrush,
    TiltFilter,
    Wavefolder,
    Delay,
    LpFilter,
    HpFilter,
    BpFilter,
    DLFilter,
    VZPFilter,
    VZLFilter,
    VZHFilter,
    VZBFilter,
    LLFilter,
    FXTypeNil
} FXType;

static const StringArray FXTypeNames = {
    "None",
    "Softclip",
    "Hardclip",
    "PolynomialShaper",
    "ABSaturator",
    "Tanh",
    "Shaper2",
    "Compressor",
    "Chorus",
    "Bitcrush",
    "TiltFilter",
    "Wavefolder",
    "Delay",
    "Lowpass",
    "Highpass",
    "Bandpass",
    "DiodeLowpass",
    "Peak",
    "Lowshelf",
    "Highshelf",
    "Notch",
    "LadderLowpass"
};

static const std::vector<StringArray> FXParamNames = {
    {"","","","","" },
    {"Drive","Offset", "","","" },
    {"Drive","Offset","Shape","","" },
    {"Drive","Offset","Shape","","" },
    {"Drive","Offset","","",""},
    {"Drive","Offset","","",""},
    {"Gain","Offset","Drive","",""},
    {"Threshold","Ratio","Makeup","Attack","Release"},
    {"Delay","Depth","Speed1","Speed2",""},
    {"Gain","Crush","Decimate","Round","Operation"},
    {"Tilt","PeakFreq","PeakQ","PeakGain",""},
    {"Drive","Offset","FB","FF","Depth"},
    {"Time","Feedback","LP","HP","Drive"},
    {"Cutoff", "", "Resonance" ,"",""},
    {"Cutoff", "", "Resonance" ,"",""},
    {"Cutoff", "", "Resonance" ,"",""},
    {"Cutoff", "", "Resonance" ,"",""},
    {"Cutoff", "Gain", "Resonance" ,"",""},
    {"Cutoff", "Gain", "Resonance" ,"",""},
    {"Cutoff", "Gain", "Resonance" ,"",""},
    {"Cutoff", "", "Resonance" ,"",""},
    {"Cutoff", "", "Resonance" ,"",""}
};

static const std::vector<std::vector<float>> FXParamDefaults = {
    {0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {0.2f, 0.5f, 1.0f, 1.0f, 0.0f},
    {0.2f, 0.5f, 1.0f, 1.0f, 0.0f},
    {0.2f, 0.5f, 1.0f, 1.0f, 0.0f},
    {0.2f, 0.5f, 1.0f, 1.0f, 0.0f},
    {0.3f, 0.5f, 1.0f, 1.0f, 0.0f},
    {0.2f, 0.5f, 1.0f, 1.0f, 0.0f},
    {0.5f, 0.4f, 0.4f, 0.05f, 0.25f},
    {0.65f, 0.65f, 0.1f, 0.25f, 0.0f},
    {0.5f, 0.5f, 0.2f, 0.8f, 0.25f},
    {0.5f, 0.5f, 0.5f, 0.5f, 0.0f},
    {0.4f, 0.5f, 0.6f, 0.4f, 0.5f},
    {0.5f, 0.5f, 0.9f, 0.1f, 0.1f},
    {0.75f, 0.5f, 0.5f, 0.0f, 0.0f},
    {0.35f, 0.5f, 0.5f, 0.0f, 0.0f},
    {0.5f, 0.5f, 0.5f, 0.0f, 0.0f},
    {0.65f, 0.5f, 0.5f, 0.0f, 0.0f},
    {0.5f, 0.75f, 0.5f, 0.0f, 0.0f},
    {0.45f, 0.65f, 0.4f, 0.0f, 0.0f},
    {0.5f, 0.65f, 0.5f, 0.0f, 0.0f},
    {0.5f, 0.5f, 0.5f, 0.0f, 0.0f},
    {0.6f, 0.5f, 0.55f, 0.0f, 0.0f}
};

//==============================================================================
static const std::vector<String> paramDestOrder
{
    "Master",
    "M1",
    "M2",
    "M3",
    "M4",
    "M5",
    "M6",
    "M7",
    "M8",
    "A",
    "B",
    "X",
    "Y",
    "Ped",
    "Transpose",
    "PitchBendRange",
    "Noise",
    "Noise Tilt",
    "Noise PeakGain",
    "Noise PeakFreq",
    "Noise Amp",
    "Noise FilterSend",
    "Osc1",
    "Osc1 Pitch",
    "Osc1 Fine",
    "Osc1 Freq",
    "Osc1 Shape",
    "Osc1 Amp",
    "Osc1 Harmonics",
    "Osc1 isHarmonic",
    "Osc1 isStepped",
    "Osc1 isSync",
    "Osc1 syncType",
    "Osc1 ShapeSet",
    "Osc1 FilterSend",
    "Osc2",
    "Osc2 Pitch",
    "Osc2 Fine",
    "Osc2 Freq",
    "Osc2 Shape",
    "Osc2 Amp",
    "Osc2 Harmonics",
    "Osc2 isHarmonic",
    "Osc2 isStepped",
    "Osc2 isSync",
    "Osc2 syncType",
    "Osc2 ShapeSet",
    "Osc2 FilterSend",
    "Osc3",
    "Osc3 Pitch",
    "Osc3 Fine",
    "Osc3 Freq",
    "Osc3 Shape",
    "Osc3 Amp",
    "Osc3 Harmonics",
    "Osc3 isHarmonic",
    "Osc3 isStepped",
    "Osc3 isSync",
    "Osc3 syncType",
    "Osc3 ShapeSet",
    "Osc3 FilterSend",
    "Effect1 FXType",
    "Effect1 Param1",
    "Effect1 Param2",
    "Effect1 Param3",
    "Effect1 Param4",
    "Effect1 Param5",
    "Effect1 Mix",
    "Effect1 PostGain",
    "Effect2 FXType",
    "Effect2 Param1",
    "Effect2 Param2",
    "Effect2 Param3",
    "Effect2 Param4",
    "Effect2 Param5",
    "Effect2 Mix",
    "Effect2 PostGain",
    "Effect3 FXType",
    "Effect3 Param1",
    "Effect3 Param2",
    "Effect3 Param3",
    "Effect3 Param4",
    "Effect3 Param5",
    "Effect3 Mix",
    "Effect3 PostGain",
    "Effect4 FXType",
    "Effect4 Param1",
    "Effect4 Param2",
    "Effect4 Param3",
    "Effect4 Param4",
    "Effect4 Param5",
    "Effect4 Mix",
    "Effect4 PostGain",
    "Filter1",
    "Filter1 Type",
    "Filter1 Cutoff",
    "Filter1 Gain",
    "Filter1 Resonance",
    "Filter1 KeyFollow",
    "Filter2",
    "Filter2 Type",
    "Filter2 Cutoff",
    "Filter2 Gain",
    "Filter2 Resonance",
    "Filter2 KeyFollow",
    "Filter Series-Parallel Mix",
    "Envelope1 Attack",
    "Envelope1 Decay",
    "Envelope1 Sustain",
    "Envelope1 Release",
    "Envelope1 Leak",
    "Envelope1 Velocity",
    "Envelope2 Attack",
    "Envelope2 Decay",
    "Envelope2 Sustain",
    "Envelope2 Release",
    "Envelope2 Leak",
    "Envelope2 Velocity",
    "Envelope3 Attack",
    "Envelope3 Decay",
    "Envelope3 Sustain",
    "Envelope3 Release",
    "Envelope3 Leak",
    "Envelope3 Velocity",
    "Envelope4 Attack",
    "Envelope4 Decay",
    "Envelope4 Sustain",
    "Envelope4 Release",
    "Envelope4 Leak",
    "Envelope4 Velocity",
    "LFO1 Rate",
    "LFO1 Shape",
    "LFO1 Phase",
    "LFO1 ShapeSet",
    "LFO1 Sync",
    "LFO2 Rate",
    "LFO2 Shape",
    "LFO2 Phase",
    "LFO2 ShapeSet",
    "LFO2 Sync",
    "LFO3 Rate",
    "LFO3 Shape",
    "LFO3 Phase",
    "LFO3 ShapeSet",
    "LFO3 Sync",
    "LFO4 Rate",
    "LFO4 Shape",
    "LFO4 Phase",
    "LFO4 ShapeSet",
    "LFO4 Sync",
    "Output Amp",
    "Output Tone",
    "FX Order",
    "PedalControlsMaster"
};
#define OSC_SOURCE_OFFSET 0
#define NOISE_SOURCE_OFFSET 3
#define MACRO_SOURCE_OFFSET 4
#define CTRL_SOURCE_OFFSET 12
#define MIDI_KEY_SOURCE_OFFSET 17
#define VELOCITY_SOURCE_OFFSET 18
#define RANDOM_SOURCE_OFFSET 19
#define ENV_SOURCE_OFFSET 20
#define LFO_SOURCE_OFFSET 24
#define PEDAL_SOURCE_OFFSET 28
static const std::vector<String> paramSourceOrder
{
    "Osc1",//0
    "Osc2",//1
    "Osc3",//2
    "Noise",//3
    "M1",//4
    "M2",//5
    "M3",//6
    "M4",//7
    "M5",//8
    "M6",//9
    "M7",//10
    "M8",//11
    "A",//12
    "B",//13
    "X",//14
    "Y",//15
    "Ped",//16
    "MIDI Key In",//17
    "Velocity In",//18
    "Random on Attack",//19
    "Envelope1",//20
    "Envelope2",//21
    "Envelope3",//22
    "Envelope4",//23
    "LFO1",//24
    "LFO2",//25
    "LFO3",//26
    "LFO4",//27
    "F1",//0
    "F2",//0//0
    "F3",//0
    "F4",//0
    "F5",//0
    "K1",//0
    "K2",//0
    "K3",//0
    "K4",//0
    "K5"//0
};
//enum ParamNames
//{
//    Master,
//    M1,
//    M2,
//    M3,
//    M4,
//    M5,
//    M6,
//    M7,
//    M8,
//    M9,
//    M10,
//    M11,
//    M12,
//    M13,
//    M14,
//    M15,
//    M16,
//    A,
//    B,
//    X,
//    Y,
//    Ped,
//    Transpose,
//    PitchBend0,
//    PitchBend1,
//    PitchBend2,
//    PitchBend3,
//    PitchBend4,
//    PitchBend5,
//    PitchBend6,
//    PitchBend7,
//    PitchBend8,
//    PitchBendRangeUp,
//    PitchBendRangeDown,
//    Noise,
//    NoiseTilt,
//    NoisePeakGain,
//    NoisePeakFreq,
//    NoiseAmp,
//    NoiseFilterSend,
//    Osc1,
//    Osc1Pitch,
//    Osc1Fine,
//    Osc1Freq,
//    Osc1Shape,
//    Osc1Amp,
//    Osc1isHarmonic,
//    Osc1isStepped,
//    Osc1ShapeSet,
//    Osc1FilterSend,
//    Osc2,
//    Osc2Pitch,
//    Osc2Fine,
//    Osc2Freq,
//    Osc2Shape,
//    Osc2Amp,
//    Osc2isHarmonic,
//    Osc2isStepped,
//    Osc2ShapeSet,
//    Osc2FilterSend,
//    Osc3,
//    Osc3Pitch,
//    Osc3Fine,
//    Osc3Freq,
//    Osc3Shape,
//    Osc3Amp,
//    Osc3isHarmonic,
//    Osc3isStepped,
//    Osc3ShapeSet,
//    Osc3FilterSend,
//    Effect1FXType,
//    Effect1Param1,
//    Effect1Param2,
//    Effect1Param3,
//    Effect1Param4,
//    Effect1Param5,
//    Effect1Mix,
//    Effect2FXType,
//    Effect2Param1,
//    Effect2Param2,
//    Effect2Param3,
//    Effect2Param4,
//    Effect2Param5,
//    Effect2Mix,
//    Effect3FXType,
//    Effect3Param1,
//    Effect3Param2,
//    Effect3Param3,
//    Effect3Param4,
//    Effect3Param5,
//    Effect3Mix,
//    Effect4FXType,
//    Effect4Param1,
//    Effect4Param2,
//    Effect4Param3,
//    Effect4Param4,
//    Effect4Param5,
//    Effect4Mix,
//    Filter1,
//    Filter1Type,
//    Filter1Cutoff,
//    Filter1Gain,
//    Filter1Resonance,
//    Filter1KeyFollow,
//    Filter2,
//    Filter2Type,
//    Filter2Cutoff,
//    Filter2Gain,
//    Filter2Resonance,
//    Filter2KeyFollow,
//    FilterSeriesParallelMix,
//    Envelope1Attack,
//    Envelope1Decay,
//    Envelope1Sustain,
//    Envelope1Release,
//    Envelope1Leak,
//    Envelope1Velocity,
//    Envelope2Attack,
//    Envelope2Decay,
//    Envelope2Sustain,
//    Envelope2Release,
//    Envelope2Leak,
//    Envelope2Velocity,
//    Envelope3Attack,
//    Envelope3Decay,
//    Envelope3Sustain,
//    Envelope3Release,
//    Envelope3Leak,
//    Envelope3Velocity,
//    Envelope4Attack,
//    Envelope4Decay,
//    Envelope4Sustain,
//    Envelope4Release,
//    Envelope4Leak,
//    Envelope4Velocity,
//    LFO1Rate,
//    LFO1Shape,
//    LFO1Phase,
//    LFO1ShapeSet,
//    LFO1Sync,
//    LFO2Rate,
//    LFO2Shape,
//    LFO2Phase,
//    LFO2ShapeSet,
//    LFO2Sync,
//    LFO3Rate,
//    LFO3Shape,
//    LFO3Phase,
//    LFO3ShapeSet,
//    LFO3Sync,
//    LFO4Rate,
//    LFO4Shape,
//    LFO4Phase,
//    LFO4ShapeSet,
//    LFO4Sync,
//    OutputAmp,
//};
