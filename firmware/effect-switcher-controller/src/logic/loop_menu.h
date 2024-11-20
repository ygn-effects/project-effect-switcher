#pragma once

#include "menu_base.h"
#include "peripherals/display.h"
#include "logic/preset.h"

class LoopOrderMenu : public MenuBase {
  private:
    Preset* m_currentPreset;
    uint8_t m_selectedLoop;
    uint8_t m_sourceLoop;
    uint8_t m_targetLoop;
    bool m_swappingMode;
    bool m_toggleRequested;
    bool m_swapRequested;
    bool m_goBackRequested;

  public:
    LoopOrderMenu(DisplayManager* t_display, LayoutManager* t_layout, Preset* t_currentPreset) :
      MenuBase(t_display, t_layout),
      m_currentPreset(t_currentPreset),
      m_selectedLoop(0),
      m_targetLoop(0),
      m_swappingMode(false),
      m_toggleRequested(false),
      m_swapRequested(false),
      m_goBackRequested(false) { };

    void update() override;
    void reset() override;
    void handleInput(MenuInputAction t_action) override;

    void setCurrentPreset(Preset* t_currentPreset);
    bool isToggleRequested();
    bool isSwapRequested();
    bool isGoBackRequested();
    uint8_t getSelectedLoop() const;
    uint8_t getSourceLoop() const;
    uint8_t getTargetLoop() const;
};