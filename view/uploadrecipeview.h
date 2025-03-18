#ifndef UPLOADRECIPEVIEW_H
#define UPLOADRECIPEVIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "../sql/DatabaseManager.h"

class UploadRecipeView : public QWidget {
    Q_OBJECT

public:
    explicit UploadRecipeView(QWidget *parent = nullptr);

private slots:
    void onUploadClicked(); // 上传按钮点击事件

private:
    QLineEdit *titleEdit;       // 食谱标题输入框
    QTextEdit *contentEdit;     // 食谱内容输入框
    QLineEdit *videoPathEdit;   // 视频路径输入框
    QComboBox *categoryCombo;   // 分类下拉框
    QComboBox *typeCombo;       // 类型下拉框
    QPushButton *uploadButton;  // 上传按钮

    void setupUI();             // 初始化界面
};

#endif // UPLOADRECIPEVIEW_H