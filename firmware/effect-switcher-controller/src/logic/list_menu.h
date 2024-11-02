#pragma once

#include "menu_base.h"

class ListMenu : public MenuBase {
  private:
    const char** m_menuItems;
    uint8_t m_itemCount;
    uint8_t m_selectedIndex;
    uint8_t m_startIndex;

    bool m_itemSelected = false;

  public:
    ListMenu(Display* t_display, const char** t_items, uint8_t t_count) :
      MenuBase(t_display),
      m_menuItems(t_items),
      m_itemCount(t_count),
      m_selectedIndex(0),
      m_startIndex(0) { };

    void update() override {
      m_display->clear();
      m_display->renderHeader("Settings");
      m_display->renderListMenu(m_menuItems, m_itemCount, m_startIndex, m_selectedIndex);
    }

    void reset() override {
      m_itemSelected = false;
      m_selectedIndex = 0;
    }

    bool isItemSelected() {
      return m_itemSelected;
    }

    uint8_t getSelectedIndex() {
      m_itemSelected = false;
      return m_selectedIndex;
    }

    void handleInput(MenuInputAction t_action) override {
      switch (t_action) {
        case MenuInputAction::kUp:
          if (m_selectedIndex > 0) {
            m_selectedIndex--;
            if (m_selectedIndex < m_startIndex) m_startIndex--;
          }
          break;

        case MenuInputAction::kDown:
          if (m_selectedIndex < m_itemCount - 1) {
            m_selectedIndex++;
            if (m_selectedIndex >= m_startIndex + (m_display->getHeight() / m_display->getLineHeight()))
              m_startIndex++;
          }
          break;

        case MenuInputAction::kPress:
          m_itemSelected = true;
          break;

        case MenuInputAction::kLongPress:
          break;

        default:
          break;
      }
    }
};
