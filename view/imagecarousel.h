#ifndef IMAGECAROUSEL_H
#define IMAGECAROUSEL_H

#include <QWidget>
#include <QStackedWidget>
#include <QTimer>
#include <QLabel>

class ImageCarousel : public QWidget {
    Q_OBJECT

public:
    explicit ImageCarousel(QWidget *parent = nullptr);

private slots:
    void switchImage(); // 切换图片的槽函数

private:
    QStackedWidget *imageStack; // 图片轮播容器
    QTimer *imageSwitchTimer;   // 定时器，用于切换图片
};

#endif // IMAGECAROUSEL_H