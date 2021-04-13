#include <escher/message_table_cell_with_editable_text.h>
#include <escher/stack_view_controller.h>
#include "store_parameter_controller.h"
#include "store_controller.h"
#include <poincare/helpers.h>
#include <assert.h>
#include <escher/container.h>

using namespace Escher;

namespace Shared {

StoreParameterController::StoreParameterController(Responder * parentResponder, DoublePairStore * store, StoreController * storeController) :
  SelectableListViewController(parentResponder),
  m_store(store),
  m_series(0),
  m_cells{I18n::Message::ClearColumn, I18n::Message::FillWithFormula},
  m_sortCell(I18n::Message::SortCellLabel),
  m_storeController(storeController),
  m_xColumnSelected(true),
  m_confirmPopUpController(Invocation([](void * context, void * parent){
    StoreParameterController * param = static_cast<StoreParameterController *>(context);
    param->deleteColumn();
    return true;
  }, this))
{ }

void StoreParameterController::willDisplayCellForIndex(HighlightCell * cell, int index) {
  assert(index >= 0 && index < k_totalNumberOfCell);
  if (index == k_totalNumberOfCell - 1) {
    assert(cell == &m_sortCell);
    m_sortCell.setSubLabelMessage(sortMessage());
  }
  ListViewDataSource::willDisplayCellForIndex(cell, index);
}

const char * StoreParameterController::title() {
  return I18n::translate(I18n::Message::ColumnOptions);
}

void StoreParameterController::viewWillAppear() {
  resetMemoization();
  m_selectableTableView.reloadData();
}

void StoreParameterController::didBecomeFirstResponder() {
  selectCellAtLocation(0, 0);
  Container::activeApp()->setFirstResponder(&m_selectableTableView);
}

bool StoreParameterController::handleEvent(Ion::Events::Event event) {
  if (event != Ion::Events::OK && event != Ion::Events::EXE) {
    return false;
  }
  switch (selectedRow()) {
    case k_indexOfRemoveColumn:
    {
      // Display confirmation popup before removing column
      Container::activeApp()->displayModalViewController(&m_confirmPopUpController, 0.f, 0.f,
          Metric::PopUpTopMargin, Metric::PopUpRightMargin, Metric::PopUpBottomMargin, Metric::PopUpLeftMargin);
      return true;
    }
    case k_indexOfFillFormula:
    {
      m_storeController->displayFormulaInput();
      break;
    }
    case k_indexOfSortValues:
    {
      Poincare::Helpers::Swap swapRows = [](int i, int j, void * context, int numberOfElements) {
        double * contextI = (static_cast<double*>(context) + i);
        double * contextJ = (static_cast<double*>(context) + j);
        double * contextIOtherColumn = (static_cast<double*>(context) + DoublePairStore::k_maxNumberOfPairs + i);
        double * contextJOtherColumn = (static_cast<double*>(context) + DoublePairStore::k_maxNumberOfPairs + j);
        double temp1 = *contextI;
        double temp2 = *contextIOtherColumn;
        *contextI = *contextJ;
        *contextIOtherColumn = *contextJOtherColumn;
        *contextJ = temp1;
        *contextJOtherColumn = temp2;
      };
      Poincare::Helpers::Compare compareX = [](int a, int b, void * context, int numberOfElements)->bool{
        double * contextA = (static_cast<double*>(context) + a);
        double * contextB = (static_cast<double*>(context) + b);
        return *contextA > *contextB;
      };
      Poincare::Helpers::Compare compareY = [](int a, int b, void * context, int numberOfElements)->bool{
        double * contextAOtherColumn = (static_cast<double*>(context) + DoublePairStore::k_maxNumberOfPairs + a);
        double * contextBOtherColumn = (static_cast<double*>(context) + DoublePairStore::k_maxNumberOfPairs + b);
        return *contextAOtherColumn > *contextBOtherColumn;
      };
      double * seriesContext = m_store->data() + m_series * DoublePairStore::k_numberOfColumnsPerSeries * DoublePairStore::k_maxNumberOfPairs;
      if (m_xColumnSelected) {
        Poincare::Helpers::Sort(swapRows, compareX, seriesContext, m_store->numberOfPairsOfSeries(m_series));
      } else {
        Poincare::Helpers::Sort(swapRows, compareY, seriesContext, m_store->numberOfPairsOfSeries(m_series));
      }
      break;
    }
  }
  assert(selectedRow() >= 0 && selectedRow() <= 2);
  StackViewController * stack = static_cast<StackViewController *>(parentResponder());
  stack->pop();
  return true;
}

HighlightCell * StoreParameterController::reusableCell(int index, int type) {
  assert(index >= 0);
  if (type == k_sortCellType) {
    return &m_sortCell;
  }
  assert(type == k_defaultCellType);
  return &m_cells[index];
}

void StoreParameterController::deleteColumn() {
  if (m_xColumnSelected) {
    m_store->deleteAllPairsOfSeries(m_series);
  } else {
    m_store->resetColumn(m_series, 1);
  }
  Container::activeApp()->dismissModalViewController(false);
  StackViewController * stack = static_cast<StackViewController *>(parentResponder());
  stack->pop();
}

}
