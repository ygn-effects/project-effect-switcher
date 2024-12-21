#include "footswitch.h"

FootSwitchMode FootSwitchConfig::getMode() const {
  return m_mode;
}

void FootSwitchConfig::setMode(FootSwitchMode t_mode) {
  m_mode = t_mode;
}

uint8_t FootSwitchConfig::getLatching() const {
  return m_latching;
}

void FootSwitchConfig::setLatching(uint8_t t_latching) {
  m_latching = t_latching;
}

uint8_t FootSwitchConfig::getLoopIndex() const {
  return m_loopIndex;
}

void FootSwitchConfig::setLoopIndex(uint8_t t_loopIndex) {
  m_loopIndex = t_loopIndex;
}

uint8_t FootSwitchConfig::getTargetBank() const {
  return m_targetBank;
}

void FootSwitchConfig::setTargetBank(uint8_t t_targetBank) {
  m_targetBank = t_targetBank;
}

uint8_t FootSwitchConfig::getTargetPreset() const {
  return m_targetPreset;
}

void FootSwitchConfig::setTargetPreset(uint8_t t_targetPreset) {
  m_targetPreset = t_targetPreset;
}

uint8_t FootSwitchConfig::getMidiMessageType(uint8_t t_message) const {
  return m_midiMessages[t_message].getType();
}

uint8_t FootSwitchConfig::getMidiMessageChannel(uint8_t t_message) const {
  return m_midiMessages[t_message].getChannel();
}

uint8_t FootSwitchConfig::getMidiMessageDataByte1(uint8_t t_message) const {
  return m_midiMessages[t_message].getDataByte1();
}

uint8_t FootSwitchConfig::getMidiMessageDataByte2(uint8_t t_message) const {
  return m_midiMessages[t_message].getDataByte2();
}

void FootSwitchConfig::setMidiMessage(uint8_t t_message, uint8_t t_type, uint8_t t_channel, uint8_t t_byte1, uint8_t t_byte2) {
  if (t_message < 2) {
    m_midiMessages[t_message].setType(t_type);
    m_midiMessages[t_message].setChannel(t_channel);
    m_midiMessages[t_message].setDataByte1(t_byte1);
    m_midiMessages[t_message].setDataByte2(t_byte2);
  }
}

void FootSwitchConfig::setMidiMessage(uint8_t t_message, uint8_t t_status, uint8_t t_byte1, uint8_t t_byte2) {
  if (t_message < 2) {
    m_midiMessages[t_message].setStatusByte(t_status);
    m_midiMessages[t_message].setDataByte1(t_byte1);
    m_midiMessages[t_message].setDataByte2(t_byte2);
  }
}