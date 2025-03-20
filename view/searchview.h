#ifndef SEARCHVIEW_H
#define SEARCHVIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "communityview.h" // 引入 CommunityView


class SearchView : public QWidget {
    Q_OBJECT

public:
    explicit SearchView(QWidget *parent = nullptr);

private slots:
    void onSearchClicked(); // 搜索按钮点击事件
    void onLikeButtonClicked(int row); // 点赞按钮点击事件
    void onCommentButtonClicked(int row); // 评论按钮点击事件
    void onContentDoubleClicked(int row, int column); // 双击内容栏位事件

private:
    QLineEdit *searchBox; // 搜索框
    QPushButton *searchButton; // 搜索按钮
    QTableWidget *resultTable; // 显示搜索结果的表格
    CommunityView *communityView = nullptr;
    
    void setupUI(); // 初始化界面
    void loadResults(const QString &keyword); // 加载搜索结果
    void showContentDialog(const QString &content); // 显示美化后的内容弹窗
    void animateLikeButton(QPushButton *button); // 点赞按钮动画效果
    
};

#endif // SEARCHVIEW_H
