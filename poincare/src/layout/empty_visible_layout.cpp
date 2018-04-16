#include "empty_visible_layout.h"
#include "matrix_layout.h"
#include <poincare/expression_layout_cursor.h>
#include <escher/palette.h>
#include <assert.h>

namespace Poincare {

EmptyVisibleLayout::EmptyVisibleLayout(Color color, bool visible) :
  StaticLayoutHierarchy(),
  m_isVisible(visible),
  m_color(color)
{
}

ExpressionLayout * EmptyVisibleLayout::clone() const {
  EmptyVisibleLayout * layout = new EmptyVisibleLayout(m_color, m_isVisible);
  return layout;
}

void EmptyVisibleLayout::backspaceAtCursor(ExpressionLayoutCursor * cursor) {
  assert(cursor->pointedExpressionLayout() == this);
  if (cursor->position() == ExpressionLayoutCursor::Position::Right) {
    cursor->setPosition(ExpressionLayoutCursor::Position::Left);
    return;
  }
  assert(cursor->position() == ExpressionLayoutCursor::Position::Left);
  if (m_parent) {
    return m_parent->backspaceAtCursor(cursor);
  }
}

bool EmptyVisibleLayout::moveLeft(ExpressionLayoutCursor * cursor, bool * shouldRecomputeLayout) {
  assert(cursor->pointedExpressionLayout() == this);
  // Ask the parent.
  if (m_parent) {
    cursor->setPosition(ExpressionLayoutCursor::Position::Left);
    return m_parent->moveLeft(cursor, shouldRecomputeLayout);
  }
  return false;
}

bool EmptyVisibleLayout::moveRight(ExpressionLayoutCursor * cursor, bool * shouldRecomputeLayout) {
  assert(cursor->pointedExpressionLayout() == this);
  // Ask the parent.
  if (m_parent) {
    cursor->setPosition(ExpressionLayoutCursor::Position::Right);
    return m_parent->moveRight(cursor, shouldRecomputeLayout);
  }
  return false;
}

int EmptyVisibleLayout::writeTextInBuffer(char * buffer, int bufferSize, int numberOfSignificantDigits) const {
  if (bufferSize == 0) {
    return -1;
  }
  buffer[0] = 0;
  return 0;
}

void EmptyVisibleLayout::render(KDContext * ctx, KDPoint p, KDColor expressionColor, KDColor backgroundColor) {
  if (m_isVisible) {
    KDColor fillColor = m_color == Color::Yellow ? Palette::YellowDark : Palette::GreyBright;
    ctx->fillRect(KDRect(p.x()+k_marginWidth, p.y()+k_marginHeight, k_width, k_height), fillColor);
    ctx->fillRect(KDRect(p.x()+k_marginWidth, p.y()+k_marginHeight, k_width, k_height), fillColor);
  }
}

KDSize EmptyVisibleLayout::computeSize() {
  KDCoordinate width = m_isVisible ? k_width + 2*k_marginWidth : 0;
  return KDSize(width, k_height + 2*k_marginHeight);
}

void EmptyVisibleLayout::computeBaseline() {
  m_baseline = k_marginHeight + k_height/2;
  m_baselined = true;
}

void EmptyVisibleLayout::privateAddBrother(ExpressionLayoutCursor * cursor, ExpressionLayout * brother, bool moveCursor) {
  Color currentColor = m_color;
  int indexInParent = m_parent->indexOfChild(this);
  ExpressionLayout * parent = m_parent;
  if (brother->mustHaveLeftBrother()) {
    m_color = Color::Yellow;
    ExpressionLayout::privateAddBrother(cursor, brother, moveCursor);
  } else {
    if (moveCursor) {
      replaceWithAndMoveCursor(brother, true, cursor);
    } else {
      replaceWith(brother, true);
    }
  }
  if (currentColor == Color::Grey) {
    // The parent is a MatrixLayout.
    static_cast<MatrixLayout *>(parent)->newRowOrColumnAtIndex(indexInParent);
  }
}

}
