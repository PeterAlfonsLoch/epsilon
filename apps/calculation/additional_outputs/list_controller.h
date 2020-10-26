#ifndef CALCULATION_ADDITIONAL_OUTPUTS_LIST_CONTROLLER_H
#define CALCULATION_ADDITIONAL_OUTPUTS_LIST_CONTROLLER_H

#include <escher.h>
#include <apps/i18n.h>

namespace Calculation {

class EditExpressionController;

class ListController : public Escher::StackViewController, public Escher::ListViewDataSource, public Escher::SelectableTableViewDataSource {
public:
  ListController(EditExpressionController * editExpressionController, Escher::SelectableTableViewDelegate * delegate = nullptr);

  // Responder
  bool handleEvent(Ion::Events::Event event) override;
  void didBecomeFirstResponder() override;

  // ListController
  virtual void setExpression(Poincare::Expression e) = 0;

protected:
  class InnerListController : public ViewController {
  public:
    InnerListController(ListController * dataSource, Escher::SelectableTableViewDelegate * delegate = nullptr);
    const char * title() override { return I18n::translate(I18n::Message::AdditionalResults); }
    Escher::View * view() override { return &m_selectableTableView; }
    void didBecomeFirstResponder() override;
    Escher::SelectableTableView * selectableTableView() { return &m_selectableTableView; }
  private:
    Escher::SelectableTableView m_selectableTableView;
  };
  virtual int textAtIndex(char * buffer, size_t bufferSize, int index) = 0;
  InnerListController m_listController;
  EditExpressionController * m_editExpressionController;
};

}

#endif

