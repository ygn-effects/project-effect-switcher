#pragma once

#include <Arduino.h>
#include "logic/preset.h"
#include "logic/memory.h"
#include "logic/menu_base.h"
#include "logic/menu_manager.h"
#include "logic/home_menu.h"
#include "logic/list_menu.h"
#include "logic/loop_menu.h"
#include "logic/midi_menu.h"
#include "logic/preset_manager.h"
#include "logic/preset_view.h"
#include "peripherals/encoder.h"
#include "peripherals/led.h"
#include "peripherals/switch.h"
#include "peripherals/leddriver.h"
#include "peripherals/switchmatrix.h"

constexpr uint8_t c_maxPresets = 4;
constexpr uint8_t c_firstLoop = 0;

/// @brief Possible system states
enum SystemState {
  kPresetState,           // Home screen displaying the active bank and preset
  kSettingsState,         // Main preset settings menu
  kLoopsEditState,        // Loops edit menu
  kMidiMessagesState,     // MIDI messages list
  kMidiMessageEditState,  // MIDI messages edit
  kMidiMessageAddState    // MIDI Messages add
};

class Hardware
{
  private:
    SystemState m_systemState = kPresetState;

    PresetView m_presetView;

    // Hardware Triggers
    bool m_menuEncoderMove = false;
    bool m_menuEncoderMoveRight = false;
    bool m_menuEncoderMoveLeft = false;
    bool m_menuEncoderSwitchPress = false;
    bool m_menuEncoderSwitchLongPress = false;
    bool m_menuEditSwitchPress = false;
    bool m_menuEditSwitchLongPress = false;
    bool m_footSwitch0Press = false;
    bool m_footSwitch0LongPress = false;
    bool m_footSwitch1Press = false;
    bool m_footSwitch1LongPress = false;
    bool m_footSwitch2Press = false;
    bool m_footSwitch2LongPress = false;
    bool m_footSwitch3Press = false;
    bool m_footSwitch3LongPress = false;
    bool m_footSwitch4Press = false;
    bool m_footSwitch4LongPress = false;
    bool m_footSwitch5Press = false;
    bool m_footSwitch5LongPress = false;

    void pollSwitch(MomentarySwitch& t_switch, bool& t_pressFlag);
    void pollSwitch(MomentarySwitch& t_switch, bool& t_pressFlag, bool& t_longPressFlag);
    void processFootSwitchAction(uint8_t t_footSwitch, bool t_longPress = false);

    void pollMenuEncoder();
    void pollMenuEditSwitch();
    void pollFootSwitches();

    void transitionToState(SystemState t_newState);
    void processMenuInput();
    void processPresetState();
    void processSettingsState();
    void processLoopsEditState();
    void processMidiMessagesState();
    void processMidiMessageEditState();

    PresetView createPresetView(const Preset* t_preset);
    void applyPresetView(Preset* t_preset);

  public:
    void setup();
    void startup();
    void poll();
    void process();
    void resetTriggers();
};
