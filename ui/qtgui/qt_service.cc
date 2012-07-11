/*
 * qt_service.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Apr 20, 2012
 */

#include "qt_service.h"
#include "account_button.h"
#include "contact_widget.h"

#include "log.h"

namespace sdc {

ContactWidget* QtService::CreateContactWidget(QWidget* parent, Contact::Ref contact) {
  return new ContactWidget(parent, contact);
}

} /* namespace sdc */
