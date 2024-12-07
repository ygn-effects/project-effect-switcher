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
const char* settingsItems[] = {"Loops Order", "MIDI", "Expression", "FootSwitches", "System"};
ListMenu settingsMenu(&displayManager, &layoutManager, settingsItems, 5);
LoopOrderMenu loopsMenu(&displayManager, &layoutManager);
MidiMessageMenu midiMessagesMenu(&displayManager, &layoutManager);
MidiMessageEditMenu midiMessageEditMenu(&displayManager, &layoutManager, 0);
const char* footSwitchesItems[] = {"FootSwitch 0", "FootSwitch 1", "FootSwitch 2", "FootSwitch 3", "FootSwitch 4", "FootSwitch 5"};
ListMenu FootSwitchesMenu(&displayManager, &layoutManager, footSwitchesItems, 6);

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
          m_presetView = createPresetView(presetManager.getCurrentPreset());
          homeMenu.setCurrentPreset(presetManager.getCurrentPreset());
          menuManager.update();
        }
        else {
          // Down
          presetManager.setPresetBankDown();
          m_presetView = createPresetView(presetManager.getCurrentPreset());
          homeMenu.setCurrentPreset(presetManager.getCurrentPreset());
          menuManager.update();
        }
        break;

      case FootSwitchMode::kPresetSelect:
        presetManager.setCurrentPreset(presetManager.getFootSwitchTargetPreset(t_footSwitch));
        m_presetView = createPresetView(presetManager.getCurrentPreset());
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

  switch (t_newState) {
    case SystemState::kPresetState:
      settingsMenu.reset();
      loopsMenu.reset();
      midiMessagesMenu.reset();
      midiMessageEditMenu.reset();
      FootSwitchesMenu.reset();
      menuManager.setMenu(&homeMenu);
      menuManager.update();
      break;

    case SystemState::kSettingsState:
      menuManager.reset();
      menuManager.setMenu(&settingsMenu);
      menuManager.update();
      break;

    case SystemState::kLoopsEditState:
      menuManager.setMenu(&loopsMenu);
      menuManager.reset();
      m_presetView = createPresetView(presetManager.getCurrentPreset());
      loopsMenu.setPresetView(&m_presetView);
      menuManager.update();
      break;

    case SystemState::kMidiMessageEditState:
      menuManager.setMenu(&midiMessageEditMenu);
      menuManager.reset();
      m_presetView = createPresetView(presetManager.getCurrentPreset());
      midiMessageEditMenu.setPresetView(&m_presetView);
      midiMessageEditMenu.setMessageEditMode(true);
      midiMessageEditMenu.setMidiMessageIndex(midiMessagesMenu.getSelectedItem());
      menuManager.update();
      break;

    case SystemState::kMidiMessageAddState:
      menuManager.setMenu(&midiMessageEditMenu);
      menuManager.reset();
      m_presetView = createPresetView(presetManager.getCurrentPreset());
      midiMessageEditMenu.setPresetView(&m_presetView);
      midiMessageEditMenu.setMessageEditMode(false);
      menuManager.update();
      break;

    case SystemState::kFootSwitchesListState:
      menuManager.setMenu(&FootSwitchesMenu);
      menuManager.update();
      break;

    default:
      break;
  }
}

void Hardware::processMenuInput() {
  if (m_menuEncoderMoveLeft) {
    menuManager.handleAction(MenuInputAction::kUp);
    menuManager.update();
  }

  if (m_menuEncoderMoveRight) {
    menuManager.handleAction(MenuInputAction::kDown);
    menuManager.update();
  }

  if (m_menuEncoderSwitchPress) {
    menuManager.handleAction(MenuInputAction::kPress);
    menuManager.update();
  }

  if (m_menuEncoderSwitchLongPress) {
    menuManager.handleAction(MenuInputAction::kLongPress);
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
    switch (settingsMenu.getSelectedItem()) {
      case 0:
        transitionToState(kLoopsEditState);
        break;

      case 1:
        transitionToState(kMidiMessagesState);
        break;

      case 2:
        break;

      case 3:
        transitionToState(kFootSwitchesListState);
        break;

      case 4:
        break;

      default:
        break;
    }
  }

  if (settingsMenu.isBackRequested()) {
    transitionToState(kPresetState);
  }
}

void Hardware::processLoopsEditState() {
  processMenuInput();

  if (loopsMenu.isSaveRequested()) {
    applyPresetView(presetManager.getCurrentPreset());
    presetManager.saveCurrentPreset();
    transitionToState(kSettingsState);
  }

  if (loopsMenu.isBackRequested()) {
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

  if (midiMessagesMenu.isBackRequested()) {
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
    applyPresetView(presetManager.getCurrentPreset());
    presetManager.saveCurrentPreset();
    transitionToState(kMidiMessagesState);
  }

  if(midiMessageEditMenu.isSaveRequested()) {
    applyPresetView(presetManager.getCurrentPreset());
    presetManager.saveCurrentPreset();
    transitionToState(kMidiMessagesState);
  }

  if (midiMessageEditMenu.isAddRequested()) {
    applyPresetView(presetManager.getCurrentPreset());
    presetManager.saveCurrentPreset();
    transitionToState(kMidiMessagesState);
  }

  if (m_menuEditSwitchLongPress) {
    transitionToState(kPresetState);
  }
}

void Hardware::processFootSwitchesListState() {
  processMenuInput();

  if (m_menuEditSwitchLongPress) {
    transitionToState(kPresetState);
  }

  if (FootSwitchesMenu.isItemSelected()) {
    switch (FootSwitchesMenu.getSelectedItem()) {
      case 0:
        break;

      case 1:
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

  if (FootSwitchesMenu.isBackRequested()) {
    transitionToState(kSettingsState);
  }
}

PresetView Hardware::createPresetView(const Preset* t_preset) {
  PresetView view;

  view.loopsCount = t_preset->getLoopsCount();
  for (uint8_t i = 0; i < view.loopsCount; i++) {
    uint8_t index = t_preset->getLoopOrder(i);
    view.loops[index].index = i;
    view.loops[index].order = index;
    view.loops[index].isActive = t_preset->getLoopState(i);
  }

  view.midiMessagesCount = t_preset->getMidiMessagesCount();
  for (uint8_t i = 0; i < view.midiMessagesCount; i++) {
    view.midiMessages[i].type = t_preset->getMidiMessageType(i);
    view.midiMessages[i].channel = t_preset->getMidiMessageChannel(i);
    view.midiMessages[i].byte1 = t_preset->getMidiMessageDataByte1(i);
    view.midiMessages[i].byte2 = t_preset->getMidiMessageDataByte2(i);
    view.midiMessages[i].hasDataByte2 = t_preset->getMidiMessageHasDataByte2(i);
  }

  return view;
}

void Hardware::applyPresetView(Preset* t_preset) {
  for (uint8_t i = 0; i < m_presetView.loopsCount; i++) {
    const LoopView& loop = m_presetView.loops[i];

    t_preset->setLoopOrder(loop.index, loop.order);
    t_preset->setLoopState(loop.index, loop.isActive);
  }

  for (uint8_t i = 0; i < m_presetView.midiMessagesCount; i++) {
    const MidiMessageView& message = m_presetView.midiMessages[i];

    t_preset->setMidiMessageType(i, message.type);
    t_preset->setMidiMessageChannel(i, message.channel);
    t_preset->setMidiMessageDataByte1(i, message.byte1);
    t_preset->setMidiMessageDataByte2(i, message.byte2);
  }

  t_preset->setMidiMessagesCount(m_presetView.midiMessagesCount);
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
  menuManager.setMenu(&homeMenu);

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
  m_presetView = createPresetView(presetManager.getCurrentPreset());
  homeMenu.setCurrentPreset(presetManager.getCurrentPreset());
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
    case kFootSwitchesListState:
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

    case kFootSwitchesListState:
      processFootSwitchesListState();

    default:
      break;
  }
}

void Hardware::resetTriggers() {
  m_menuEncoderMove = false;
  m_menuEncoderMoveRight = false;
  m_menuEncoderMoveLeft = false;
  m_menuEncoderSwitchPress = false;
  m_menuEncoderSwitchLongPress = false;
  m_menuEditSwitchPress = false;
  m_menuEditSwitchLongPress = false;
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

