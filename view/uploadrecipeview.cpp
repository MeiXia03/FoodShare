#include "uploadrecipeview.h"
#include <QFileInfo>

UploadRecipeView::UploadRecipeView(QWidget *parent) : QWidget(parent) {
    if (!DatabaseManager::instance().connect()) {
        qDebug() << "数据库连接失败";
        return;
    }
    setupUI();
}

void UploadRecipeView::setupUI() {
    // 创建标题输入框
    titleEdit = new QLineEdit(this);
    titleEdit->setPlaceholderText("请输入食谱标题");
    titleEdit->setStyleSheet("padding: 5px; border: 1px solid #ccc; border-radius: 5px;");

    // 创建内容输入框
    contentEdit = new QTextEdit(this);
    contentEdit->setPlaceholderText("请输入食谱内容");
    contentEdit->setStyleSheet("padding: 5px; border: 1px solid #ccc; border-radius: 5px;");

    // 创建图片路径输入框
    videoPathEdit = new QLineEdit(this);
    videoPathEdit->setPlaceholderText("请选择图片文件");
    videoPathEdit->setReadOnly(true);
    videoPathEdit->setStyleSheet("padding: 5px; border: 1px solid #ccc; border-radius: 5px;");

    // 创建选择图片按钮
    selectImageButton = new QPushButton("选择图片", this);
    selectImageButton->setStyleSheet("padding: 5px; background-color: #2196F3; color: white; border-radius: 5px;");
    connect(selectImageButton, &QPushButton::clicked, this, &UploadRecipeView::onSelectImageClicked);

    // 创建分类下拉框
    categoryCombo = new QComboBox(this);
    categoryCombo->addItems({"中餐", "西餐", "甜点", "饮品"});
    categoryCombo->setStyleSheet("padding: 5px; border: 1px solid #ccc; border-radius: 5px;");

    // 创建类型下拉框
    typeCombo = new QComboBox(this);
    typeCombo->addItems({"素食", "荤食", "其他"});
    typeCombo->setStyleSheet("padding: 5px; border: 1px solid #ccc; border-radius: 5px;");

    // 创建上传按钮
    uploadButton = new QPushButton("上传", this);
    uploadButton->setStyleSheet("padding: 10px; background-color: #4CAF50; color: white; border-radius: 5px;");
    connect(uploadButton, &QPushButton::clicked, this, &UploadRecipeView::onUploadClicked);

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("食谱标题:", this));
    layout->addWidget(titleEdit);
    layout->addWidget(new QLabel("食谱内容:", this));
    layout->addWidget(contentEdit);
    layout->addWidget(new QLabel("图片路径:", this));
    layout->addWidget(videoPathEdit);
    layout->addWidget(selectImageButton);
    layout->addWidget(new QLabel("分类:", this));
    layout->addWidget(categoryCombo);
    layout->addWidget(new QLabel("类型:", this));
    layout->addWidget(typeCombo);
    layout->addWidget(uploadButton);

    setLayout(layout);
}

void UploadRecipeView::onSelectImageClicked() {
    QString filePath = QFileDialog::getOpenFileName(this, "选择图片文件", "", "图片文件 (*.jpg *.png *.bmp)");
    if (!filePath.isEmpty()) {
        QString destDir = QDir::currentPath() + "/res/";
        QDir().mkpath(destDir); // 确保目录存在
        QString destPath = destDir + QFileInfo(filePath).fileName();
        if (QFile::copy(filePath, destPath)) {
            videoPathEdit->setText(destPath); // 将图片路径设置到输入框
        } else {
            QMessageBox::warning(this, "错误", "图片保存失败！");
        }
    }
}

void UploadRecipeView::onUploadClicked() {
    QString title = titleEdit->text();
    QString content = contentEdit->toPlainText();
    QString imagePath = videoPathEdit->text(); // 使用 videoPathEdit 存储图片路径
    QString category = categoryCombo->currentText();
    QString type = typeCombo->currentText();

    if (title.isEmpty() || content.isEmpty()) {
        QMessageBox::warning(this, "警告", "标题和内容不能为空！");
        return;
    }

    if (imagePath.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择图片文件！");
        return;
    }

    // 调用 DatabaseManager 添加食谱
    if (DatabaseManager::instance().addRecipe(1, title, content, category, type, imagePath)) { // 假设 user_id 为 1
        QMessageBox::information(this, "成功", "食谱上传成功！");
        titleEdit->clear();
        contentEdit->clear();
        videoPathEdit->clear();
        categoryCombo->setCurrentIndex(0);
        typeCombo->setCurrentIndex(0);
    } else {
        QMessageBox::critical(this, "错误", "食谱上传失败，请稍后重试！");
    }
}