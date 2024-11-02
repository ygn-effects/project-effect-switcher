#pragma once
#include <Arduino.h>
#include "peripherals/display.h"

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
    Display* m_display;  // Pointer to the display driver

  public:
    /// @brief Constructor for the base menu
    MenuBase(Display* t_display) : m_display(t_display) {}

    /// @brief Update the menu (called in the main loop)
    virtual void update() = 0;

    /// @brief Reset the menu so its initial state
    virtual void reset() = 0;

    /// @brief Handle user input (e.g., button press, encoder turn)
    virtual void handleInput(MenuInputAction t_action) = 0;
};
