#ifndef PERSONALINFOVIEW_H
#define PERSONALINFOVIEW_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class PersonalInfoView : public QWidget {
    Q_OBJECT

public:
    explicit PersonalInfoView(QWidget *parent = nullptr);

private:
    QLabel *infoLabel; // 显示个人信息的标签
};

#endif // PERSONALINFOVIEW_H