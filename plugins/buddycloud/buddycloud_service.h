/**
 * @file buddycloud_service.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Feb 7, 2012
 */

#ifndef BUDDYCLOUD_SERVICE_H_
#define BUDDYCLOUD_SERVICE_H_

#include "sdc.h"

class BuddycloudService : public sdc::QtService {
 public:
  BuddycloudService() {
    name_ = "Buddycloud";
    description_ = "Buddycloud channels service.";
  }

  sdc::Connection* CreateConnection();
  sdc::ServiceModelRef CreateMainModel();
  sdc::ServiceViewRef CreateMainView();
  sdc::ServiceController* CreateServiceController();
  //void InitializeGui(sdc::MainView* main_view);
  sdc::ServiceModel* CreateServiceModel(sdc::AccountData* account_data);

  sdc::NewAccountWidget* CreateNewAccountPane();
  sdc::NewContactWidget* CreateNewContactWidget();

  std::vector<sdc::WidgetFactory*> CreateWidgetFactories();
};

#endif /* BUDDYCLOUD_SERVICE_H_ */
