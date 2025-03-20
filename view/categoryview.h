#ifndef CATEGORYVIEW_H
#define CATEGORYVIEW_H

#include <QWidget>
#include <QComboBox>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

class CategoryView : public QWidget {
    Q_OBJECT

public:
    explicit CategoryView(QWidget *parent = nullptr);
    void refreshData(); // 添加刷新数据的方法

private slots:
    void onFilterChanged(); // 当筛选条件改变时触发

private:
    QComboBox *categoryComboBox; // 分类选择框
    QComboBox *typeComboBox;     // 类型选择框
    QTableWidget *resultTable;   // 显示查询结果的表格

    void setupUI();              // 初始化界面
    void loadCategories();       // 加载分类数据
    void loadTypes();            // 加载类型数据
    void loadRecipes(const QString &category, const QString &type); // 加载食谱数据
};

#endif // CATEGORYVIEW_H