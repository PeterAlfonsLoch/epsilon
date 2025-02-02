#include "intersection_graph_controller.h"
#include "../../shared/poincare_helpers.h"
#include <poincare/preferences.h>

using namespace Shared;
using namespace Escher;

namespace Graph {

IntersectionGraphController::IntersectionGraphController(Responder * parentResponder, GraphView * graphView, BannerView * bannerView, Shared::InteractiveCurveViewRange * curveViewRange, CurveViewCursor * cursor) :
  CalculationGraphController(parentResponder, graphView, bannerView, curveViewRange, cursor, I18n::Message::NoIntersectionFound),
  m_intersectedRecord()
{
}

const char * IntersectionGraphController::title() {
  return I18n::translate(I18n::Message::Intersection);
}

void IntersectionGraphController::reloadBannerView() {
  assert(!m_intersectedRecord.isNull());
  CalculationGraphController::reloadBannerView();
  constexpr size_t bufferSize = FunctionBannerDelegate::k_textBufferSize;
  char buffer[bufferSize];
  const char * legend = "=";
  // 'f(x)=g(x)=', keep 2 chars for '='
  ExpiringPointer<ContinuousFunction> f = functionStore()->modelForRecord(m_record);
  int numberOfChar = f->nameWithArgument(buffer, bufferSize-2);
  assert(numberOfChar <= bufferSize);
  numberOfChar += strlcpy(buffer+numberOfChar, legend, bufferSize-numberOfChar);
  // keep 1 char for '=';
  ExpiringPointer<ContinuousFunction> g = functionStore()->modelForRecord(m_intersectedRecord);
  numberOfChar += g->nameWithArgument(buffer+numberOfChar, bufferSize-numberOfChar-1);
  assert(numberOfChar <= bufferSize);
  numberOfChar += strlcpy(buffer+numberOfChar, legend, bufferSize-numberOfChar);
  numberOfChar += PoincareHelpers::ConvertFloatToText<double>(m_cursor->y(), buffer+numberOfChar, bufferSize-numberOfChar, numberOfSignificantDigits());
  assert(numberOfChar < bufferSize);
  buffer[numberOfChar++] = '\0';
  bannerView()->ordinateView()->setText(buffer);
  bannerView()->reload();
}

Poincare::Coordinate2D<double> IntersectionGraphController::computeNewPointOfInterest(double start, double max, Poincare::Context * context, double relativePrecision, double minimalStep, double maximalStep) {
  // TODO The following three lines should be factored.
  Poincare::Coordinate2D<double> result = Poincare::Coordinate2D<double>(NAN, NAN);
  for (int i = 0; i < functionStore()->numberOfActiveFunctions(); i++) {
    Ion::Storage::Record record = functionStore()->activeRecordAtIndex(i);
    if (record != m_record) {
      ContinuousFunction f = *(functionStore()->modelForRecord(record));
      Poincare::Coordinate2D<double> intersection = functionStore()->modelForRecord(m_record)->nextIntersectionFrom(start, max, context, f.expressionReduced(context), relativePrecision, minimalStep, maximalStep, f.tMin(), f.tMax());
      if ((std::isnan(result.x1()) || std::fabs(intersection.x1()-start) < std::fabs(result.x1()-start)) && !std::isnan(intersection.x1())) {
        m_intersectedRecord = record;
        result = (std::isnan(result.x1()) || std::fabs(intersection.x1()-start) < std::fabs(result.x1()-start)) ? intersection : result;
      }
    }
  }
  return result;
}

}
