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

void Preset::swapPresetLoopsOrder(uint8_t t_loop1, uint8_t t_order1, uint8_t t_loop2, uint8_t t_order2) {
  m_loops[t_loop1].setLoopOrder(t_order2);
  m_loops[t_loop2].setLoopOrder(t_order1);
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