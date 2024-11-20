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
    /// @param t_display Pointer to the display driver
    /// @param t_currentPreset Pointer to the current preset
    HomeMenu(DisplayManager* t_display, LayoutManager* t_layout, const Preset* t_currentPreset) :
      MenuBase(t_display, t_layout),
      m_currentPreset(t_currentPreset) { };

    /// @brief Update the home menu (render the current bank and preset)
    void update() override;

    /// @brief Update the current preset pointer
    /// @param currentPreset Pointer to the new current preset
    void setCurrentPreset(const Preset* t_currentPreset);

    /// @brief Reset the menu to its initial state
    void reset() override;

    /// @brief Handle input (home menu doesn't need input handling)
    void handleInput(MenuInputAction t_action) override;
};