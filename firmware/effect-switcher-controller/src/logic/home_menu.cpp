#include "home_menu.h"

void HomeMenu::update() {
  m_display->clear();
  m_display->renderHeader("Current Preset");

  // Banks are stored as numbers with a start index of 0
  char bankLetter = 'A' + m_currentPreset->getBank();
  uint8_t presetNumber = m_currentPreset->getPreset();
  m_display->renderBankAndPreset(bankLetter, presetNumber);
}

void HomeMenu::setCurrentPreset(const Preset* t_currentPreset) {
  m_currentPreset = t_currentPreset;
}

void HomeMenu::reset() {

}

void HomeMenu::handleInput(MenuInputAction t_action) {

}