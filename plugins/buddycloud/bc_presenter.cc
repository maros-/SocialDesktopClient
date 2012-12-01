/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "bc_presenter.h"
#include "bc_contact.h"
#include "bc_contact_widget.h"
#include "bc_model.h"
#include "channel.h"
#include "channel_widget.h"
#include "comment_activity.h"
#include "post.h"
#include "post_activity.h"
#include "sdc/core/core.h"
#include "sdc/core/log.h"
#include "sdc/qtgui/activity.h"
#include "sdc/qtgui/activities_panel.h"
#include <QFile>
#include <QIcon>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QPixmap>
#include <QWidgetAction>

#include "sdc/core/log.h"

BcPresenter::BcPresenter() : AbstractPresenter(this), channel_(0) {}

BcPresenter::~BcPresenter() {}

void BcPresenter::Init() {
  model_ = model<BcModel>();
  qRegisterMetaType<JID>("JID");

  // set avatar
  UpdateAvatar(model_->GetOwnJID());

  // set buddycloud icon on account button
  QIcon icon(":/icon.svg");
  account_button()->button()->setIcon(icon);
  account_button()->button()->setIconSize(QSize(32, 32));
  // ... and also on contacts button
  contacts_button()->setIcon(icon);
  contacts_button()->setIconSize(QSize(16, 16));

  connect(account_button()->title_action(), SIGNAL(triggered()),
          this, SLOT(ShowOwnChannel()));

  // set subscribe to channel action
  QMenu* subscribe = account_button()->button()->menu()->addMenu("Show Channel");
  QWidgetAction* edit = new QWidgetAction(account_button()->button()->menu());
  subscribe_to_ = new QLineEdit;
  subscribe_to_->setMinimumWidth(200);
  edit->setDefaultWidget(subscribe_to_);
  subscribe->addAction(edit);
  connect(subscribe_to_, SIGNAL(returnPressed()), this, SLOT(OnShowChannelLineEditEnter()));

  // contacts button logic
  subscribers_layout_ = new QVBoxLayout;
  subscribers_layout_->addStretch(10);
  QWidget* sw = new QWidget;
  sw->setLayout(subscribers_layout_);

  followers_ = new QWidget;
  followers_ ->setLayout(new QVBoxLayout);
  subscribers_pos_ = main_window()->contacts()->stacked_widget()->addWidget(sw);
  followers_pos_ = main_window()->contacts()->stacked_widget()->addWidget(followers_);
  QAction* subs = new QAction("Subscriptions", contacts_button());
  QAction* foll = new QAction("Followers", contacts_button());
  contacts_button()->addAction(subs);
  contacts_button()->addAction(foll);
  connect(subs, SIGNAL(triggered()), this, SLOT(ShowSubscribers()));
  connect(foll, SIGNAL(triggered()), this, SLOT(ShowFollowers()));

  // set stylesheet
  QFile file("plugins/buddycloud/resources/bc.qss");
  file.open(QFile::ReadOnly);
  QString stylesheet = QLatin1String(file.readAll());
  SetStylesheet(stylesheet);

  // bindings
  sdc::bind(model_->onConnected, [&] () {
    if (!channel_) {
      channel_ = new ChannelPanel(this, model_->GetOwnChannel());
      channels_[model_->GetOwnJID()] = channel_;
      this->main_window()->AddContentPanel(this, channel_);
    }
  });

  sdc::bind(model_->onOwnAvatarChanged, [&] () {
    //SetOwnAvatar(model_->GetOwnAvatarPath());
  });

  sdc::bind(model_->onAvatarChanged, [&] (const JID jid) {
    UpdateAvatar(jid);
  });

  sdc::bind(model_->onContactAdded, [&] (const JID jid) {
    BcContact* contact = model_->GetContact(jid);
    BcContactWidget* widget = new BcContactWidget(this, contact);
    contacts_.append(widget);
    this->main_window()->AddContact(this, widget);
  });

  sdc::bind(model_->onNewPost, [&] (const Post* post) {
    PostActivity* act = new PostActivity(this, post);
    this->main_window()->activities()->AddActivity(act);
  });

  sdc::bind(model_->onNewComment, [&] (const Comment* comment) {
    CommentActivity* act = new CommentActivity(this, comment);
    this->main_window()->activities()->AddActivity(act);
  });

  sdc::bind(model_->onError, [&] (BcModel::Error error) {
    switch (error) {
    case BcModel::ServiceUnavailable:
      QMessageBox msgbox;
      msgbox.setText("Channel service is unavailable. Account will be deactivated.");
      msgbox.setIcon(QMessageBox::Critical);
      msgbox.exec();
      sdc::Core::Instance()->data()->SetEnabledAccount(model()->account()->GetDataIndex(), false);
      break;
    }
  });

  /*
   * Own channel logic
   * In order to work with the channel it has to be available first.
   */
  sdc::bind(model_->onChannelAvailable, [&] () {

    model_->GetOwnChannel()->RetrieveSubscriptions();

    sdc::bind(model_->GetOwnChannel()->onSubscriptionsRetrieved, [&] (std::map<Swift::JID, Channel::Subscription> subscriptions) {
      if (subscribers_layout_ != NULL)
      {
        QLayoutItem* item;
        while ((item = subscribers_layout_->takeAt(0)) != NULL)
        {
          delete item->widget();
          delete item;
        }
      }
      std::map<Swift::JID, Channel::Subscription>::iterator iter;
      for (iter = subscriptions.begin(); iter != subscriptions.end(); ++iter) {
        BcContact* contact = new BcContact(model_, iter->first);
        contact->SetUid(iter->first);
        BcContactWidget* widget = new BcContactWidget(this, contact);
        subscribers_layout_->insertWidget(0, widget, 0, Qt::AlignTop);
      }
    });

  });


  model_->Connect();
}

Avatar* BcPresenter::GetAvatar(const JID &jid) {
  if (avatars_.count(jid) > 0) {
    return avatars_[jid];
  } else {
    avatars_[jid] = new Avatar(model_->GetDefaultAvatarPath());
    return avatars_[jid];
  }
}

void BcPresenter::ShowChannel(const JID &jid) {
  if (channels_.contains(jid))
    channels_[jid]->show();
  else {
    ChannelPanel* cw = new ChannelPanel(this, model_->GetChannel(jid.toString()));
    channels_[jid] = cw;
    main_window()->AddContentPanel(this, cw);
  }
}

void BcPresenter::ShowOwnChannel() {
  channel_->show();
}

void BcPresenter::OnShowChannelLineEditEnter() {
  ShowChannel(subscribe_to_->text().toStdString());
}

void BcPresenter::ShowSubscribers() {
  main_window()->contacts()->stacked_widget()->setCurrentIndex(subscribers_pos_);
}

void BcPresenter::ShowFollowers() {
  main_window()->contacts()->stacked_widget()->setCurrentIndex(followers_pos_);
}

void BcPresenter::UpdateAvatar(const JID &jid) {
  if (avatars_.count(jid) > 0) {
    avatars_[jid]->Load(model_->GetAvatarPath(jid));
  } else {
    avatars_[jid] = new Avatar(model_->GetAvatarPath(jid));
  }
}
