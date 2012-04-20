/*
 * qt_service_model.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Apr 20, 2012
 */

#include "qt_service_model.h"
#include "qt_service.h"
#include "boost/cast.hpp"

namespace sdc {

QtService* QtServiceModel::GetQtService() {
  return boost::polymorphic_downcast<QtService*>(this->GetService());
}

} /* namespace sdc */