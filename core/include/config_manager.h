/**
 * @file config_manager.h
 * @brief SDC configuration interface. User specific configuration data.
 *
 * @author Maros Kasinec
 *   @date Aug 6, 2011
 */

#ifndef CORE_CONFIGMANAGER_H_
#define CORE_CONFIGMANAGER_H_

#include "account_data.h"
#include "abstract_manager.h"
#include "properties.h"
#include "boost/serialization/nvp.hpp"
#include "boost/serialization/map.hpp"
#include "boost/serialization/string.hpp"
#include "boost/serialization/vector.hpp"
#include "boost/signals2.hpp"
#include <string>
#include <vector>
#include <map>

namespace sdc {

class Library {
 public:
  friend class boost::serialization::access;

 private:
  bool enabled_;
  std::string filename_;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    if (version) {}
    ar & BOOST_SERIALIZATION_NVP(enabled_);
    ar & BOOST_SERIALIZATION_NVP(filename_);
  }
};

class AccountData;

/// @class ConfigManager
/// @brief Maintains application configuration and provides interface for other managers.
class ConfigManager : public AbstractManager, public Properties {
 public:
  friend class boost::serialization::access;
  friend class Core;

  ConfigManager(Core* core) : AbstractManager(core) {}
  void Init();

  void SetEnabledAccount(int index, bool is_enabled) {
    if (accounts_[index]->IsEnabled() != is_enabled) {
      accounts_[index]->SetEnabled(is_enabled);
      onAccountsChanged();
      if (is_enabled) {
        onAccountEnabled(accounts_[index]);
      } else {
        onAccountDisabled(accounts_[index]);
      }
    }
  }

  void AddAccount(AccountData* account) {
    accounts_.push_back(account);
    SetEnabledAccount(accounts_.size() - 1, true);
    onAccountsChanged();
  }

  void SetAccount(int index, AccountData* data) {
    accounts_[index] = data;
    onAccountsChanged();
  }

  void RemoveAccount(int index) {
    onAccountDisabled(accounts_[index]);
    accounts_.erase(accounts_.begin() + index);
    onAccountsChanged();
  }

  AccountData* GetAccount(int index) {
    return accounts_[index];
  }

  std::vector<AccountData*> GetAccounts() const {
    return accounts_;
  }

  void OnExit();

  boost::signals2::signal<void ()> onAccountsChanged;
  boost::signals2::signal<void (AccountData*)> onAccountEnabled;
  boost::signals2::signal<void (AccountData*)> onAccountDisabled;

 private:
  std::vector<AccountData*>& accounts() {
    return accounts_;
  }

  static const std::string kConfFile;
  std::vector<Library> libraries_;
  std::vector<AccountData*> accounts_;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    //ar & boost::serialization::base_object<Properties>(*this);
    if (version) {}
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Properties);
    ar & BOOST_SERIALIZATION_NVP(libraries_);
    ar & BOOST_SERIALIZATION_NVP(accounts_);
  }
};

} /* namespace sdc */

#endif /* CORE_CONFIGMANAGER_H_ */
