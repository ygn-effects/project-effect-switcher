#include "hardware.h"

MemoryManager memoryManager(0);
PresetManager presetManager(memoryManager);

Encoder menuEncoder(12, 13);
MomentarySwitch menuEncoderSwitch(14, 1000);

MomentarySwitch menuEditSwitch(11, 1000);
Led menuEditSwitchLed(10);

MomentarySwitch footSwitch0(24, 1000);
MomentarySwitch footSwitch1(25, 1000);
MomentarySwitch footSwitch2(26);
MomentarySwitch footSwitch3(27);
MomentarySwitch footSwitch4(28);
MomentarySwitch footSwitch5(29);

// LedDriver16 presetLed(1);

// SwitchMatrix matrix(2);

Preset presetBank[c_maxPresets];

MenuManager menuManager;
DisplayManager displayManager(128, 64);
LayoutManager layoutManager(&displayManager);
HomeMenu homeMenu(&displayManager, &layoutManager, nullptr);
const char* settingsItems[] = {"Loops Order", "MIDI", "Expression", "System"};
ListMenu settingsMenu(&displayManager, &layoutManager, settingsItems, 4);
LoopOrderMenu loopsMenu(&displayManager, &layoutManager, nullptr);
MidiMessageMenu midiMessagesMenu(&displayManager, &layoutManager, nullptr);
MidiMessageEditMenu midiMessageEditMenu(&displayManager, &layoutManager, nullptr, 0);

void Hardware::pollSwitch(MomentarySwitch& t_switch, bool& t_pressFlag) {
  t_switch.poll();

  t_pressFlag = t_switch.isPushed();
}

void Hardware::pollSwitch(MomentarySwitch& t_switch, bool& t_pressFlag, bool& t_longPressFlag) {
  t_switch.poll();

  t_pressFlag = t_switch.isPushed();
  t_longPressFlag = t_switch.isLongPress();
}

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

  pollSwitch(menuEncoderSwitch, m_menuEncoderSwitchPress, m_menuEncoderSwitchLongPress);
}

void Hardware::pollMenuEditSwitch() {
  pollSwitch(menuEditSwitch, m_menuEditSwitchPress, m_menuEditSwitchLongPress);
}

void Hardware::pollFootSwitches() {
  pollSwitch(footSwitch0, m_footSwitch0Press, m_footSwitch0LongPress);
  pollSwitch(footSwitch1, m_footSwitch1Press, m_footSwitch1LongPress);
  pollSwitch(footSwitch2, m_footSwitch2Press, m_footSwitch2LongPress);
  pollSwitch(footSwitch3, m_footSwitch3Press, m_footSwitch3LongPress);
  pollSwitch(footSwitch4, m_footSwitch4Press, m_footSwitch4LongPress);
  pollSwitch(footSwitch5, m_footSwitch5Press, m_footSwitch5LongPress);
}

void Hardware::processFootSwitchAction(uint8_t t_footSwitch, bool t_longPress) {
  if (t_longPress) {

  }
  else {
    switch (presetManager.getFootSwitchMode(t_footSwitch))
    {
      case FootSwitchMode::kNone:
        break;

      case FootSwitchMode::kToggleLoop:
        break;

      case FootSwitchMode::kSendMidiMessage:
        break;

      case FootSwitchMode::kBankSelect:
        if (presetManager.getFootSwitchTargetBank(t_footSwitch) == 1) {
          // Up
          presetManager.setPresetBankUp();
          homeMenu.setCurrentPreset(presetManager.getCurrentPreset());
          menuManager.update();
        }
        else {
          // Down
          presetManager.setPresetBankDown();
          homeMenu.setCurrentPreset(presetManager.getCurrentPreset());
          menuManager.update();
        }
        break;

      case FootSwitchMode::kPresetSelect:
        presetManager.setCurrentPreset(presetManager.getFootSwitchTargetPreset(t_footSwitch));
        homeMenu.setCurrentPreset(presetManager.getCurrentPreset());
        menuManager.update();
        break;

      case FootSwitchMode::kMute:
        break;

      default:
        break;
    }
  }
}

void Hardware::transitionToState(SystemState t_newState) {
  m_systemState = t_newState;

  if (t_newState == SystemState::kPresetState) {
    settingsMenu.reset();
    loopsMenu.reset();
    midiMessagesMenu.reset();
    midiMessageEditMenu.reset();
    menuManager.setMenu(&homeMenu);
    menuManager.update();
  }
  else if (t_newState == SystemState::kSettingsState) {
    menuManager.reset();
    menuManager.setMenu(&settingsMenu);
    menuManager.update();
  }
  else if (t_newState == SystemState::kLoopsEditState) {
    menuManager.setMenu(&loopsMenu);
    menuManager.reset();
    loopsMenu.setCurrentPreset(presetManager.getCurrentPreset());
    menuManager.update();
  }
  else if (t_newState == SystemState::kMidiMessagesState) {
    menuManager.setMenu(&midiMessagesMenu);
    menuManager.reset();
    midiMessagesMenu.setCurrentPreset(presetManager.getCurrentPreset());
    menuManager.update();
  }
  else if (t_newState == SystemState::kMidiMessageEditState) {
    menuManager.setMenu(&midiMessageEditMenu);
    menuManager.reset();
    midiMessageEditMenu.setCurrentPreset(presetManager.getCurrentPreset());
    midiMessageEditMenu.setMidiMessageIndex(midiMessagesMenu.getStartIndex() + midiMessagesMenu.getSelectedIndex());
    midiMessageEditMenu.setMessageEditMode(true);
    menuManager.update();
  }
  else if (t_newState == SystemState::kMidiMessageAddState) {
    menuManager.setMenu(&midiMessageEditMenu);
    menuManager.reset();
    midiMessageEditMenu.setCurrentPreset(presetManager.getCurrentPreset());
    midiMessageEditMenu.setMidiMessageIndex(midiMessagesMenu.getStartIndex() + midiMessagesMenu.getSelectedIndex());
    midiMessageEditMenu.setMessageEditMode(false);
    menuManager.update();
  }
}

void Hardware::processMenuInput() {
  if (m_menuEncoderMoveLeft) {
    menuManager.handleInput(MenuInputAction::kUp);
    menuManager.update();
  }

  if (m_menuEncoderMoveRight) {
    menuManager.handleInput(MenuInputAction::kDown);
    menuManager.update();
  }

  if (m_menuEncoderSwitchPress) {
    menuManager.handleInput(MenuInputAction::kPress);
    menuManager.update();
  }

  if (m_menuEncoderSwitchLongPress) {
    menuManager.handleInput(MenuInputAction::kLongPress);
    menuManager.update();
  }
}

void Hardware::processPresetState() {
  if (m_footSwitch0Press) {
    processFootSwitchAction(0);
  }

  if (m_footSwitch1Press) {
    processFootSwitchAction(1);
  }

  if (m_footSwitch2Press) {
    processFootSwitchAction(2);
  }

  if (m_footSwitch3Press) {
    processFootSwitchAction(3);
  }

  if (m_footSwitch4Press) {
    processFootSwitchAction(4);
  }

  if (m_footSwitch5Press) {
    processFootSwitchAction(5);
  }

  if (m_menuEditSwitchPress) {
    transitionToState(kSettingsState);
  }
}

void Hardware::processSettingsState() {
  processMenuInput();

  if (m_menuEditSwitchLongPress) {
    transitionToState(kPresetState);
  }

  if (settingsMenu.isItemSelected()) {
    switch (settingsMenu.getSelectedIndex()) {
      case 0:
        transitionToState(kLoopsEditState);
        break;

      case 1:
        transitionToState(kMidiMessagesState);
        break;

      case 2:
        break;

      case 3:
        break;

      case 4:
        break;

      default:
        break;
    }
  }
}

void Hardware::processLoopsEditState() {
  processMenuInput();

  if (loopsMenu.isToggleRequested()) {
    presetManager.toggleLoopState(presetManager.getLoopByOrder(loopsMenu.getSelectedLoop()));
    presetManager.saveCurrentPreset();

    menuManager.update();
  }

  if (loopsMenu.isSwapRequested()) {
    presetManager.swapLoops(presetManager.getLoopByOrder(loopsMenu.getSourceLoop()), presetManager.getLoopByOrder(loopsMenu.getTargetLoop()));
    presetManager.saveCurrentPreset();

    menuManager.update();
  }

  if (loopsMenu.isGoBackRequested()) {
    transitionToState(kSettingsState);
  }

  if (m_menuEditSwitchLongPress) {
    transitionToState(kPresetState);
  }
}

void Hardware::processMidiMessagesState() {
  processMenuInput();

  if (midiMessagesMenu.isEditRequested()) {
    transitionToState(kMidiMessageEditState);
  }

  if (midiMessagesMenu.isAddRequested()) {
    transitionToState(kMidiMessageAddState);
  }

  if (midiMessagesMenu.isGoBackRequested()) {
    transitionToState(kSettingsState);
  }

  if (m_menuEditSwitchLongPress) {
    transitionToState(kPresetState);
  }
}

void Hardware::processMidiMessageEditState() {
  processMenuInput();

  if (midiMessageEditMenu.isCancelRequested()) {
    transitionToState(kMidiMessagesState);
  }

  if (midiMessageEditMenu.isDeleteRequested()) {
    presetManager.removeMidiMessage(midiMessageEditMenu.getMidiMessageIndex());
    presetManager.saveCurrentPreset();
    transitionToState(kMidiMessagesState);
  }

  if(midiMessageEditMenu.isSaveRequested()) {
    presetManager.setMidiMessageValues(midiMessageEditMenu.getMidiMessageIndex() ,midiMessageEditMenu.getNewMessageType(), midiMessageEditMenu.getNewMessageChannel(), midiMessageEditMenu.getNewMessageDataByte1(), midiMessageEditMenu.getNewMessageDataByte2(), midiMessageEditMenu.getNewMessageHasDataByte2());
    presetManager.saveCurrentPreset();
    transitionToState(kMidiMessagesState);
  }

  if (midiMessageEditMenu.isAddRequested()) {
    presetManager.addMidiMessage(midiMessageEditMenu.getNewMessageType(), midiMessageEditMenu.getNewMessageChannel(), midiMessageEditMenu.getNewMessageDataByte1(), midiMessageEditMenu.getNewMessageDataByte2(), midiMessageEditMenu.getNewMessageHasDataByte2());
    presetManager.saveCurrentPreset();
    transitionToState(kMidiMessagesState);
  }

  if (m_menuEditSwitchLongPress) {
    transitionToState(kPresetState);
  }
}


void Hardware::setup() {
  delay(500);
  displayManager.setup();
  menuEncoder.setup();
  menuEncoderSwitch.setup();
  menuEditSwitch.setup();
  menuEditSwitchLed.setup();
  footSwitch0.setup();
  footSwitch1.setup();
  footSwitch2.setup();
  footSwitch3.setup();
  footSwitch4.setup();
  footSwitch5.setup();
  // presetLed.setup();
  // matrix.switchMatrixSetup();
  menuManager.setMenu(&settingsMenu);

  // Careful
  //  memoryManager.initializeTestData();
  // Careful

  delay(100);
}

void Hardware::startup() {
  menuEncoderSwitch.poll();
  menuEncoder.poll();
  menuEditSwitch.poll();
  footSwitch0.poll();
  footSwitch1.poll();
  footSwitch2.poll();
  footSwitch3.poll();
  footSwitch4.poll();
  footSwitch5.poll();

  // Careful
  //  memoryManager.readTestData();
  //Careful

  presetManager.initialize();
  delay(200);
  menuManager.update();
  delay(100);
}

void Hardware::poll() {
  switch (m_systemState) {
    case kPresetState:
      pollMenuEditSwitch();
      pollFootSwitches();
      break;

    case kSettingsState:
    case kLoopsEditState:
    case kMidiMessagesState:
    case kMidiMessageAddState:
    case kMidiMessageEditState:
      pollMenuEditSwitch();
      pollMenuEncoder();
      break;

    default:
      break;
  }
}

void Hardware::process() {
  switch (m_systemState) {
    case kPresetState:
      processPresetState();
      break;

    case kSettingsState:
      processSettingsState();
      break;

    case kLoopsEditState:
      processLoopsEditState();
      break;

    case kMidiMessagesState:
      processMidiMessagesState();
      break;

    case kMidiMessageAddState:
    case kMidiMessageEditState:
      processMidiMessageEditState();

    default:
      break;
  }
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
  m_footSwitch0Press = false;
  m_footSwitch0LongPress = false;
  m_footSwitch1Press = false;
  m_footSwitch1LongPress = false;
  m_footSwitch2Press = false;
  m_footSwitch2LongPress = false;
  m_footSwitch3Press = false;
  m_footSwitch3LongPress = false;
  m_footSwitch4Press = false;
  m_footSwitch4LongPress = false;
  m_footSwitch5Press = false;
  m_footSwitch5LongPress = false;

  // Reset the internal state flags of the switches
  menuEncoderSwitch.reset();
  menuEditSwitch.reset();
  footSwitch0.reset();
  footSwitch1.reset();
  footSwitch2.reset();
  footSwitch3.reset();
  footSwitch4.reset();
  footSwitch5.reset();
}

