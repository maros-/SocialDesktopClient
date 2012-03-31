/**
 * @file core_anchor.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Mar 31, 2012
 */

#ifndef CORE_ANCHOR_H_
#define CORE_ANCHOR_H_

namespace sdc {

class Core;

class CoreAnchor {
 public:
  CoreAnchor() {} // TODO: Remove this together with SetCore() method
  CoreAnchor(Core* core) {
    core_ = core;
  }

  void SetCore(Core* core) { // TODO: Modify so the core would be set in ctor in every manager.
    core_ = core;
  }

 protected:
  Core* GetCore() { return core_; }
  Core* core()  { return core_; }

 private:
  Core* core_;
};

} /* namespace sdc */

#endif /* CORE_ANCHOR_H_ */
