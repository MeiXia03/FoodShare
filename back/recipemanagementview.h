#ifndef RECIPEMANAGEMENTVIEW_H
#define RECIPEMANAGEMENTVIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class RecipeManagementView : public QWidget {
    Q_OBJECT

public:
    explicit RecipeManagementView(QWidget *parent = nullptr);

private slots:
    void onSearchClicked(); // 搜索按钮点击事件
    void onDeleteButtonClicked(int row); // 删除按钮点击事件
    void onContentDoubleClicked(int row, int column); // 双击内容列事件

private:
    QLineEdit *searchBox; // 搜索框
    QPushButton *searchButton; // 搜索按钮
    QTableWidget *resultTable; // 显示搜索结果的表格

    void setupUI(); // 初始化界面
    void loadResults(const QString &keyword); // 加载搜索结果
    void showContentDialog(const QString &content); // 显示内容弹窗
    bool deleteRecipeFromDatabase(int recipeId); // 从数据库删除菜谱
    bool deleteFile(const QString &filePath); // 删除文件
};

#endif // RECIPEMANAGEMENTVIEW_H