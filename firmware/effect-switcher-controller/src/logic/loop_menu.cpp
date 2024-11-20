#include "loop_menu.h"

void LoopOrderMenu::update() {

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