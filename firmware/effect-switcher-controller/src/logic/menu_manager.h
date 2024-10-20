#pragma once

#include "menu_base.h"

/// @brief Manages different menus and handles state transitions
class MenuManager {
  private:
    MenuBase* m_currentMenu;     // Pointer to the current active menu

  public:
    /// @brief Constructor
    MenuManager() : m_currentMenu(nullptr) {}

    /// @brief Set the active menu (switch state)
    /// @param menu Pointer to the new menu to set as active
    void setMenu(MenuBase* menu) {
      m_currentMenu = menu;
    }

    /// @brief Update the current menu (called in the main loop)
    void update() {
      if (m_currentMenu) {
        m_currentMenu->update();   // Delegate update logic to the current menu
      }
    }

    /// @brief Handle input for the current menu
    /// @param input The input event (e.g., button press, encoder turn)
    void handleInput(uint8_t input) {
      if (m_currentMenu) {
        m_currentMenu->handleInput(input);   // Delegate input handling to the current menu
      }
    }
};
