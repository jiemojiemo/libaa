[![Build Status](https://github.com/jiemojiemo/libaa/workflows/build/badge.svg)](https://github.com/jiemojiemo/libaa/actions)
[![codecov](https://codecov.io/gh/jiemojiemo/libaa/branch/master/graph/badge.svg?token=Y3ZGGNX6I1)](https://codecov.io/gh/jiemojiemo/libaa)

# LibAA
LibAA is a audio library.

## Audio Effect
+ Biquad Filter
+ Chorus
+ Compressor
+ Channel Vocoder
+ Delay
+ Distortion
+ Limiter
+ Pitch Shifter
+ Reverb
+ Flanger
+ Tremolo
+ Vibrato

## DSP
+ DTW
+ MFCC
+ YIN
+ FFT
+ STFT

## Audio Codec
+ Decoder
  + AAC
  + MP3
  + WAV
+ Encoder
  + MP3
  + WAV

## MIR
+ Ebur128 Loudness
+ SuperFlux (Onset detection)

## Audio Graph
+ Connect Nodes
+ Serialization/Deserialization
+ Realtime Parameter Control
  
# How to build libaa
```bash
git clone https://github.com/jiemojiemo/libaa
cd libaa
git submodule update --init --recursive
cmake -S . -B build
cmake --build build
```
