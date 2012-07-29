/**
 * @file bc_contact_widget.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Apr 24, 2012
 */

#ifndef BC_CONTACT_WIDGET_H_
#define BC_CONTACT_WIDGET_H_

#include "abstract_presenter.h"
#include "sdc/qtgui/contact_widget.h"

class BcContact;
class BcPresenter;
class ChannelPanel;

class BcContactWidget : public sdc::ContactWidget, public AbstractPresenter {
 public:
  BcContactWidget(BcPresenter* presenter, BcContact* contact);
  ~BcContactWidget();

  void Remove() {
    // TODO: impelment
    //contact_->bot()->RemoveContact(contact_);
  }

 protected:
  void mouseDoubleClickEvent(QMouseEvent* event);

 private:
  BcContact* contact_;
  ChannelPanel* channel_;
};

#endif /* BC_CONTACT_WIDGET_H_ */
