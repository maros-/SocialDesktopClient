#include "activity.h"
#include "ui_activity.h"

namespace sdc {

Activity::Activity(QWidget *parent)
  : QWidget(parent),
    ui(new Ui::Activity) {
  ui->setupUi(this);
}

Activity::~Activity() {
  delete ui;
}

QLabel* Activity::image_label() {
  return ui->imageLabel;
}

QLabel* Activity::text_label() {
  return ui->textLabel;
}

} /* namespace sdc */