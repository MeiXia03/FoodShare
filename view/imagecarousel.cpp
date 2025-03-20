#include "imagecarousel.h"
#include <QVBoxLayout>
#include <QPixmap>
#include <QFile>

ImageCarousel::ImageCarousel(QWidget *parent) : QWidget(parent) {
    // 创建顶部标题标签
    titleLabel = new QLabel("今日热门", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #333; margin-bottom: 10px;");

    // 创建图片轮播容器
    imageStack = new QStackedWidget(this);
    imageStack->setStyleSheet("border: 2px solid #ccc; border-radius: 10px; background-color: #f9f9f9;");

    // 创建当前图片标题标签
    foodTitleLabel = new QLabel(this);
    foodTitleLabel->setAlignment(Qt::AlignCenter);
    foodTitleLabel->setStyleSheet("font-size: 18px; color: #555; margin-top: 10px;");

    // 创建定时器切换图片
    imageSwitchTimer = new QTimer(this);
    connect(imageSwitchTimer, &QTimer::timeout, this, &ImageCarousel::switchImage);
    imageSwitchTimer->start(5000); // 每 5 秒切换一次图片

    // 设置布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel); // 添加顶部标题
    layout->addWidget(imageStack); // 添加图片轮播区
    layout->addWidget(foodTitleLabel); // 添加当前图片标题
    layout->setContentsMargins(20, 20, 20, 20); // 设置边距
    setLayout(layout);

    // 加载数据
    loadTopLikedImages();
}

void ImageCarousel::refreshData() {
    // 清空图片轮播容器
    while (imageStack->count() > 0) {
        QWidget *widget = imageStack->widget(0);
        imageStack->removeWidget(widget);
        delete widget; // 删除子控件以释放内存
    }

    // 重新加载数据
    loadTopLikedImages();
}

void ImageCarousel::switchImage() {
    int currentIndex = imageStack->currentIndex();
    int nextIndex = (currentIndex + 1) % imageStack->count();
    imageStack->setCurrentIndex(nextIndex);

    // 更新当前图片标题
    QLabel *currentLabel = qobject_cast<QLabel *>(imageStack->currentWidget());
    if (currentLabel) {
        foodTitleLabel->setText(currentLabel->property("title").toString());
    }
}

void ImageCarousel::loadTopLikedImages() {
    QSqlQuery query;
    query.prepare("SELECT title, video_path FROM recipes ORDER BY likes DESC LIMIT 3");

    if (!query.exec()) {
        qDebug() << "加载点赞数最多的图片失败:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QString title = query.value("title").toString(); // 获取标题
        QString imagePath = query.value("video_path").toString(); // 从数据库获取图片路径

        QLabel *imageLabel = new QLabel(this);

        // 检查图片路径是否有效
        if (!QFile::exists(imagePath)) {
            qDebug() << "图片文件不存在，路径:" << imagePath;
            continue;
        }

        QPixmap pixmap(imagePath); // 使用文件系统中的路径加载图片

        if (pixmap.isNull()) {
            qDebug() << "图片加载失败，路径:" << imagePath;
            continue;
        }

        imageLabel->setPixmap(pixmap.scaled(800, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        imageLabel->setAlignment(Qt::AlignCenter);
        imageLabel->setProperty("title", title); // 将标题存储为属性
        imageStack->addWidget(imageLabel);
    }

    // 设置初始标题
    if (imageStack->count() > 0) {
        QLabel *firstLabel = qobject_cast<QLabel *>(imageStack->currentWidget());
        if (firstLabel) {
            foodTitleLabel->setText(firstLabel->property("title").toString());
        }
    }
}