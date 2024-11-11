#pragma once

#include <Arduino.h>
#include "midi_message.h"

enum class FootSwitchMode : uint8_t {
  kNone = 0,
  kToggleLoop = 1,
  kSendMidiMessage = 2,
  kBankSelect = 3,
  kPresetSelect = 4,
  kMute = 5
};

class FootSwitchConfig {
  private:
    FootSwitchMode m_mode;
    uint8_t m_latching = 0;
    uint8_t m_loopIndex = 0;
    uint8_t m_targetBank = 0;
    uint8_t m_targetPreset = 0;
    MidiMessage m_midiMessages[2];

  public:
    FootSwitchConfig(FootSwitchMode t_mode) :
      m_mode(t_mode) { };

    FootSwitchMode getMode() const;

    void setMode(FootSwitchMode t_mode);

    uint8_t getLatching() const;

    void setLatching(uint8_t t_latching);

    uint8_t getLoopIndex() const;

    void setLoopIndex(uint8_t t_loopIndex);

    uint8_t getTargetBank() const;

    void setTargetBank(uint8_t t_targetBank);

    uint8_t getTargetPreset() const;

    void setTargetPreset(uint8_t t_targetPreset);

    uint8_t getMidiMessageType(uint8_t t_message) const;
    uint8_t getMidiMessageChannel(uint8_t t_message) const;
    uint8_t getMidiMessageDataByte1(uint8_t t_message) const;
    uint8_t getMidiMessageDataByte2(uint8_t t_message) const;

    void setMidiMessage(uint8_t t_message, uint8_t t_type, uint8_t t_channel, uint8_t t_byte1, uint8_t t_byte2);
    void setMidiMessage(uint8_t t_message, uint8_t t_status, uint8_t t_byte1, uint8_t t_byte2);
};
