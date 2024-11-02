#include "loop_menu.h"

void LoopOrderMenu::update() {
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
    }
  }

  m_display->clear();
  m_display->renderHeader("Loops order");
  m_display->renderLoopOrderList(loopIndexes, loopOrders, loopStates, loopsCount, m_selectedLoop, m_swappingMode);
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
  switch (t_action) {
    case MenuInputAction::kDown:
      if (m_selectedLoop > 0) {
        m_selectedLoop--;
      }
      break;

    case MenuInputAction::kUp:
      if (m_selectedLoop < m_currentPreset->getLoopsCount()) {
        m_selectedLoop++;
      }
      break;

    case MenuInputAction::kPress:
      if (m_swappingMode) {
        m_swapRequested = true;  // Flag that a swap has been requested
        m_targetLoop = m_selectedLoop;
        m_swappingMode = false;  // Exit swapping mode after setting the flag
      }
      else {
        if (m_selectedLoop < m_currentPreset->getLoopsCount() - 1)
        {
          m_toggleRequested = true; // Flag that a toggle has been requested
        }
        else {
          m_goBackRequested = true; // Out of bounds so go back
        }
      }
      break;

    case MenuInputAction::kLongPress:
      m_sourceLoop = m_selectedLoop;
      m_swappingMode = true;  // Enter swapping mode
      break;

    default:
      break;
    }
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