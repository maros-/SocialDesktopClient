/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef CORE_UI_H_
#define CORE_UI_H_

namespace sdc {

class Service;
class ServiceController;

/**
 * Abstract class for UI implementation.
 */
class UI {
 public:
  UI();
  virtual ~UI();

  /**
   * UI initializer.
   */
  virtual void Init();

  /**
   * UI execution point.
   *
   * This method is responsible for keeping UI alive as long as it takes.
   */
  virtual int Exec()=0;
};

} // namespace sdc

#endif // CORE_UI_H_
