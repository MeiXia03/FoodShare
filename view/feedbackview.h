#ifndef FEEDBACKVIEW_H
#define FEEDBACKVIEW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class FeedbackView : public QMainWindow {
    Q_OBJECT

public:
    explicit FeedbackView(int userId, QWidget *parent = nullptr);

private slots:
    void onSubmitFeedbackClicked(); // 提交反馈按钮点击事件

private:
    int userId; // 当前用户ID
    QTableWidget *feedbackTable; // 显示反馈内容的表格
    QLineEdit *feedbackInput; // 输入反馈内容的输入框
    QPushButton *submitFeedbackButton; // 提交反馈按钮

    void setupUI(); // 初始化界面
    void loadFeedbacks(); // 加载用户已提交的反馈内容
};

#endif // FEEDBACKVIEW_H