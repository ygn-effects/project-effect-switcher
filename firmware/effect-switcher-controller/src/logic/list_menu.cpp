#include "list_menu.h"
#include "utils/logging.h"

void ListMenu::update() {
  m_layoutManager->clear();

  // Set header
  m_layoutManager->setHeader("Settings");

  // Set footer
  m_footerColumnCount = 1;
  const char* footItems[] = {"Back"};
  m_layoutManager->setFooter(footItems, m_footerColumnCount);

  // Visible Content Rows
  uint8_t endIndex = m_startIndex + m_visibleRowCount;
  if (endIndex > m_itemsCount) {
    endIndex = m_itemsCount;
  }

  uint8_t rowIndex = 0;
  for (uint8_t i = m_startIndex; i < endIndex; i++) {

    Row row;
    row.alignment = Row::kLeft;
    row.columnsCount = 1;
    row.columns[0] = { Column::kLabel, Column::kNormal, m_menuItems[i], nullptr, 0 };

    m_layoutManager->addRow(row);

    m_rowCounts[rowIndex] = i;
    m_rowColumnCounts[rowIndex] = row.columnsCount;
    rowIndex++;
  }

  m_layoutManager->setActiveRow(m_selectedRow);
  m_layoutManager->setActiveColumn(m_selectedColumn);
  m_layoutManager->render();
}

void ListMenu::reset() {
  m_selectedRow = 0;
  m_selectedColumn = 0;
  m_startIndex = 0;
  m_isFooterActive = false;
  m_layoutManager->setIsFooterActive(false);
  m_layoutManager->setActiveRow(m_selectedRow);
  m_layoutManager->setActiveColumn(m_selectedColumn);
}

void ListMenu::handleAction(MenuInputAction t_action) {
  switch (t_action) {
    case MenuInputAction::kPress:
      if (m_isFooterActive) {
        if (m_selectedColumn == 0) {
          m_backrequested = true;
        }
      }
      else {
        m_itemSelected = true;
      }
      break;

    case MenuInputAction::kUp:
      handleNavigation(t_action);
      break;

    case MenuInputAction::kDown:
      handleNavigation(t_action);
      break;

    case MenuInputAction::kLongPress:
    default:
      break;
  }
}

bool ListMenu::isItemSelected() {
  bool selected = m_itemSelected;
  m_itemSelected = false;

  return selected;
}

bool ListMenu::isBackRequested() {
  bool requested = m_backrequested;
  m_backrequested = false;

  return requested;
}
