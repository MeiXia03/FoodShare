#ifndef UPLOADRECIPEVIEW_H
#define UPLOADRECIPEVIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include "../sql/DatabaseManager.h"

class UploadRecipeView : public QWidget {
    Q_OBJECT

public:
    explicit UploadRecipeView(QWidget *parent = nullptr);

private slots:
    void onUploadClicked();       // 上传按钮点击事件
    void onSelectImageClicked();  // 选择图片文件

private:
    QLineEdit *titleEdit;         // 食谱标题输入框
    QTextEdit *contentEdit;       // 食谱内容输入框
    QLineEdit *videoPathEdit;     // 图片路径输入框（使用 video_path 字段）
    QComboBox *categoryCombo;     // 分类下拉框
    QComboBox *typeCombo;         // 类型下拉框
    QPushButton *uploadButton;    // 上传按钮
    QPushButton *selectImageButton; // 选择图片按钮

    void setupUI();               // 初始化界面
};

#endif // UPLOADRECIPEVIEW_H