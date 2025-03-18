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

class SearchView : public QWidget {
    Q_OBJECT

public:
    explicit SearchView(QWidget *parent = nullptr);

private slots:
    void onSearchClicked(); // 搜索按钮点击事件
    void onLikeButtonClicked(int row); // 点赞按钮点击事件

private:
    QLineEdit *searchBox; // 搜索框
    QPushButton *searchButton; // 搜索按钮
    QTableWidget *resultTable; // 显示搜索结果的表格

    void setupUI(); // 初始化界面
    void loadResults(const QString &keyword); // 加载搜索结果
};

#endif // SEARCHVIEW_H