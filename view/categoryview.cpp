#include "CategoryView.h"
#include "../sql/DatabaseManager.h"
#include <QHeaderView>
#include <QSqlError>
#include <QDebug>

CategoryView::CategoryView(QWidget *parent) : QWidget(parent) {
    if (!DatabaseManager::instance().connect()) {
        qDebug() << "数据库连接失败";
        return;
    }
    setupUI();
    loadCategories();
    loadTypes();
    loadRecipes("", ""); // 默认加载所有食谱
}

void CategoryView::setupUI() {
    // 创建分类选择框
    categoryComboBox = new QComboBox(this);
    categoryComboBox->addItem("全部分类", ""); // 默认选项
    connect(categoryComboBox, &QComboBox::currentTextChanged, this, &CategoryView::onFilterChanged);

    // 创建类型选择框
    typeComboBox = new QComboBox(this);
    typeComboBox->addItem("全部类型", ""); // 默认选项
    connect(typeComboBox, &QComboBox::currentTextChanged, this, &CategoryView::onFilterChanged);

    // 创建结果表格
    resultTable = new QTableWidget(this);
    resultTable->setColumnCount(4); // 显示标题、分类、类型、点赞数
    resultTable->setHorizontalHeaderLabels({"标题", "分类", "类型", "点赞数"});
    resultTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    resultTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑
    resultTable->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选择

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(categoryComboBox);
    layout->addWidget(typeComboBox);
    layout->addWidget(resultTable);
    setLayout(layout);
}

void CategoryView::refreshData() {
    categoryComboBox->clear();
    typeComboBox->clear();
    resultTable->setRowCount(0);

    categoryComboBox->addItem("全部分类", ""); // 默认选项
    typeComboBox->addItem("全部类型", "");   // 默认选项

    loadCategories();
    loadTypes();
    loadRecipes("", ""); // 默认加载所有食谱
}

void CategoryView::loadCategories() {
    QSqlQuery query;
    if (!query.exec("SELECT DISTINCT category FROM recipes WHERE category IS NOT NULL")) {
        qDebug() << "加载分类失败:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QString category = query.value(0).toString();
        categoryComboBox->addItem(category, category);
    }
}

void CategoryView::loadTypes() {
    QSqlQuery query;
    if (!query.exec("SELECT DISTINCT type FROM recipes WHERE type IS NOT NULL")) {
        qDebug() << "加载类型失败:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QString type = query.value(0).toString();
        typeComboBox->addItem(type, type);
    }
}

void CategoryView::loadRecipes(const QString &category, const QString &type) {
    resultTable->setRowCount(0); // 清空表格

    QSqlQuery query;
    QString sql = "SELECT title, category, type, likes FROM recipes WHERE 1=1";
    if (!category.isEmpty()) {
        sql += " AND category = :category";
    }
    if (!type.isEmpty()) {
        sql += " AND type = :type";
    }

    query.prepare(sql);
    if (!category.isEmpty()) {
        query.bindValue(":category", category);
    }
    if (!type.isEmpty()) {
        query.bindValue(":type", type);
    }

    if (!query.exec()) {
        qDebug() << "加载食谱失败:" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        resultTable->insertRow(row);
        resultTable->setItem(row, 0, new QTableWidgetItem(query.value("title").toString()));
        resultTable->setItem(row, 1, new QTableWidgetItem(query.value("category").toString()));
        resultTable->setItem(row, 2, new QTableWidgetItem(query.value("type").toString()));
        resultTable->setItem(row, 3, new QTableWidgetItem(query.value("likes").toString()));
        row++;
    }
}

void CategoryView::onFilterChanged() {
    QString category = categoryComboBox->currentData().toString();
    QString type = typeComboBox->currentData().toString();
    loadRecipes(category, type);
}
