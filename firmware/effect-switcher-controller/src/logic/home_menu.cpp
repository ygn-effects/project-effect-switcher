#include "home_menu.h"

void HomeMenu::update() {
  m_layoutManager->clear();

  m_layoutManager->setHeader("Current preset");

  char text[4];
  text[0] = 'A' + m_currentPreset->getBank();
  text[1] = '-';
  text[2] = m_currentPreset->getPreset();
  text[3] = '\0';

  Row row;
  row.alignment = Row::kFullScreen;
  row.columnsCount = 1;
  row.columns[0] = { Column::kLabel, Column::kNormal, text, 0, 0 };

  m_layoutManager->addRow(row);
  m_layoutManager->render();
}

void HomeMenu::setCurrentPreset(const Preset* t_currentPreset) {
  m_currentPreset = t_currentPreset;
}

void HomeMenu::reset() {

}

void HomeMenu::handleInput(MenuInputAction t_action) {

}
