#include "ImageCarousel.h"
#include <QVBoxLayout>
#include <QPixmap>

ImageCarousel::ImageCarousel(QWidget *parent) : QWidget(parent) {
    // 创建图片轮播容器
    imageStack = new QStackedWidget(this);

    // 添加图片到 QStackedWidget
    QStringList imagePaths = {
        ":/image1.jpg",
        ":/image2.jpg",
        ":/image3.jpg"
    };

    for (const QString &path : imagePaths) {
        QLabel *imageLabel = new QLabel(this);
        QPixmap pixmap(path);
        imageLabel->setPixmap(pixmap.scaled(800, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        imageLabel->setAlignment(Qt::AlignCenter);
        imageStack->addWidget(imageLabel);
    }

    // 创建定时器切换图片
    imageSwitchTimer = new QTimer(this);
    connect(imageSwitchTimer, &QTimer::timeout, this, &ImageCarousel::switchImage);
    imageSwitchTimer->start(3000); // 每 3 秒切换一次图片

    // 设置布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(imageStack);
    setLayout(layout);
}

void ImageCarousel::switchImage() {
    int currentIndex = imageStack->currentIndex();
    int nextIndex = (currentIndex + 1) % imageStack->count();
    imageStack->setCurrentIndex(nextIndex);
}
