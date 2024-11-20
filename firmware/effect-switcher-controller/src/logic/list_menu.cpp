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

  uint8_t rowIndex = 0;
  for (uint8_t i = m_startIndex; i < endIndex; i++) {

    Row row;
    row.alignment = Row::kLeft;
    row.columnsCount = 1;
    row.columns[0] = { Column::kLabel, Column::kNormal, m_menuItems[i], 0, 0 };

    m_layoutManager->addRow(row);

    m_rowCounts[rowIndex] = i;
    m_rowColumnCounts[rowIndex] = row.columnsCount;
    LOG_DEBUG("Row index %d:", rowIndex);
    LOG_DEBUG("Column count %d:", row.columnsCount);
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
      LOG_DEBUG("UP");
      if (m_selectedRow > 0) { // Check if we're not at the topmost row
        if (m_selectedColumn == 0) {
          // Move to the last column of the previous row
          m_selectedRow--;
          m_selectedColumn = m_rowColumnCounts[m_selectedRow] - 1; // Last column of the new row
        } else {
          // Move left within the same row
          m_selectedColumn--;
        }

        // Scroll up if necessary
        if (m_selectedRow < m_startIndex) {
          m_startIndex--;
        }
      }
      break;
    }

    case MenuInputAction::kDown: {
      LOG_DEBUG("DOWN");
      if (m_selectedRow + m_startIndex < m_itemCount - 1) { // Check if we're not at the bottommost row
        if (m_selectedColumn == m_rowColumnCounts[m_selectedRow] - 1) {
          // Move to the first column of the next row
          m_selectedRow++;
          m_selectedColumn = 0; // First column of the new row
        } else {
          // Move right within the same row
          m_selectedColumn++;
        }

        // Scroll down if necessary
        if (m_selectedRow >= m_startIndex + m_visibleRowCount) {
          m_startIndex++;
          m_selectedRow = m_visibleRowCount - 1;
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
