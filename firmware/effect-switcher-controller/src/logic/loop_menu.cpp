#include "loop_menu.h"

void LoopOrderMenu::update() {
  m_layoutManager->clear();

  // Set header
  m_layoutManager->setHeader("Loops Order");

  // Set footer
  const char* footItems[] = {"Cancel", "Save"};
  m_layoutManager->setFooter(footItems, 2);

  // Gather data from the preset
  const uint8_t loopsCount = m_currentPreset->getLoopsCount();
  uint8_t loopIndexes[loopsCount];
  uint8_t loopOrders[loopsCount];
  uint8_t loopStates[loopsCount];

  // Construct the arrays by loops order to send ordered data to the display
  for (uint8_t i = 0; i < loopsCount; i++) {
    const uint8_t loopIndex = m_currentPreset->getLoopIndexByOrder(i);

    if (loopIndex < loopsCount) {
      loopIndexes[i] = loopIndex;
      loopOrders[i] = i;
      loopStates[i] = m_currentPreset->getLoopState(loopIndex);
      LOG_DEBUG("Index %d:", loopIndexes[i]);
      LOG_DEBUG("Order %d:", loopOrders[i]);
      LOG_DEBUG("State %d:", loopStates[i]);
    }
  }

  char loopNumbers[loopsCount][2] = {0};

  uint8_t rowIndex = 0;
  for (uint8_t i = 0; i < loopsCount; i += LayoutConstants::c_maxColumnsPerRow) {
    uint8_t columnIndex = 0;
    Row row;
    row.alignment = Row::kJustify;

    for (uint8_t j = 0; j < LayoutConstants::c_maxColumnsPerRow; j++) {
      uint8_t loopIndex = i + j;

      if (loopIndex < loopsCount) {
        loopNumbers[loopIndex][0] = '0' + loopIndex;
        loopNumbers[loopIndex][1] = '\0';
        row.columns[j] = { Column::kLabel, Column::kNormal, loopNumbers[loopIndex], 0, 0 };
      } else {
        row.columns[j] = { Column::kLabel, Column::kNormal, "", 0, 0 };
      }
    }

    m_layoutManager->addRow(row);
    m_rowCounts[rowIndex] = rowIndex;
    m_rowColumnCounts[rowIndex] = columnIndex;
    rowIndex++;
  }

  m_layoutManager->setActiveRow(m_selectedRow);
  m_layoutManager->setActiveColumn(m_selectedColumn);
  m_layoutManager->render();
}

void LoopOrderMenu::reset() {
  m_swappingMode = false;
  m_toggleRequested = false;
  m_swapRequested = false;
  m_goBackRequested = false;
  m_sourceLoop = 0;
  m_targetLoop = 0;
  m_selectedLoop = 0;
}

void LoopOrderMenu::handleInput(MenuInputAction t_action) {

  }

void LoopOrderMenu::setCurrentPreset(Preset* t_currentPreset) {
  m_currentPreset = t_currentPreset;
}

bool LoopOrderMenu::isToggleRequested() {
  bool requested = m_toggleRequested;
  m_toggleRequested = false; // Reset after checking

  return requested;
}

bool LoopOrderMenu::isSwapRequested() {
  bool requested = m_swapRequested;
  m_swapRequested = false; // Reset after checking

  return requested;
}

bool LoopOrderMenu::isGoBackRequested() {
  bool requested = m_goBackRequested;
  m_goBackRequested = false;

  return requested;
}

uint8_t LoopOrderMenu::getSelectedLoop() const {
  return m_selectedLoop;
}

uint8_t LoopOrderMenu::getSourceLoop() const {
  return m_sourceLoop;
}

uint8_t LoopOrderMenu::getTargetLoop() const {
  return m_targetLoop;
}