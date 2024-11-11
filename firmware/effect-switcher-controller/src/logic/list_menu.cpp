#include "list_menu.h"

void ListMenu::update() {
  m_display->clear();
  m_display->renderHeader("Settings");
  m_display->renderListMenu(m_menuItems, m_itemCount, m_startIndex, m_selectedIndex);
}

void ListMenu::reset() {
  m_selectedIndex = 0;
  m_startIndex = 0;
  m_itemSelected = false;
}

void ListMenu::handleInput(MenuInputAction t_action) {
  switch (t_action) {
    case MenuInputAction::kUp:
      if (m_selectedIndex > 0) {
        m_selectedIndex--;
        if (m_selectedIndex < m_startIndex) {
          m_startIndex--;
        }
      }
      break;

    case MenuInputAction::kDown:
      if (m_selectedIndex < m_itemCount - 1) {
        m_selectedIndex++;
        if (m_selectedIndex >= m_startIndex + m_display->getMaxVisibleLines()) {
          m_startIndex++;
        }
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

uint8_t ListMenu::getSelectedIndex() {
  m_itemSelected = false;
  return m_selectedIndex;
}

bool ListMenu::isItemSelected() {
  return m_itemSelected;
}