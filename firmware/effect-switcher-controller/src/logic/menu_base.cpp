#include "logic/menu_base.h"

void MenuBase::navigateMenuUp() {
  // Check if it's the last row and column and need to enter the footer
  if (m_selectedRow == m_itemsCount - 1 && m_selectedColumn == m_rowColumnCounts[m_selectedRow] - 1) {
    enterFooterNavigation();
    return;
  }

  // Move within the current row or to the next row
  if (m_selectedColumn == m_rowColumnCounts[m_selectedRow] - 1) {
    m_selectedRow++;
    m_selectedColumn = 0;
  } else {
    m_selectedColumn++;
  }

  // Scroll if we move past the visible rows
  if (m_selectedRow >= m_startIndex + m_visibleRowCount) {
    m_startIndex++;
  }
}

void MenuBase::navigateMenuDown() {
  // Move within the current row or to the previous row
  if (m_selectedColumn == 0) {
      if (m_selectedRow > 0) {
        m_selectedRow--;
        m_selectedColumn = m_rowColumnCounts[m_selectedRow] - 1;
      }
  } else {
    m_selectedColumn--;
  }

  // Scroll if we move past the visible rows
  if (m_selectedRow < m_startIndex) {
    m_startIndex--;
  }
}

void MenuBase::navigateFooterUp() {
  if (m_selectedColumn < m_footerColumnCount - 1) {
    m_selectedColumn++;
  }
}

void MenuBase::navigateFooterDown() {
  if (m_selectedColumn > 0) {
    m_selectedColumn--;
  } else {
    exitFooterNavigation();
  }
}

void MenuBase::enterFooterNavigation() {
  m_isFooterActive = true;
  m_layoutManager->setIsFooterActive(true);
  m_selectedColumn = 0;  // Start at the first footer column
}

void MenuBase::exitFooterNavigation() {
  m_isFooterActive = false;
  m_layoutManager->setIsFooterActive(false);
  m_selectedRow = m_itemsCount - 1;
  m_selectedColumn = m_rowColumnCounts[m_selectedRow] - 1;  // Last column of the last row
}

void MenuBase::setPresetView(PresetView* t_view) {
  m_presetView = t_view;
}

uint8_t MenuBase::getSelectedItem() {
  return m_startIndex + m_selectedRow;
}

void MenuBase::handleNavigation(MenuInputAction t_action) {
  switch (t_action) {
    case MenuInputAction::kUp:
      if (m_isFooterActive) {
        navigateFooterUp();
      } else {
        navigateMenuUp();
      }
      break;

    case MenuInputAction::kDown:
      if (m_isFooterActive) {
        navigateFooterDown();
      } else {
        navigateMenuDown();
      }
      break;

    case MenuInputAction::kPress:
    case MenuInputAction::kLongPress:
    default:
        break;
  }
}