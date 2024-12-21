#pragma once

#include <Arduino.h>
#include "preset.h"

struct LoopView {
  uint8_t index;
  bool isActive;
  uint8_t order;
};

struct MidiMessageView {
  uint8_t type;
  uint8_t channel;
  uint8_t byte1;
  uint8_t byte2;
  bool hasDataByte2;
};

struct PresetView {
  LoopView loops[c_maxLoops];
  uint8_t loopsCount;

  MidiMessageView midiMessages[c_maxMidiMessages];
  uint8_t midiMessagesCount;
};
