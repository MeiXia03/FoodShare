#ifndef IMAGECAROUSEL_H
#define IMAGECAROUSEL_H

#include <QWidget>
#include <QStackedWidget>
#include <QTimer>
#include <QLabel>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QSqlQuery>
#include <QSqlError>

class ImageCarousel : public QWidget {
    Q_OBJECT

public:
    explicit ImageCarousel(QWidget *parent = nullptr);

private slots:
    void switchImage(); // 切换图片的槽函数

private:
    QStackedWidget *imageStack; // 图片轮播容器
    QTimer *imageSwitchTimer;   // 定时器，用于切换图片
    QMediaPlayer *mediaPlayer;  // 视频播放器
    QVideoWidget *videoWidget;  // 视频显示组件

    void loadTopLikedImages();  // 加载点赞数最多的图片
    void loadVideo();           // 加载视频
};

#endif // IMAGECAROUSEL_H
