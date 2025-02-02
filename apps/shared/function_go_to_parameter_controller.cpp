#include "function_go_to_parameter_controller.h"
#include "function_app.h"
#include <assert.h>
#include <cmath>
#include <ion/display.h>

using namespace Escher;

namespace Shared {

FunctionGoToParameterController::FunctionGoToParameterController(Responder * parentResponder, InputEventHandlerDelegate * inputEventHandlerDelegate, InteractiveCurveViewRange * graphRange, CurveViewCursor * cursor) :
  GoToParameterController(parentResponder, inputEventHandlerDelegate, graphRange, cursor),
  m_record()
{
}

bool FunctionGoToParameterController::confirmParameterAtIndex(int parameterIndex, double f) {
  assert(parameterIndex == 0);
  FunctionApp * myApp = FunctionApp::app();
  ExpiringPointer<Function> function = myApp->functionStore()->modelForRecord(m_record);
  // If possible, round f so that we go to the evaluation of the displayed f
  double pixelWidth = (m_graphRange->xMax() - m_graphRange->xMin()) / Ion::Display::Width;
  f = FunctionBannerDelegate::getValueDisplayedOnBanner(f, myApp->localContext(), Poincare::Preferences::sharedPreferences()->numberOfSignificantDigits(), pixelWidth, false);

  Poincare::Coordinate2D<double> xy = function->evaluateXYAtParameter(f, myApp->localContext());
  m_cursor->moveTo(f, xy.x1(), xy.x2());
  m_graphRange->centerAxisAround(CurveViewRange::Axis::X, m_cursor->x());
  m_graphRange->centerAxisAround(CurveViewRange::Axis::Y, m_cursor->y());
  /* The range might have evolved to center around the cursor but we don't want
   * to reinit the cursor position when displaying the graph controller. To
   * prevent this, we update the snapshot range version in order to make the
   * graph controller as if the range has not evolved since last appearance. */
  uint32_t * snapshotRangeVersion = static_cast<FunctionApp::Snapshot *>(myApp->snapshot())->rangeVersion();
  *snapshotRangeVersion = m_graphRange->rangeChecksum();
  return true;
}

void FunctionGoToParameterController::setRecord(Ion::Storage::Record record) {
  m_record = record;
  FunctionApp * myApp = FunctionApp::app();
  ExpiringPointer<Function> function = myApp->functionStore()->modelForRecord(m_record);
  setParameterName(function->parameterMessageName());
}

}
