/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "activities_button.h"
#include "activities_panel.h"

namespace sdc {

ActivitiesButton::ActivitiesButton() {
  button()->setText("Activities");
  button()->setObjectName("ActivitiesButton");
  button()->setCheckable(true);
  button()->setChecked(true);
  // set icon
  button()->setIcon(QIcon(":/icons/Activity Monitor.ico"));
  button()->setIconSize(QSize(30, 30));
}

void ActivitiesButton::SetPanel(ActivitiesPanel* panel) {
  activities_ = panel;
  connect(button(), SIGNAL(toggled(bool)), activities_, SLOT(setVisible(bool)));
  connect(activities_, SIGNAL(closed()), this, SLOT(UncheckButton()));
}

void ActivitiesButton::UncheckButton() {
  button()->setChecked(false);
}

} // namespace sdc
