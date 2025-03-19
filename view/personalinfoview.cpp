#include "personalinfoview.h"

PersonalInfoView::PersonalInfoView(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    infoLabel = new QLabel("个人信息界面（暂时为空）", this);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet("font-size: 18px; color: #555;");

    layout->addWidget(infoLabel);
    setLayout(layout);
}