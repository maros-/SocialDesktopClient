/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef CORE_EVENT_MANAGER_H_
#define CORE_EVENT_MANAGER_H_

#include "common.h"
#include "abstract_manager.h"
#include "boost/bind.hpp"
#include "boost/thread.hpp"
#include "boost/function.hpp"
#include <vector>

namespace sdc {

class Core;
class Message;

class EventManager : public AbstractManager {
 public:
  class Event;

  EventManager(Core* core) : AbstractManager(core), is_running_(true) {}

  void Run();

  void PostEvent(boost::function<void ()> callback);

  void Stop();

 private:
  void DoStop();
  void ProcessEvent(const Event& event);

  std::vector<Event> queue_;
  boost::mutex mutex_;
  boost::condition_variable is_empty_queue_;
  bool is_running_;
};

struct EventManager::Event {
  Event(boost::function<void ()> function) : callback(function) {}
  unsigned int id; //FIXME: this is unused so far
  boost::function<void ()> callback;
};

}  // namespace sdc

#endif // CORE_EVENT_MANAGER_H_
