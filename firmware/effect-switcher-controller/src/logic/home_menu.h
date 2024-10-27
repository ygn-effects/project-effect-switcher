#pragma once

#include "menu_base.h"
#include "logic/preset.h"
#include "peripherals/display.h"

/// @brief Home menu for displaying the last used bank and preset
class HomeMenu : public MenuBase {
  private:
    const Preset* m_currentPreset;  // Pointer to the current preset

  public:
    /// @brief Constructor for HomeMenu
    /// @param display Pointer to the display driver
    /// @param currentPreset Pointer to the current preset
    HomeMenu(Display* display, const Preset* currentPreset) :
      MenuBase(display),
      m_currentPreset(currentPreset) { };

    /// @brief Update the home menu (render the current bank and preset)
    void update() override {
      m_display->clear();
      m_display->renderHeader("Current Preset");

      char bankLetter = 'A' + m_currentPreset->getBank();
      uint8_t presetNumber = m_currentPreset->getPreset();
      m_display->renderBankAndPreset(bankLetter, presetNumber);
    }

    /// @brief Update the current preset pointer
    /// @param currentPreset Pointer to the new current preset
    void setCurrentPreset(const Preset* currentPreset) {
      m_currentPreset = currentPreset;
    }

    /// @brief Handle input (home menu doesn't need input handling)
    void handleInput(uint8_t t_input) override {

    }
};
