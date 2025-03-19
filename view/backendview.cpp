#include "backendview.h"
#include <QVBoxLayout>
#include <QLabel>

BackendView::BackendView(QWidget *parent) : QWidget(parent) {
    setWindowTitle("后台管理系统");
    resize(800, 600);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel("欢迎进入后台管理系统！", this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    setLayout(layout);
}