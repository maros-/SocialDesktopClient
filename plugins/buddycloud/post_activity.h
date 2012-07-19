#ifndef POST_ACTIVITY_H
#define POST_ACTIVITY_H

#include "sdc/qtgui/activity.h"

class BcPresenter;
class Post;

class PostActivity : public sdc::Activity {
 public:
  PostActivity(BcPresenter* parent, const Post* post);

 protected:
  void mouseDoubleClickEvent(QMouseEvent*);

 private:
  BcPresenter* presenter_;
  const Post* post_;
};

#endif // POST_ACTIVITY_H
