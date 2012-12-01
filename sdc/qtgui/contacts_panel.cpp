/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "contacts_panel.h"
#include "new_contact_dialog.h"
#include <QStackedWidget>
#include <QToolButton>
#include <QPushButton>

namespace sdc {

ContactsPanel::ContactsPanel() {
  title_label()->setText("Contacts");
  setObjectName("ContactsPanel");

  /* TODO: deprecated
  add_button_ = new QToolButton(this);
  add_button_->setObjectName("AddButton");
  add_button_->setToolTip("New Contact");
  AddTitleBarButton(add_button_);
  connect(add_button_, SIGNAL(clicked()), this, SLOT(ShowNewContactDialog()));
  */

  stack_ = new QStackedWidget;
  content_layout()->addWidget(stack_);
}

void ContactsPanel::ShowNewContactDialog() {
  NewContactDialog* dialog = new NewContactDialog(this);
  dialog->exec();
}

} // namespace sdc
