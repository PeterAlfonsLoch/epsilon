#ifndef SEQUENCE_LIST_CONTROLLER_H
#define SEQUENCE_LIST_CONTROLLER_H

#include "../../shared/sequence_title_cell.h"
#include "../../shared/sequence_store.h"
#include "../../shared/function_expression_cell.h"
#include "../../shared/function_list_controller.h"
#include "../../shared/input_event_handler_delegate.h"
#include "../../shared/layout_field_delegate.h"
#include "../../shared/text_field_delegate.h"
#include "list_parameter_controller.h"
#include "sequence_toolbox.h"
#include "type_parameter_controller.h"

namespace Sequence {

class ListController : public Shared::FunctionListController, public Shared::InputEventHandlerDelegate, public Shared::TextFieldDelegate, public Shared::LayoutFieldDelegate {
public:
  ListController(Escher::Responder * parentResponder, Escher::InputEventHandlerDelegate * inputEventHandlerDelegate, Escher::ButtonRowController * header, Escher::ButtonRowController * footer);
  const char * title() override;
  int numberOfExpressionRows() const override;
  KDCoordinate expressionRowHeight(int j) override;
  void willDisplayCellAtLocation(Escher::HighlightCell * cell, int i, int j) override;
  Escher::Toolbox * toolboxForInputEventHandler(Escher::InputEventHandler * handler) override;
  void selectPreviousNewSequenceCell();
  void editExpression(int sequenceDefinitionIndex, Ion::Events::Event event);
private:
  static constexpr KDCoordinate k_expressionCellVerticalMargin = 3;
  bool editInitialConditionOfSelectedRecordWithText(const char * text, bool firstInitialCondition);
  ListParameterController * parameterController() override;
  int maxNumberOfDisplayableRows() override;
  Shared::FunctionTitleCell * titleCells(int index) override;
  Escher::HighlightCell * expressionCells(int index) override;
  void willDisplayTitleCellAtIndex(Escher::HighlightCell * cell, int j) override;
  void willDisplayExpressionCellAtIndex(Escher::HighlightCell * cell, int j) override;
  int modelIndexForRow(int j) override;
  int sequenceDefinitionForRow(int j);
  void addModel() override;
  void reinitSelectedExpression(Shared::ExpiringPointer<Shared::ExpressionModelHandle> model) override;
  void editExpression(Ion::Events::Event event) override;
  bool removeModelRow(Ion::Storage::Record record) override;
  Shared::SequenceStore * modelStore() override;
  constexpr static int k_maxNumberOfRows = 3*Shared::MaxNumberOfSequences;
  Shared::SequenceTitleCell m_sequenceTitleCells[k_maxNumberOfRows];
  Shared::FunctionExpressionCell m_expressionCells[k_maxNumberOfRows];
  ListParameterController m_parameterController;
  TypeParameterController m_typeParameterController;
  Escher::StackViewController m_typeStackController;
  SequenceToolbox m_sequenceToolbox;
};

}

#endif
