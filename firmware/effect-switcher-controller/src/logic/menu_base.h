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
    DisplayManager* m_displayManager;               // Display manager instance
    LayoutManager* m_layoutManager;                 // Layout manager instance

    uint8_t m_itemCount = 0;                        // Number of items in the menu.
    uint8_t m_visibleRowCount = 0;                  // Number of rows visible on the screen.
    uint8_t m_selectedRow = 0;                      // Currently selected row index.
    uint8_t m_selectedColumn = 0;                   // Currently selected column index.
    uint8_t m_startIndex = 0;                       // Index of the first visible item in the menu list.

    uint8_t m_rowCounts[c_maxRowsPerLayout];        // Array to track rows.
    uint8_t m_rowColumnCounts[c_maxRowsPerLayout];  // Array to track columns per row.
    uint8_t m_footerColumnCount = 0;

    bool m_isFooterActive = false;
    bool m_itemSelected = false;                    // Flag indicating if an item has been selected.

  public:
    /// @brief Constructor for the base menu
    /// @param t_displayWidth Width of the display
    /// @param t_displayHeight Height of the display
    MenuBase(DisplayManager* t_display, LayoutManager* t_layout) :
      m_displayManager(t_display),
      m_layoutManager(t_layout) {
        m_visibleRowCount = m_displayManager->getMaxVisibleLines() - 2; // Account for the header and footer
      };

    /// @brief Update the menu (called in the main loop)
    virtual void update() = 0;

    /// @brief Reset the menu to its initial state
    virtual void reset() = 0;

    /// @brief Handle user input (e.g., button press, encoder turn)
    virtual void handleInput(MenuInputAction t_action) = 0;
};