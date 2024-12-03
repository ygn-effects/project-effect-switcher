#pragma once

#include "menu_base.h"
#include "peripherals/display.h"
#include "logic/preset_view.h"

class LoopOrderMenu : public MenuBase {
  private:
    uint8_t m_sourceLoop;
    uint8_t m_targetLoop;

    bool m_swappingMode;
    bool m_saveRequested;
    bool m_backRequested;

  public:
    LoopOrderMenu(DisplayManager* t_display, LayoutManager* t_layout) :
      MenuBase(t_display, t_layout),
      m_targetLoop(0),
      m_swappingMode(false),
      m_saveRequested(false),
      m_backRequested(false) { };

    void update() override;
    void reset() override;

    void handleAction(MenuInputAction t_action) override;

    bool isSaveRequested();
    bool isBackRequested();
};