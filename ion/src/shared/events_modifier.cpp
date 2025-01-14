#include "events_modifier.h"
#include <assert.h>

namespace Ion {
namespace Events {

static ShiftAlphaStatus sShiftAlphaStatus = ShiftAlphaStatus::Default;
static int sLongRepetition = 1;
int sEventRepetitionCount = 0;

ShiftAlphaStatus shiftAlphaStatus() {
  return sShiftAlphaStatus;
}

void removeShift() {
  if (sShiftAlphaStatus == ShiftAlphaStatus::Shift) {
    sShiftAlphaStatus = ShiftAlphaStatus::Default;
  } else if (sShiftAlphaStatus == ShiftAlphaStatus::ShiftAlpha ) {
    sShiftAlphaStatus = ShiftAlphaStatus::Alpha;
  } else if (sShiftAlphaStatus == ShiftAlphaStatus::ShiftAlphaLock) {
    sShiftAlphaStatus = ShiftAlphaStatus::AlphaLock;
  }
}

bool isShiftActive() {
  return sShiftAlphaStatus == ShiftAlphaStatus::Shift || sShiftAlphaStatus == ShiftAlphaStatus::ShiftAlpha || sShiftAlphaStatus == ShiftAlphaStatus::ShiftAlphaLock;
}

bool isAlphaActive() {
  return sShiftAlphaStatus == ShiftAlphaStatus::Alpha || sShiftAlphaStatus == ShiftAlphaStatus::ShiftAlpha || sShiftAlphaStatus == ShiftAlphaStatus::AlphaLock || sShiftAlphaStatus == ShiftAlphaStatus::ShiftAlphaLock;
}

bool isLockActive() {
  return sShiftAlphaStatus == ShiftAlphaStatus::AlphaLock || sShiftAlphaStatus == ShiftAlphaStatus::ShiftAlphaLock;
}

void setLongRepetition(int longRepetition) {
 sLongRepetition = longRepetition;
}

int repetitionFactor() {
  return sLongRepetition;
};

void setShiftAlphaStatus(ShiftAlphaStatus s) {
  sShiftAlphaStatus = s;
}

static void ComputeAndSetRepetionFactor(int eventRepetitionCount) {
  // The Repetition factor is increased by 4 every 20 loops in getEvent(2 sec)
  setLongRepetition((eventRepetitionCount / 20) * 4 + 1);
}

void resetLongRepetition() {
  sEventRepetitionCount = 0;
  ComputeAndSetRepetionFactor(sEventRepetitionCount);
}

void incrementRepetitionFactor() {
  sEventRepetitionCount++;
  ComputeAndSetRepetionFactor(sEventRepetitionCount);
}

void updateModifiersFromEvent(Event e) {
  assert(e.isKeyboardEvent());
  switch (sShiftAlphaStatus) {
    case ShiftAlphaStatus::Default:
      if (e == Shift) {
        sShiftAlphaStatus = ShiftAlphaStatus::Shift;
      } else if (e == Alpha) {
        sShiftAlphaStatus = ShiftAlphaStatus::Alpha;
      }
      break;
    case ShiftAlphaStatus::Shift:
      if (e == Shift) {
        sShiftAlphaStatus = ShiftAlphaStatus::Default;
      } else if (e == Alpha) {
        sShiftAlphaStatus = ShiftAlphaStatus::ShiftAlpha;
      } else {
        sShiftAlphaStatus = ShiftAlphaStatus::Default;
      }
      break;
    case ShiftAlphaStatus::Alpha:
      if (e == Shift) {
        sShiftAlphaStatus = ShiftAlphaStatus::ShiftAlpha;
      } else if (e == Alpha) {
        sShiftAlphaStatus = ShiftAlphaStatus::AlphaLock;
      } else {
        sShiftAlphaStatus = ShiftAlphaStatus::Default;
      }
      break;
    case ShiftAlphaStatus::ShiftAlpha:
      if (e == Shift) {
        sShiftAlphaStatus = ShiftAlphaStatus::Alpha;
      } else if (e == Alpha) {
        sShiftAlphaStatus = ShiftAlphaStatus::ShiftAlphaLock;
      } else {
        sShiftAlphaStatus = ShiftAlphaStatus::Default;
      }
      break;
    case ShiftAlphaStatus::AlphaLock:
      if (e == Shift) {
        sShiftAlphaStatus = ShiftAlphaStatus::ShiftAlphaLock;
      } else if (e == Alpha) {
        sShiftAlphaStatus = ShiftAlphaStatus::Default;
      }
      break;
    case ShiftAlphaStatus::ShiftAlphaLock:
      if (e == Shift) {
        sShiftAlphaStatus = ShiftAlphaStatus::AlphaLock;
      } else if (e == Alpha) {
        sShiftAlphaStatus = ShiftAlphaStatus::Default;
      }
      break;
  }
}

}
}
