#include "logic/preset_manager.h"

void PresetManager::loadPresetBank(uint8_t t_bank) {
  for (uint8_t i = 0; i < c_maxPresetsPerBank; i++) {
    m_memoryManager.loadPreset(t_bank, i, m_presetBanks[i]);
  }
}

void PresetManager::initialize() {
  uint8_t lastBank = 0;
  uint8_t lastPreset = 0;
  m_memoryManager.loadDeviceState(lastBank, lastPreset);
  LOG_DEBUG("Last bank: %d, Last preset: %d", lastBank, lastPreset);

  setPresetBank(lastBank);
  setCurrentPreset(lastPreset);
}

uint8_t PresetManager::getCurrentBank() const {
  return m_currentPresetBank;
}

void PresetManager::setPresetBank(uint8_t t_bank) {
  if (t_bank < c_maxPresetBanks) {
    loadPresetBank(t_bank);
    m_currentPresetBank = t_bank;
    m_currentPresetIndex = 0;
    p_currentPreset = &m_presetBanks[m_currentPresetIndex];

    LOG_DEBUG("Current bank: %d, Current preset: %d", m_currentPresetBank, m_currentPresetIndex);

    m_memoryManager.saveDeviceState(m_currentPresetBank, m_currentPresetIndex);
  } else {
    LOG_DEBUG("Invalid bank: %d", t_bank);
  }
}

void PresetManager::setPresetBankUp() {
  // If the current bank is the last one, wrap around to 0
  if ((m_currentPresetBank + 1) >= c_maxPresetBanks) {
    setPresetBank(0);
  } else {
    setPresetBank(m_currentPresetBank + 1);
  }
}

void PresetManager::setPresetBankDown() {
  // If the current bank is the first one, wrap around to the last bank
  if (m_currentPresetBank == 0) {
    setPresetBank(c_maxPresetBanks - 1);
  } else {
    setPresetBank(m_currentPresetBank - 1);
  }
}

Preset* PresetManager::getCurrentPreset() const {
  return p_currentPreset;
}

uint8_t PresetManager::getCurrentPresetIndex() const {
  return m_currentPresetIndex;
}

void PresetManager::setCurrentPreset(uint8_t t_presetIndex) {
  if (t_presetIndex < c_maxPresetsPerBank) {
    m_currentPresetIndex = t_presetIndex;
    p_currentPreset = &m_presetBanks[m_currentPresetIndex];

    m_memoryManager.saveDeviceState(m_currentPresetBank, m_currentPresetIndex);

    LOG_DEBUG("Current bank: %d, Current preset: %d", m_currentPresetBank, m_currentPresetIndex);
  }
  else {
    LOG_DEBUG("Invalid preset: %d", t_presetIndex);
  }
}

void PresetManager::saveCurrentPreset() {
  m_memoryManager.savePreset(m_currentPresetBank, m_currentPresetIndex, *p_currentPreset);
  LOG_DEBUG("Saved current preset: Bank %d, Preset %d", m_currentPresetBank, m_currentPresetIndex);
}

void PresetManager::toggleLoopState(uint8_t t_loop) {
  p_currentPreset->toggleLoopState(t_loop);
}

void PresetManager::swapLoops(uint8_t t_loop1, uint8_t t_loop2) {
  p_currentPreset->swapPresetLoopsOrder(t_loop1, t_loop2);
}

uint8_t PresetManager::getLoopByOrder(uint8_t t_order) {
  return p_currentPreset->getLoopIndexByOrder(t_order);
}

void PresetManager::addMidiMessage(uint8_t t_type, uint8_t t_channel, uint8_t t_byte1, uint8_t t_byte2, bool t_hasDataByte2) {
  p_currentPreset->AddMidiMessage(t_type, t_channel, t_byte1, t_byte2, t_hasDataByte2);
}

void PresetManager::setMidiMessageValues(uint8_t t_message, uint8_t t_type, uint8_t t_channel, uint8_t t_byte1, uint8_t t_byte2, bool t_hasDataByte2) {
  p_currentPreset->setMidiMessageType(t_message, t_type);
  p_currentPreset->setMidiMessageChannel(t_message, t_channel);
  p_currentPreset->setMidiMessageDataByte1(t_message, t_byte1);

  if (t_hasDataByte2) {
    p_currentPreset->setMidiMessageDataByte2(t_message, t_byte2);
  }
  else {
    p_currentPreset->setMidiMessageDataByte2(t_message, 255);
  }
}

void PresetManager::removeMidiMessage(uint8_t t_message) {
  p_currentPreset->removeMidiMessage(t_message);
}