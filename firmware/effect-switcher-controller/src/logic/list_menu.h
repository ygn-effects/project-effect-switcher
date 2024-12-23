#pragma once

#include "menu_base.h"

/// @brief Class representing a simple list menu, inheriting from the MenuBase class.
///
/// The ListMenu class provides a scrollable, selectable menu with a set number of items.
/// It handles navigation and selection within the list and displays each item on the screen.
class ListMenu : public MenuBase {
  private:
    const char** m_menuItems;      // Pointer to an array of menu item strings.
    const char* m_header;
    bool m_backrequested = false;

  public:
    /// @brief Constructor to initialize ListMenu with display, items, and item count.
    /// @param t_display Pointer to the Display object used for rendering.
    /// @param t_items Pointer to an array of item strings for the menu.
    /// @param t_count The number of items in the menu.
    ListMenu(DisplayManager* t_display, LayoutManager* t_layout, const char** t_items, uint8_t t_count, const char* t_header) :
      MenuBase(t_display, t_layout, t_count),
      m_menuItems(t_items),
      m_header(t_header) { };

    /// @brief Updates the display to render the menu, including the header and list of items.
    void update() override;

    /// @brief Resets the menu's state, clearing selection and returning to the start of the list.
    void reset() override;

    void handleAction(MenuInputAction t_action) override;

    /// @brief Checks if an item has been selected.
    /// @return true if an item is selected, false otherwise.
    bool isItemSelected();

    bool isBackRequested();
};
