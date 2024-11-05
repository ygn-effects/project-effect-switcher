#pragma once

#include "menu_base.h"
#include "peripherals/display.h"
#include "logic/preset.h"

class MidiMessageMenu : public MenuBase {
  private:
    Preset* m_currentPreset;
    uint8_t m_selectedIndex;
    uint8_t m_startIndex;
    bool m_editRequested;
    bool m_goBackRequested;

  public:
    MidiMessageMenu(Display* t_display, Preset* t_curentPreset) :
      MenuBase(t_display),
      m_currentPreset(t_curentPreset),
      m_selectedIndex(0),
      m_startIndex(0),
      m_editRequested(false),
      m_goBackRequested(false) { };

    void update() override;
    void reset() override;
    void handleInput(MenuInputAction t_action) override;

    void setCurrentPreset(Preset* t_currentPreset);
    bool isEditRequested();
    bool isGoBackRequested();
};