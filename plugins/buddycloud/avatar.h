/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef AVATAR_H_
#define AVATAR_H_

#include <QObject>
#include <QPixmap>

class Avatar : public QObject {

    Q_OBJECT

 public:
  Avatar(const std::string &file) {
    Load(file);
  }
  Avatar(const QPixmap &pixmap) {
    avatar_ = pixmap;
  }

  void Load(const std::string &file) {
    avatar_.load(QString::fromStdString(file));
    changed(avatar_);
  }

  QPixmap GetPixmap() {
    return avatar_;
  }

 signals:
  void changed(const QPixmap &);

 private:
  QPixmap avatar_;
};

#endif // AVATAR_H_
