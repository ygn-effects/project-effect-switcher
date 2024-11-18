#include "list_menu.h"
#include "utils/logging.h"

void ListMenu::update() {
  m_layoutManager->clear();

  // Set header
  m_layoutManager->setHeader("Settings");

  // Set footer
  const char* footItems[] = {"Back"};
  m_layoutManager->setFooter(footItems, 1);

  // Visible Content Rows
  uint8_t endIndex = m_startIndex + m_visibleRowCount;
  if (endIndex > m_itemCount) {
    endIndex = m_itemCount;
  }

  for (uint8_t i = m_startIndex; i < endIndex; i++) {
    uint8_t rowIndex = 0;

    Row row;
    row.alignment = Row::kLeft;
    row.columnsCount = 1;
    row.columns[0] = { Column::kLabel, Column::kNormal, m_menuItems[i], 0, 0 };

    m_layoutManager->addRow(row);

    m_rowCounts[rowIndex] = i;
    m_rowColumnCounts[rowIndex] = row.columnsCount;
    rowIndex++;
  }

  LOG_DEBUG("Selected row %d:", m_selectedRow);
  LOG_DEBUG("Selected column %d:", m_selectedColumn);

  m_layoutManager->setActiveRow(m_selectedRow);
  m_layoutManager->setActiveColumn(m_selectedColumn);
  m_layoutManager->render();
}

void ListMenu::reset() {
  m_selectedRow = 0;
  m_selectedColumn = 0;
  m_startIndex = 0;
  m_itemSelected = false;
}

void ListMenu::handleInput(MenuInputAction t_action) {
  switch (t_action) {
    case MenuInputAction::kUp: {
      if (m_selectedRow > 0) {
        if (m_selectedRow == m_startIndex) {
          // Scroll up when the selected row is at the top of the visible area
          m_startIndex--;
        } else {
          // Move the selection up within the visible rows
          m_selectedRow--;
        }

        // Ensure the selected column is within bounds for the new row
        if (m_rowColumnCounts[m_selectedRow] == 0) {
          m_selectedColumn = 0;
        } else if (m_selectedColumn >= m_rowColumnCounts[m_selectedRow]) {
          m_selectedColumn = m_rowColumnCounts[m_selectedRow] - 1;
        }
      }
      break;
    }

    case MenuInputAction::kDown: {
      if (m_selectedRow < m_itemCount - 1) {
        if (m_selectedRow == m_startIndex + m_visibleRowCount - 1) {
          // Scroll down when the selected row is at the bottom of the visible area
          m_startIndex++;
        } else if (m_selectedRow + m_startIndex < m_itemCount - 1) {
          // Move the selection down within the visible rows
          m_selectedRow++;
        }

        // Ensure the selected column is within bounds for the new row
        if (m_rowColumnCounts[m_selectedRow] == 0) {
          m_selectedColumn = 0;
        } else if (m_selectedColumn >= m_rowColumnCounts[m_selectedRow]) {
          m_selectedColumn = m_rowColumnCounts[m_selectedRow] - 1;
        }
      }
      break;
    }

    case MenuInputAction::kPress:
      m_itemSelected = true;
      break;

    default:
      break;
  }
}

uint8_t ListMenu::getSelectedIndex() {
  return m_startIndex + m_selectedRow;
}

bool ListMenu::isItemSelected() {
  return m_itemSelected;
}
