#pragma once

#include "menu_base.h"

/// @brief Class representing a simple list menu, inheriting from the MenuBase class.
///
/// The ListMenu class provides a scrollable, selectable menu with a set number of items.
/// It handles navigation and selection within the list and displays each item on the screen.
class ListMenu : public MenuBase {
  private:
    const char** m_menuItems;      // Pointer to an array of menu item strings.
    uint8_t m_itemCount;           // Number of items in the menu.
    uint8_t m_selectedRow;         // Currently selected row index.
    uint8_t m_selectedColumn;      // Currently selected column index.
    uint8_t m_startIndex;          // Index of the first visible item in the menu list.
    uint8_t m_visibleRowCount;     // Number of rows visible on the screen.
    bool m_itemSelected = false;   // Flag indicating if an item has been selected.

    uint8_t m_rowCounts[c_maxRowsPerLayout];        // Array to track rows.
    uint8_t m_rowColumnCounts[c_maxRowsPerLayout];  // Array to track columns per row.

    bool m_isFooterActive = false;
    uint8_t m_footerColumnCount = 0;

  public:
    /// @brief Constructor to initialize ListMenu with display, items, and item count.
    /// @param t_display Pointer to the Display object used for rendering.
    /// @param t_items Pointer to an array of item strings for the menu.
    /// @param t_count The number of items in the menu.
    ListMenu(DisplayManager* t_display, LayoutManager* t_layout, const char** t_items, uint8_t t_count) :
      MenuBase(t_display, t_layout),
      m_menuItems(t_items),
      m_itemCount(t_count),
      m_selectedRow(0),
      m_selectedColumn(0),
      m_startIndex(0) {
        m_visibleRowCount = m_displayManager->getMaxVisibleLines() - 2; // Account for the header
      };

    /// @brief Updates the display to render the menu, including the header and list of items.
    void update() override;

    /// @brief Resets the menu's state, clearing selection and returning to the start of the list.
    void reset() override;

    /// @brief Handles user input actions to navigate and select items within the menu.
    /// @param t_action The input action, such as moving up, down, or selecting an item.
    void handleInput(MenuInputAction t_action) override;

    uint8_t getSelectedIndex();

    /// @brief Checks if an item has been selected.
    /// @return true if an item is selected, false otherwise.
    bool isItemSelected();
};
