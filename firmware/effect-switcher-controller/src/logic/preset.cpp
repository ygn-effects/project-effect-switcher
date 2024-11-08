#include "preset.h"

uint8_t Preset::getBank() const {
  return m_bank;
}

void Preset::setBank(uint8_t t_bank) {
  m_bank = t_bank;

  LOG_DEBUG("Bank set to %d", t_bank);
}

uint8_t Preset::getPreset() const {
  return m_preset;
}

void Preset::setPreset(uint8_t t_preset) {
  m_preset = t_preset;

  LOG_DEBUG("Preset set to %d", t_preset);
}

uint8_t Preset::getLoopsCount() const {
  return m_loopsCount;
}

void Preset::setLoopsCount(uint8_t t_count) {
  m_loopsCount = t_count;
}

uint8_t Preset::getLoopState(uint8_t t_loop) const {
  return m_loops[t_loop].getLoopState();
}

void Preset::setLoopState(uint8_t t_loop, uint8_t t_state) {
  m_loops[t_loop].setLoopState(t_state);
}

uint8_t Preset::getLoopOrder(uint8_t t_loop) const {
  return m_loops[t_loop].getLoopOrder();
}

void Preset::setLoopOrder(uint8_t t_loop, uint8_t t_order) {
  m_loops[t_loop].setLoopOrder(t_order);
}

uint8_t Preset::getLoopSend(uint8_t t_loop) const {
  return m_loops[t_loop].getLoopSend();
}

void Preset::setLoopSend(uint8_t t_loop, uint8_t t_send) {
  m_loops[t_loop].setLoopSend(t_send);
}

uint8_t Preset::getLoopReturn(uint8_t t_loop) const {
  return m_loops[t_loop].getLoopReturn();
}

void Preset::setLoopReturn(uint8_t t_loop, uint8_t t_return) {
  m_loops[t_loop].setLoopReturn(t_return);
}

void Preset::toggleLoopState(uint8_t t_loop) {
  m_loops[t_loop].toggleLoopState();
}

uint8_t Preset::getLoopIndexByOrder(uint8_t t_order) {
  for (uint8_t i = 0; i < m_loopsCount; i++) {
    if (m_loops[i].getLoopOrder() == t_order) {
      return i;
    }
  }

  return m_loopsCount + 1; // Invalid return in case the loop isn't found
}

void Preset::swapPresetLoopsOrder(uint8_t t_loop1, uint8_t t_loop2) {
  uint8_t loop1Order = m_loops[t_loop1].getLoopOrder();
  uint8_t loop2Order = m_loops[t_loop2].getLoopOrder();

  m_loops[t_loop1].setLoopOrder(loop2Order);
  m_loops[t_loop2].setLoopOrder(loop1Order);
}

void Preset::setPresetLoopSendReturn(uint8_t t_loop, uint8_t t_send, uint8_t t_return) {
  m_loops[t_loop].setLoopSend(t_send);
  m_loops[t_loop].setLoopReturn(t_return);
}

uint8_t Preset::getPresetLoopSend(uint8_t t_loop) {
  return m_loops[t_loop].getLoopSend();
}

uint8_t Preset::getPresetLoopReturn(uint8_t t_loop) {
  return m_loops[t_loop].getLoopReturn();
}

uint8_t Preset::getMidiMessagesCount() const {
  return m_midiMessagesCount;
}

void Preset::setMidiMessagesCount(uint8_t t_count) {
  m_midiMessagesCount = t_count;
}

uint8_t Preset::getMidiMessageStatusByte(uint8_t t_message) const {
  return m_midiMessages[t_message].getMessageStatusByte();
}

void Preset::setMidiMessageStatusByte(uint8_t t_message, uint8_t t_byte) {
  m_midiMessages[t_message].setMessageStatusByte(t_byte);
}

uint8_t Preset::getMidiMessageType(uint8_t t_message) const {
  return m_midiMessages[t_message].getMessageType();
}

uint8_t Preset::getMidiMessageChannel(uint8_t t_message) const {
  return m_midiMessages[t_message].getChannel();
}

uint8_t Preset::getMidiMessageDataByte1(uint8_t t_message) const {
  return m_midiMessages[t_message].getDataByte1();
}

void Preset::setMidiMessageDataByte1(uint8_t t_message, uint8_t t_byte) {
  m_midiMessages[t_message].setMessageDataByte1(t_byte);
}

uint8_t Preset::getMidiMessageDataByte2(uint8_t t_message) const {
  if (m_midiMessages[t_message].hasDataByte2()) {
    return m_midiMessages[t_message].getDataByte2();
  }
  else {
    return 0xFF; // Invalid value for no data byte
  }
}

void Preset::setMidiMessageDataByte2(uint8_t t_message, uint8_t t_byte) {
  m_midiMessages[t_message].setMessageDataByte2(t_byte);
}

bool Preset::getMidiMessageHasDataByte2(uint8_t t_message) const {
  return m_midiMessages[t_message].hasDataByte2();
}