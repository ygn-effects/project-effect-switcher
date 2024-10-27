#pragma once

#include "menu_base.h"
#include "peripherals/display.h"

/// @brief Home menu for displaying the last used bank and preset
class HomeMenu : public MenuBase {
  private:
    char m_lastBank;         // Last used bank (A, B, C, etc.)
    uint8_t m_lastPreset;    // Last used preset number

  public:
    /// @brief Constructor for HomeMenu
    /// @param display Pointer to the display driver
    /// @param lastBank The last used bank
    /// @param lastPreset The last used preset
    HomeMenu(Display* display, char lastBank, uint8_t lastPreset) :
      MenuBase(display),
      m_lastBank(lastBank),
      m_lastPreset(lastPreset) { };

    /// @brief Update the home menu (render the current bank and preset)
    void update() override {
      m_display->clear();
      m_display->renderHeader("Current Preset");         // Display header
      m_display->renderBankAndPreset(m_lastBank, m_lastPreset);  // Display bank and preset
    }

    /// @brief Handle input (home menu doesn't need input handling)
    void handleInput(uint8_t t_input) override {
      // No input handling needed for the home menu
    }
};
