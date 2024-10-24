#include "hardware.h"

// Memory mem(0);

Encoder menuEncoder(12, 13);
MomentarySwitch menuEncoderSwitch(14, 1000);
MomentarySwitch menuEditSwitch(11, 1000);
Led menuEditSwitchLed(10);

// MomentarySwitch presetBankUpFsw(24, 1000);
// MomentarySwitch presetBankDownFsw(25, 1000);
// MomentarySwitch preset0Fsw(26, 1000);
// MomentarySwitch preset1Fsw(27, 1000);
// MomentarySwitch preset2Fsw(28, 1000);
// MomentarySwitch preset3Fsw(29, 1000);

// LedDriver16 presetLed(1);

// SwitchMatrix matrix(2);

Preset presetBank[c_maxPresets];

// Display display(128, 64, 9, 1);  // Example values
// MenuManager menuManager;
// HomeMenu homeMenu(&display, 'A', 1);

void Hardware::pollMenuEncoder() {
  if (menuEncoder.poll()) {
    m_menuEncoderMove = true;

    if (menuEncoder.isMovedRight()) {
      m_menuEncoderMoveRight = true;
    }
    else {
      m_menuEncoderMoveLeft = true;
    }
  }

  menuEncoderSwitch.poll();

  if (menuEncoderSwitch.isSwitched()) {
    if (menuEncoderSwitch.isPushed()) {
      m_menuEncoderSwitchPress = true;
    }

    if (menuEncoderSwitch.isLongPress()) {
      m_menuEncoderSwitchLongPress = true;
    }
  }
}

void Hardware::pollMenuEditSwitch() {
  menuEditSwitch.poll();

  if (menuEditSwitch.isSwitched()) {
    if (menuEditSwitch.isPushed()) {
      m_menuEditSwitchPress = true;
    }

    if (menuEditSwitch.isLongPress()) {
      m_menuEditSwitchLongPress = true;
    }
  }
}

void Hardware::pollFootSwitches() {
  // presetBankUpFsw.poll();
  // presetBankDownFsw.poll();
  // preset0Fsw.poll();
  // preset1Fsw.poll();
  // preset2Fsw.poll();
  // preset3Fsw.poll();
}

void Hardware::setup()
{
  Serial.begin(115200);
  delay(500);
  // display.setup();
  // mem.memorySetup();
  menuEncoder.setup();
  menuEncoderSwitch.setup();
  menuEditSwitch.setup();
  menuEditSwitchLed.setup();
  // presetUpFsw.setup();
  // presetDownFsw.setup();
  // preset0Fsw.setup();
  // preset1Fsw.setup();
  // preset2Fsw.setup();
  // preset3Fsw.setup();
  // presetLed.setup();
  // matrix.switchMatrixSetup();

  //mem.memoryReset();
  // menuManager.setMenu(&homeMenu);
  // menuManager.update();
}

void Hardware::startup()
{
  menuEncoderSwitch.poll();
  menuEncoder.poll();
  menuEditSwitch.poll();
  // presetBankUpFsw.poll();
  // presetBankDownFsw.poll();
  // preset0Fsw.poll();
  // preset1Fsw.poll();
  // preset2Fsw.poll();
  // preset3Fsw.poll();
}

void Hardware::poll() {
  switch (m_systemState)
  {
    case kPresetState:
      pollMenuEditSwitch();
      pollFootSwitches();

      break;

    case kEditMenuState:
      pollMenuEditSwitch();
      pollMenuEncoder();
      pollMenuEditSwitch();

      break;

    default:
      break;
  }
}

void Hardware::restoreLastState()
{

}

void Hardware::resetTriggers()
{
  m_menuEncoderMove = false;
  m_menuEncoderMoveRight = false;
  m_menuEncoderMoveLeft = false;
  m_menuEncoderSwitchPress = false;
  m_menuEncoderSwitchLongPress = false;
  m_menuEditSwitchPress = false;
  m_menuEncoderSwitchLongPress = false;
  m_presetBankUpFswPress = false;
  m_presetBankUpFswLongPress = false;
  m_presetBankDownFswPress = false;
  m_presetBankDownFswLongPress = false;
  m_preset0FswPress = false;
  m_preset1FswPress = false;
  m_preset2FswPress = false;
  m_preset3FswPress = false;
}

void Hardware::loadPresetBank()
{

}

void Hardware::loadPreset()
{

}

void Hardware::savePreset()
{

}
