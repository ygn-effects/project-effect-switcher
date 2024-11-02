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
    /// @param t_menu Pointer to the new menu to set as active
    void setMenu(MenuBase* t_menu) {
      m_currentMenu = t_menu;
    }

    /// @brief Update the current menu (called in the main loop)
    void update() {
      if (m_currentMenu) {
        m_currentMenu->update();   // Delegate update logic to the current menu
      }
    }

    /// @brief Reset the menu to its initial state
    void reset() {
      if (m_currentMenu) {
        m_currentMenu->reset();
      }
    }

    /// @brief Handle input for the current menu
    /// @param input The input event (e.g., button press, encoder turn)
    void handleInput(MenuInputAction t_action) {
      if (m_currentMenu) {
        m_currentMenu->handleInput(t_action);   // Delegate input handling to the current menu
      }
    }
};
