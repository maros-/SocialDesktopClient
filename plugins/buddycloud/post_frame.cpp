#include "post_frame.h"
#include "comment_frame.h"
#include "bc_model.h"
#include "bc_presenter.h"
#include "post.h"
#include "sdc/qtgui/bind.h"
#include <QMessageBox>
#include <QAbstractTextDocumentLayout>
#include "boost/date_time/posix_time/time_formatters.hpp"

PostFrame::PostFrame(AbstractPresenter* presenter, Post* post)
    : AbstractPresenter(presenter),
      post_(post) {
	ui.setupUi(this);
  ui.commentLineEdit->hide();
  // set post data
  ui.authorLabel->setText(QString::fromStdString(post_->GetAuthor()));
  ui.contentLabel->setText(QString::fromStdString(post_->GetContent()));
  setToolTip(QString::fromStdString(boost::posix_time::to_simple_string(post_->GetPublished())));
  // setup avatar
  Avatar* avatar = this->presenter()->GetAvatar(post_->GetAuthor());
  ui.avatarLabel->setPixmap(avatar->GetPixmap());
  connect(avatar, SIGNAL(changed(QPixmap)), ui.avatarLabel, SLOT(setPixmap(QPixmap)));
  // set focusing policy
  setFocusPolicy(Qt::StrongFocus);
  setFocusProxy(ui.commentLineEdit);
  // bindings
	connect(ui.deleteButton, SIGNAL(clicked()),
	    this, SLOT(DeletePost()));
  connect(ui.commentLineEdit, SIGNAL(returnPressed()),
          this, SLOT(PostComment()));
  sdc::bind(post_->onCommentAdded, [&] (Comment* comment) {
    ShowCommentInOrder(comment);
  });
  // show comments
  for (Comment* comment : post_->comments()) {
    ShowCommentInOrder(comment);
  }
}

PostFrame::~PostFrame() {
  for (CommentFrame* widget : comments_)
    delete widget;
}

void PostFrame::DeletePost() {
  QMessageBox confirm;
  confirm.setText("Do you really want to delete this post?");
  confirm.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  confirm.setDefaultButton(QMessageBox::No);
  int result = confirm.exec();
  if (result == QMessageBox::Yes) {
    post_->Delete();
  }
}

void PostFrame::mouseReleaseEvent(QMouseEvent *) {
  ui.commentLineEdit->show();
  setFocus();
}

void PostFrame::PostComment() {
  post_->PostComment(ui.commentLineEdit->text().toStdString());
  ui.commentLineEdit->clear();
}

void PostFrame::ShowCommentInOrder(Comment* comment) {
  QList<Comment*>::iterator it = qUpperBound(comments_order_.begin(), comments_order_.end(), comment,
      [&] (const Comment* c1, const Comment* c2) { return c1->GetPublished() < c2->GetPublished(); });
  comments_order_.insert(it, comment);
  CommentFrame* cw = new CommentFrame(this, comment);
  comments_[comment] = cw;
  ui.commentsLayout->insertWidget(comments_order_.indexOf(comment), cw);
}