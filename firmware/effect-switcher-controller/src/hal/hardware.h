#pragma once

#include <Arduino.h>
#include "logic/preset.h"
#include "logic/memory.h"
#include "logic/menu_base.h"
#include "logic/menu_manager.h"
#include "logic/home_menu.h"
#include "peripherals/encoder.h"
#include "peripherals/led.h"
#include "peripherals/switch.h"
#include "peripherals/leddriver.h"
#include "peripherals/switchmatrix.h"


constexpr uint8_t c_firstPresetBank = 65; // A
constexpr uint8_t c_maxPresetBanks = 10;
constexpr uint8_t c_firstPreset = 48; // 0
constexpr uint8_t c_maxPresets = 4;
constexpr uint8_t c_firstLoop = 0;
constexpr uint8_t c_maxLoops = 8;

enum SystemState {
  kPresetState,
  kEditMenuState
};

class Hardware
{
  private:
    SystemState m_systemState = kPresetState;

    // Hardware Triggers
    bool m_menuEncoderMove = false;
    bool m_menuEncoderMoveRight = false;
    bool m_menuEncoderMoveLeft = false;
    bool m_menuEncoderSwitchPress = false;
    bool m_menuEncoderSwitchLongPress = false;
    bool m_menuEditSwitchPress = false;
    bool m_menuEditSwitchLongPress = false;
    bool m_presetBankUpFswPress = false;
    bool m_presetBankUpFswLongPress = false;
    bool m_presetBankDownFswPress = false;
    bool m_presetBankDownFswLongPress = false;
    bool m_preset0FswPress = false;
    bool m_preset1FswPress = false;
    bool m_preset2FswPress = false;
    bool m_preset3FswPress = false;

    // Loops Ins/Outs
    const uint8_t c_inputLoopReturn = 0; // X0
    const uint8_t c_tunerLoopSend = 15; // Y15
    const uint8_t c_buffer1LoopSend = 14; // Y14
    const uint8_t c_buffer1LoopReturn = 1; // X1
    const uint8_t c_buffer2LoopSend = 13; // Y13
    const uint8_t c_buffer2LoopReturn = 2; // X1
    const uint8_t c_loop0Send = 12;
    const uint8_t c_loop0Return = 3;
    const uint8_t c_loop1Send = 11;
    const uint8_t c_loop1Return = 4;
    const uint8_t c_loop2Send = 10;
    const uint8_t c_loop2Return = 5;
    const uint8_t c_loop3Send = 9;
    const uint8_t c_loop3Return = 6;
    const uint8_t c_loop4Send = 8;
    const uint8_t c_loop4Return = 7;
    const uint8_t c_loop5Send = 7;
    const uint8_t c_loop5Return = 8;
    const uint8_t c_outputLoopSend = 0; // Y0;

    // Presets logic
    uint8_t m_currentPresetBank = 0;
    uint8_t m_currentPreset = 0;
    Preset *p_currentPreset = NULL;

    void pollMenuEncoder();
    void pollMenuEditSwitch();
    void pollFootSwitches();

  public:
    void setup();
    void startup();
    void poll();
    void restoreLastState();
    void resetTriggers();

    void loadPresetBank();
    void loadPreset();
    void savePreset();
};
