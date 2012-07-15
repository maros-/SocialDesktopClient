#ifndef CORE_PROPERTIES_H
#define CORE_PROPERTIES_H

#include "boost/serialization/nvp.hpp"
#include "boost/serialization/map.hpp"

namespace sdc {

class Properties {
 public:
  friend class boost::serialization::access;

  void SetProperty(const std::string &key, const std::string &value) {
    properties_[key] = value;
  }

  std::string GetProperty(const std::string &key) {
    if (properties_.count(key) > 0)
      return properties_[key];
    else
      return std::string();
  }

 private:
  std::map<std::string, std::string> properties_;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    if (version) {}
    ar & BOOST_SERIALIZATION_NVP(properties_);
  }
};

} /* namespace sdc */

#endif // CORE_PROPERTIES_H
