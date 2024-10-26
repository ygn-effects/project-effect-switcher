#include "memory.h"

uint16_t MemoryManager::calculatePresetAddress(uint8_t t_bank, uint8_t t_presetIndex) const {
  return c_banksStartAddress + (t_bank * c_presetsPerBank + t_presetIndex) * c_presetSize;
}


void MemoryManager::serializePreset(const Preset& t_preset, uint8_t* t_buffer) const {
  t_buffer[0] = t_preset.getBank(); // Address 0
  t_buffer[1] = t_preset.getPreset(); // Address 1
  t_buffer[2] = t_preset.getLoopsCount(); // Address 2

  for (uint8_t i = 0; i < t_preset.getLoopsCount(); i++) {
    t_buffer[3 + i * 4] = t_preset.getLoopState(i); // First loop, address 3
    t_buffer[3 + i * 4 + 1] = t_preset.getLoopOrder(i); // First loop, address 4
    t_buffer[3 + i * 4 + 2] = t_preset.getLoopSend(i); // First loop, address 5
    t_buffer[3 + i * 4 + 3] = t_preset.getLoopReturn(i); // First loop, address 6
  }
}

void MemoryManager::deserializePreset(const uint8_t* t_buffer, Preset& t_preset) const {
  t_preset.setBank(t_buffer[0]);
  t_preset.setPreset(t_buffer[1]);

  uint8_t loopsCount = t_buffer[2];
  t_preset.setLoopsCount(loopsCount);

  for (uint8_t i = 0; i < loopsCount; i++) {
    t_preset.setLoopState(i, t_buffer[3 + i * 4]);
    t_preset.setLoopOrder(i, t_buffer[3 + i * 4 + 1]);
    t_preset.setLoopSend(i, t_buffer[3 + i * 4 + 2]);
    t_preset.setLoopReturn(i, t_buffer[3 + i * 4 + 3]);
  }
}

void MemoryManager::saveDeviceState(uint8_t t_bank, uint8_t t_preset) {
  eeprom.writeInt8(c_deviceStateAddress, t_bank);
  eeprom.writeInt8(c_deviceStateAddress + 1, t_preset);
}

void MemoryManager::loadDeviceState(uint8_t& t_bank, uint8_t& t_preset) {
  t_bank = eeprom.readInt8(c_deviceStateAddress);
  t_preset = eeprom.readInt8(c_deviceStateAddress + 1);
}

void MemoryManager::savePreset(uint8_t t_bank, uint8_t t_presetIndex, const Preset& t_preset) {
  uint16_t address = calculatePresetAddress(t_bank, t_presetIndex);
  uint8_t buffer[c_presetSize];

  serializePreset(t_preset, buffer);

  for (uint16_t i = 0; i < c_presetSize; i++) {
      eeprom.writeInt8(address + i, buffer[i]);
  }
}

void MemoryManager::loadPreset(uint8_t t_bank, uint8_t t_presetIndex, Preset& t_preset) {
uint16_t address = calculatePresetAddress(t_bank, t_presetIndex);
  uint8_t buffer[c_presetSize];

  for (uint16_t i = 0; i < c_presetSize; i++) {
      buffer[i] = eeprom.readInt8(address + i);
  }

  deserializePreset(buffer, t_preset);
}

/// Test functions

void MemoryManager::initializeTestData() {
  // Loop through 8 banks
  for (uint8_t bank = 0; bank < 8; bank++) {
    // Loop through 4 presets per bank
    for (uint8_t presetIndex = 0; presetIndex < 4; presetIndex++) {
      Preset testPreset(bank, presetIndex, 8);  // Initialize Preset with 8 loops

      // Set each loop's test data
      for (uint8_t loopIndex = 0; loopIndex < 8; loopIndex++) {
        testPreset.setLoopState(loopIndex, loopIndex % 2);    // Alternating states (0,1)
        testPreset.setLoopOrder(loopIndex, loopIndex);        // Sequential order
        testPreset.setLoopSend(loopIndex, loopIndex * 2);     // Example send values
        testPreset.setLoopReturn(loopIndex, loopIndex * 3);   // Example return values
      }

      // Save initialized preset to EEPROM
      savePreset(bank, presetIndex, testPreset);
    }
  }

  saveDeviceState(1, 0);
}

void MemoryManager::readTestData() {
  // Loop through 8 banks
  for (uint8_t bank = 0; bank < 8; bank++) {
    LOG_DEBUG("Reading Bank %d:", bank);

    // Loop through 4 presets per bank
    for (uint8_t presetIndex = 0; presetIndex < 4; presetIndex++) {
      Preset testPreset;  // Empty preset to load data into

      // Load the preset from EEPROM
      loadPreset(bank, presetIndex, testPreset);

      LOG_DEBUG("  Preset %d:", presetIndex);
      LOG_DEBUG("    Bank: %d, Preset: %d", testPreset.getBank(), testPreset.getPreset());

      // Log each loop in the preset
      for (uint8_t loopIndex = 0; loopIndex < testPreset.getLoopsCount(); loopIndex++) {
        LOG_DEBUG("    Loop %d:", loopIndex);
        LOG_DEBUG("      State: %d", testPreset.getLoopState(loopIndex));
        LOG_DEBUG("      Order: %d", testPreset.getLoopOrder(loopIndex));
        LOG_DEBUG("      Send: %d", testPreset.getLoopSend(loopIndex));
        LOG_DEBUG("      Return: %d", testPreset.getLoopReturn(loopIndex));
      }
    }
  }
  uint8_t lastBank = 0;
  uint8_t lastPreset = 0;
  loadDeviceState(lastBank, lastPreset);

  LOG_DEBUG("Last bank: %d, Last preset: %d", lastBank, lastPreset);
}
