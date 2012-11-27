/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef CHANNEL_WIDGET_H_
#define CHANNEL_WIDGET_H_

#include "abstract_presenter.h"
#include "channel.h"
#include "ui_new_post_widget.h"
#include "sdc/qtgui/content_panel.h"
#include "boost/date_time/posix_time/ptime.hpp"
#include <QList>
#include <QMap>

class PostFrame;

class ChannelPanel : public sdc::ContentPanel, public AbstractPresenter {

    Q_OBJECT

 public:
  ChannelPanel(AbstractPresenter* presenter, Channel* channel);
  ~ChannelPanel();

 public slots:
  void SendPost();
  void OnScrollBarValueChanged(int value);
  void RetrieveNext();

 private:
  void ShowPostInOrder(Post* post);

  Channel* channel_;
  QList<Post*> posts_order_;
  QMap<Post*, PostFrame*> posts_;
  QToolButton* new_post_button_;
  QWidget* new_post_;
  Ui::NewPostClass new_post_ui;
  QScrollBar* scroll_bar_;
  int old_scroll_bar_value_;
};

#endif // CHANNEL_WIDGET_H_
