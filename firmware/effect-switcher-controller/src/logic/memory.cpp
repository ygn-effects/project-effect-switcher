#include "memory.h"

uint16_t MemoryManager::calculatePresetAddress(uint8_t t_bank, uint8_t t_presetIndex) const {
  return c_banksStartAddress + (t_bank * c_presetsPerBank + t_presetIndex) * c_presetSize;
}

uint16_t MemoryManager::calculateFootSwitchConfigAddress(uint8_t t_bank, uint8_t t_footSwitchIndex) const {
  return c_footSwitchConfigStartAddress + (t_bank * c_footSwitchConfigPerBank + t_footSwitchIndex) * c_footSwitchConfigSize;
}

void MemoryManager::serializePreset(const Preset& t_preset, uint8_t* t_buffer) const {
  // Basic preset data
  t_buffer[0] = t_preset.getBank();
  t_buffer[1] = t_preset.getPreset();
  t_buffer[2] = t_preset.getLoopsCount();
  t_buffer[3] = t_preset.getMidiMessagesCount();

  // Loops data: start at address 4, each loop occupies 4 bytes
  for (uint8_t i = 0; i < t_preset.getLoopsCount(); i++) {
    uint16_t loopOffset = 4 + i * 4;  // Start at byte 4, 4 bytes per loop
    t_buffer[loopOffset] = t_preset.getLoopState(i);
    t_buffer[loopOffset + 1] = t_preset.getLoopOrder(i);
    t_buffer[loopOffset + 2] = t_preset.getLoopSend(i);
    t_buffer[loopOffset + 3] = t_preset.getLoopReturn(i);
  }

  // MIDI messages data: start after loops data, each message occupies 4 bytes
  uint8_t midiOffset = 4 + t_preset.getLoopsCount() * 4;  // Calculate start address for MIDI messages
  for (uint8_t j = 0; j < t_preset.getMidiMessagesCount(); j++) {
    uint8_t msgOffset = midiOffset + j * 4;
    t_buffer[msgOffset] = t_preset.getMidiMessageStatusByte(j);
    t_buffer[msgOffset + 1] = t_preset.getMidiMessageDataByte1(j);
    t_buffer[msgOffset + 2] = t_preset.getMidiMessageDataByte2(j);
  }
}

void MemoryManager::deserializePreset(const uint8_t* t_buffer, Preset& t_preset) const {
  // Basic preset data
  t_preset.setBank(t_buffer[0]);
  t_preset.setPreset(t_buffer[1]);

  uint8_t loopsCount = t_buffer[2];
  t_preset.setLoopsCount(loopsCount);

  uint8_t midiMessageCount = t_buffer[3];
  t_preset.setMidiMessagesCount(midiMessageCount);

  // Loops data: start at address 4, each loop occupies 4 bytes
  for (uint8_t i = 0; i < loopsCount; i++) {
    uint8_t loopOffset = 4 + i * 4;
    t_preset.setLoopState(i, t_buffer[loopOffset]);
    t_preset.setLoopOrder(i, t_buffer[loopOffset + 1]);
    t_preset.setLoopSend(i, t_buffer[loopOffset + 2]);
    t_preset.setLoopReturn(i, t_buffer[loopOffset + 3]);
  }

  // MIDI messages data: start after loops data, each message occupies 4 bytes
  uint8_t midiOffset = 4 + loopsCount * 4;  // Calculate start address for MIDI messages
  for (uint8_t j = 0; j < midiMessageCount; j++) {
    uint8_t msgOffset = midiOffset + j * 4;
    t_preset.setMidiMessageStatusByte(j, t_buffer[msgOffset]);
    t_preset.setMidiMessageDataByte1(j, t_buffer[msgOffset + 1]);
    t_preset.setMidiMessageDataByte2(j, t_buffer[msgOffset + 2]);
  }
}

void MemoryManager::serializeFootSwitchConfig(const FootSwitchConfig& t_config, uint8_t* t_buffer) const {
  t_buffer[0] = uint8_t(t_config.getMode());
  t_buffer[1] = t_config.getLatching();
  t_buffer[2] = t_config.getLoopIndex();
  t_buffer[3] = t_config.getTargetBank();
  t_buffer[4] = t_config.getTargetPreset();

  for (uint8_t i = 0 ; i < 2; i++) {
    uint8_t baseIndex = 5 + i * 3;
    t_buffer[baseIndex] = t_config.getMidiMessageType(i);
    t_buffer[baseIndex + 1] = t_config.getMidiMessageDataByte1(i);
    t_buffer[baseIndex + 2] = t_config.getMidiMessageDataByte2(i);
  }
}

void MemoryManager::deserializeFootSwitchConfig(const uint8_t* t_buffer, FootSwitchConfig& t_config) const {
  t_config.setMode(static_cast<FootSwitchMode>(t_buffer[0]));
  t_config.setLatching(t_buffer[1]);
  t_config.setLoopIndex(t_buffer[2]);
  t_config.setTargetBank(t_buffer[3]);
  t_config.setTargetPreset(t_buffer[4]);

  for (uint8_t i = 0; i < 2; i++) {
    uint8_t baseIndex = 5 + i * 3;
    t_config.setMidiMessage(i, t_buffer[baseIndex], t_buffer[baseIndex + 1], t_buffer[baseIndex + 2]);
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

void MemoryManager::saveFootSwitchConfig(uint8_t t_bank, uint8_t t_footSwitchIndex, const FootSwitchConfig& t_config) {
  uint16_t address = calculateFootSwitchConfigAddress(t_bank, t_footSwitchIndex);
  uint8_t buffer[c_footSwitchConfigSize];

  serializeFootSwitchConfig(t_config, buffer);

  for (uint8_t i = 0; i < c_footSwitchConfigSize; i++) {
    eeprom.writeInt8(address + i, buffer[i]);
  }
}

void MemoryManager::loadFootSwitchConfig(uint8_t t_bank, uint8_t t_footSwitchIndex, FootSwitchConfig& t_config) {
  uint16_t address = calculateFootSwitchConfigAddress(t_bank, t_footSwitchIndex);
  uint8_t buffer[c_footSwitchConfigSize];

  for (uint8_t i = 0; i < c_footSwitchConfigSize; i++) {
    buffer[i] = eeprom.readInt8(address + i);
  }

  deserializeFootSwitchConfig(buffer, t_config);
}

/// Test functions

void MemoryManager::initializeTestData() {
  // Loop through 4 banks
  for (uint8_t bank = 0; bank < 4; bank++) {
    // Loop through 4 presets per bank
    for (uint8_t presetIndex = 0; presetIndex < 4; presetIndex++) {
      Preset testPreset(bank, presetIndex, 8, 1);  // Initialize Preset with 8 loops and 1 MIDI message

      // Set each loop's test data
      for (uint8_t loopIndex = 0; loopIndex < 8; loopIndex++) {
        testPreset.setLoopState(loopIndex, loopIndex % 2);    // Alternating states (0,1)
        testPreset.setLoopOrder(loopIndex, loopIndex);        // Sequential order
        testPreset.setLoopSend(loopIndex, loopIndex * 2);     // Example send values
        testPreset.setLoopReturn(loopIndex, loopIndex * 3);   // Example return values
      }

      // Set a test MIDI message
      testPreset.setMidiMessageStatusByte(0, 0xB0 | 0x1); // CC message channel 1
      testPreset.setMidiMessageDataByte1(0, 32);
      testPreset.setMidiMessageDataByte2(0, 64);

      // Save initialized preset to EEPROM
      savePreset(bank, presetIndex, testPreset);
    }

    // Configure 6 footswitches per bank
    for (uint8_t footSwitchIndex = 0; footSwitchIndex < 6; footSwitchIndex++) {
      FootSwitchConfig footSwitchConfig(FootSwitchMode::kNone);

      switch (footSwitchIndex) {
        case 0: // Bank select 0
          footSwitchConfig.setMode(FootSwitchMode::kBankSelect);
          footSwitchConfig.setTargetBank(0);
          break;

        case 1: // Bank select 1
          footSwitchConfig.setMode(FootSwitchMode::kBankSelect);
          footSwitchConfig.setTargetBank(1);
          break;

        case 2: // Preset select 0
          footSwitchConfig.setMode(FootSwitchMode::kPresetSelect);
          footSwitchConfig.setTargetPreset(0);
          break;

        case 3: // Preset select 1
          footSwitchConfig.setMode(FootSwitchMode::kPresetSelect);
          footSwitchConfig.setTargetPreset(1);
          break;

        case 4: // Preset select 2
          footSwitchConfig.setMode(FootSwitchMode::kPresetSelect);
          footSwitchConfig.setTargetPreset(2);
          break;

        case 5: // Preset select 3
          footSwitchConfig.setMode(FootSwitchMode::kPresetSelect);
          footSwitchConfig.setTargetPreset(3);
          break;
      }

      // Save footswitch configuration to EEPROM
      saveFootSwitchConfig(bank, footSwitchIndex, footSwitchConfig);
    }
  }

  saveDeviceState(1, 0); // Set initial device state
}


void MemoryManager::readTestData() {
  // Loop through 4 banks
  for (uint8_t bank = 0; bank < 4; bank++) {
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

      // Log MIDI messages
      for (uint8_t midiIndex = 0; midiIndex < testPreset.getMidiMessagesCount(); midiIndex++) {
        LOG_DEBUG("    MIDI Message %d:", midiIndex);
        LOG_DEBUG("      Type: 0x%X", testPreset.getMidiMessageType(midiIndex));
        LOG_DEBUG("      Channel: %d", testPreset.getMidiMessageChannel(midiIndex));
        LOG_DEBUG("      Data Byte 1: %d", testPreset.getMidiMessageDataByte1(midiIndex));
        LOG_DEBUG("      Data Byte 2: %d", testPreset.getMidiMessageDataByte2(midiIndex));
      }
    }

    // Log footswitch configurations for each bank
    for (uint8_t footSwitchIndex = 0; footSwitchIndex < 6; footSwitchIndex++) {
      FootSwitchConfig footSwitchConfig(FootSwitchMode::kNone);

      // Load footswitch configuration from EEPROM
      loadFootSwitchConfig(bank, footSwitchIndex, footSwitchConfig);

      LOG_DEBUG("  Footswitch %d:", footSwitchIndex);
      LOG_DEBUG("    Mode: %d", static_cast<uint8_t>(footSwitchConfig.getMode()));
      LOG_DEBUG("    Latching: %d", footSwitchConfig.getLatching());
      LOG_DEBUG("    Loop Index: %d", footSwitchConfig.getLoopIndex());
      LOG_DEBUG("    Target Bank: %d", footSwitchConfig.getTargetBank());
      LOG_DEBUG("    Target Preset: %d", footSwitchConfig.getTargetPreset());

      for (uint8_t i = 0; i < 2; i++) {
        LOG_DEBUG("    MIDI Message %d:", i);
        LOG_DEBUG("      Type: 0x%X", footSwitchConfig.getMidiMessageType(i));
        LOG_DEBUG("      Channel: %d", footSwitchConfig.getMidiMessageChannel(i));
        LOG_DEBUG("      Data Byte 1: %d", footSwitchConfig.getMidiMessageDataByte1(i));
        LOG_DEBUG("      Data Byte 2: %d", footSwitchConfig.getMidiMessageDataByte2(i));
      }
    }
  }

  uint8_t lastBank = 0;
  uint8_t lastPreset = 0;
  loadDeviceState(lastBank, lastPreset);

  LOG_DEBUG("Last bank: %d, Last preset: %d", lastBank, lastPreset);
}
