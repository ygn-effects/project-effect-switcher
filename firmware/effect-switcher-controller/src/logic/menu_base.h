#pragma once

#include <Arduino.h>
#include "logic/layout_manager.h"

enum class MenuInputAction {
  kNone,
  kUp,
  kDown,
  kPress,
  kLongPress
};

/// @brief Base class for all menu types
class MenuBase {
  protected:
    DisplayManager* m_displayManager;  // Display manager instance
    LayoutManager* m_layoutManager;   // Layout manager instance

  public:
    /// @brief Constructor for the base menu
    /// @param t_displayWidth Width of the display
    /// @param t_displayHeight Height of the display
    MenuBase(DisplayManager* t_display, LayoutManager* t_layout) :
      m_displayManager(t_display),
      m_layoutManager(t_layout) {
    };

    /// @brief Update the menu (called in the main loop)
    virtual void update() = 0;

    /// @brief Reset the menu to its initial state
    virtual void reset() = 0;

    /// @brief Handle user input (e.g., button press, encoder turn)
    virtual void handleInput(MenuInputAction t_action) = 0;
};