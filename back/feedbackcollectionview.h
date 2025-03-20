#ifndef FEEDBACKCOLLECTIONVIEW_H
#define FEEDBACKCOLLECTIONVIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class FeedbackCollectionView : public QWidget {
    Q_OBJECT

public:
    explicit FeedbackCollectionView(QWidget *parent = nullptr);

private slots:
    void onSearchClicked(); // 搜索按钮点击事件
    void onConfirmButtonClicked(int row); // 确认按钮点击事件
    void onContentDoubleClicked(int row, int column); // 双击内容列事件

private:
    QLineEdit *searchBox; // 搜索框
    QPushButton *searchButton; // 搜索按钮
    QTableWidget *resultTable; // 显示反馈结果的表格

    void setupUI(); // 初始化界面
    void loadResults(const QString &keyword); // 加载搜索结果
    void showContentDialog(const QString &content); // 显示反馈内容弹窗
    bool updateFeedbackStatus(int feedbackId); // 更新反馈状态
};

#endif // FEEDBACKCOLLECTIONVIEW_H