#pragma once

#include "menu_base.h"
#include "peripherals/display.h"
#include "logic/preset_view.h"

class LoopOrderMenu : public MenuBase {
  private:
    PresetView* m_presetView;
    uint8_t m_itemsCount;
    uint8_t m_loopsCount;
    uint8_t m_selectedLoop;
    uint8_t m_sourceLoop;
    uint8_t m_targetLoop;

    bool m_swappingMode;
    bool m_toggleRequested;
    bool m_swapRequested;
    bool m_goBackRequested;

  public:
    LoopOrderMenu(DisplayManager* t_display, LayoutManager* t_layout, PresetView* t_view) :
      MenuBase(t_display, t_layout),
      m_presetView(t_view),
      m_itemsCount(0),
      m_loopsCount(0),
      m_selectedLoop(0),
      m_targetLoop(0),
      m_swappingMode(false),
      m_toggleRequested(false),
      m_swapRequested(false),
      m_goBackRequested(false) { };

    void update() override;
    void reset() override;
    void handleInput(MenuInputAction t_action) override;

    void setPresetView(PresetView* t_view);
    bool isToggleRequested();
    bool isSwapRequested();
    bool isGoBackRequested();
    uint8_t getSelectedLoop() const;
    uint8_t getSourceLoop() const;
    uint8_t getTargetLoop() const;
};