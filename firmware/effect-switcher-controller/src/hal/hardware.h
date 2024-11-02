#pragma once

#include <Arduino.h>
#include "logic/preset.h"
#include "logic/memory.h"
#include "logic/menu_base.h"
#include "logic/menu_manager.h"
#include "logic/home_menu.h"
#include "logic/list_menu.h"
#include "logic/loop_menu.h"
#include "logic/preset_manager.h"
#include "peripherals/encoder.h"
#include "peripherals/led.h"
#include "peripherals/switch.h"
#include "peripherals/leddriver.h"
#include "peripherals/switchmatrix.h"


constexpr uint8_t c_firstPresetBank = 65; // A
constexpr uint8_t c_firstPreset = 48; // 0
constexpr uint8_t c_maxPresets = 4;
constexpr uint8_t c_firstLoop = 0;

enum SystemState {
  kPresetState,
  kSettingsState,
  kLoopsEditState
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

    void pollSwitch(MomentarySwitch& t_switch, bool& t_pressFlag);
    void pollSwitch(MomentarySwitch& t_switch, bool& t_pressFlag, bool& t_longPressFlag);

    void pollMenuEncoder();
    void pollMenuEditSwitch();
    void pollFootSwitches();

    void transitionToState(SystemState t_newState);
    void processPresetState();
    void processSettingsState();
    void processLoopsEditState();

  public:
    void setup();
    void startup();
    void poll();
    void process();
    void resetTriggers();
};
