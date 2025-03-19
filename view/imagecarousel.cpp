#include "imagecarousel.h"
#include <QVBoxLayout>
#include <QPixmap>

ImageCarousel::ImageCarousel(QWidget *parent) : QWidget(parent) {
    // 创建图片轮播容器
    imageStack = new QStackedWidget(this);

    // 创建定时器切换图片
    imageSwitchTimer = new QTimer(this);
    connect(imageSwitchTimer, &QTimer::timeout, this, &ImageCarousel::switchImage);
    imageSwitchTimer->start(3000); // 每 3 秒切换一次图片

    // 设置布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(imageStack); // 图片轮播区
    setLayout(layout);

    // 加载数据
    loadTopLikedImages();
}

void ImageCarousel::switchImage() {
    int currentIndex = imageStack->currentIndex();
    int nextIndex = (currentIndex + 1) % imageStack->count();
    imageStack->setCurrentIndex(nextIndex);
}

void ImageCarousel::loadTopLikedImages() {
    QSqlQuery query;
    query.prepare("SELECT recipe_id, title FROM recipes ORDER BY likes DESC LIMIT 3");

    if (!query.exec()) {
        qDebug() << "加载点赞数最多的图片失败:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        int recipeId = query.value("recipe_id").toInt(); // 获取 recipe_id
        QString title = query.value("title").toString(); // 获取标题

        QLabel *imageLabel = new QLabel(this);
        QString imagePath = QString(":/recipe_%1.jpg").arg(recipeId); // 构造图片路径
        QPixmap pixmap(imagePath);

        if (pixmap.isNull()) {
            qDebug() << "图片加载失败，路径:" << imagePath;
            continue;
        }

        imageLabel->setPixmap(pixmap.scaled(800, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        imageLabel->setAlignment(Qt::AlignCenter);
        imageStack->addWidget(imageLabel);
    }
}