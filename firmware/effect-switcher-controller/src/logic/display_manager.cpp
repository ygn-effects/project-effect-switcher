#include "display_manager.h"

void DisplayManager::setup() {
  m_st7789.init(m_height, m_width);
  m_st7789.setRotation(1);
}

void DisplayManager::reset() {
  m_st7789.fillScreen(ST77XX_BLACK);
  m_st7789.setFont();
  m_st7789.setTextColor(ST77XX_WHITE);
}

void DisplayManager::render() {

}

uint16_t DisplayManager::calcTextWidth(const char* t_text) {
  int16_t x1, y1;
  uint16_t textWidth, height;
  m_st7789.getTextBounds(t_text, 0, 0, &x1, &y1, &textWidth, &height);

  return textWidth;
}

uint16_t DisplayManager::calcTextHeight(const char* t_text) {
  int16_t x1, y1;
  uint16_t textWidth, height;
  m_st7789.getTextBounds(t_text, 0, 0, &x1, &y1, &textWidth, &height);

  return height;
}

uint16_t DisplayManager::getWidth() {
  return m_width;
}

uint16_t DisplayManager::getHeight() {
  return m_height;
}

uint8_t DisplayManager::getNewLine() {
  return m_newLine;
}

uint8_t DisplayManager::getNewTab() {
  return m_newTab;
}

uint8_t DisplayManager::getMaxVisibleLines() {
  return m_height / m_newLine;
}

void DisplayManager::printItem(const char* t_text, uint16_t t_x, uint16_t t_y) {
  m_st7789.setCursor(t_x, t_y);
  m_st7789.write(t_text);
}

void DisplayManager::printFullScreenItem(const char* t_text, uint16_t t_usableHeight, uint16_t t_usableWidth) {
  for (uint8_t fontSize = 16; fontSize >= 0; fontSize -= 2) {
    m_st7789.setTextSize(fontSize);

    uint16_t textHeight = calcTextHeight(t_text);
    uint16_t textWidth = calcTextWidth(t_text);

    if (textHeight <= t_usableHeight && textWidth <= t_usableWidth) {
      uint16_t xPosition = (m_height - textHeight) / 2;
      uint16_t yPosition = (m_width - textWidth) / 2;

      printItem(t_text, xPosition, yPosition);
      m_st7789.setTextSize(m_fontSize);
      return;
    }
  }
}

void DisplayManager::printHighlightedItem(const char* t_text, uint16_t t_x, uint16_t t_y) {
  m_st7789.setTextColor(ST77XX_BLACK, ST77XX_WHITE);
  printItem(t_text, t_x, t_y);
  m_st7789.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
}

void DisplayManager::drawInvertedLine(uint8_t t_y) {
  m_st7789.fillRect(0, 0, m_width, m_newLine, ST77XX_WHITE);
  m_st7789.setTextColor(ST77XX_WHITE);
}