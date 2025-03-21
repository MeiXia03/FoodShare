#include "recipemanagementview.h"
#include "RecipeCommentsView.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QFile>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>

RecipeManagementView::RecipeManagementView(QWidget *parent) : QWidget(parent) {
    setupUI();

    onSearchClicked();
}

void RecipeManagementView::setupUI() {
    // 创建搜索框
    searchBox = new QLineEdit(this);
    searchBox->setPlaceholderText("输入关键字搜索菜谱...");
    searchBox->setStyleSheet("padding: 5px; border-radius: 5px; border: 1px solid #ccc;");

    // 创建搜索按钮
    searchButton = new QPushButton("搜索", this);
    searchButton->setStyleSheet("padding: 5px 10px; background-color: #03A9F4; color: white; border-radius: 5px;");
    connect(searchButton, &QPushButton::clicked, this, &RecipeManagementView::onSearchClicked);

    // 创建结果表格
    resultTable = new QTableWidget(this);
    resultTable->setColumnCount(4);
    resultTable->setHorizontalHeaderLabels({"标题", "内容", "点赞数", "删除"});
    resultTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    resultTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑
    resultTable->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选择

    // 连接双击事件
    connect(resultTable, &QTableWidget::cellDoubleClicked, this, &RecipeManagementView::onContentDoubleClicked);

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(searchBox);
    layout->addWidget(searchButton);
    layout->addWidget(resultTable);
    setLayout(layout);
}

void RecipeManagementView::onSearchClicked() {
    QString keyword = searchBox->text();
    loadResults(keyword);
}

void RecipeManagementView::loadResults(const QString &keyword) {
    resultTable->setRowCount(0); // 清空表格

    QSqlQuery query;
    QString sql = "SELECT recipe_id, title, content, likes, video_path FROM recipes "
                  "WHERE title LIKE :keyword";
    query.prepare(sql);
    query.bindValue(":keyword", "%" + keyword + "%");

    if (!query.exec()) {
        qDebug() << "搜索失败:" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        resultTable->insertRow(row);

        // 显示标题
        QTableWidgetItem *titleItem = new QTableWidgetItem(query.value("title").toString());
        titleItem->setData(Qt::UserRole, query.value("recipe_id").toInt()); // 绑定 recipe_id
        resultTable->setItem(row, 0, titleItem);

        // 显示内容
        resultTable->setItem(row, 1, new QTableWidgetItem(query.value("content").toString()));

        // 显示点赞数
        resultTable->setItem(row, 2, new QTableWidgetItem(query.value("likes").toString()));

        // 创建删除按钮
        QPushButton *deleteButton = new QPushButton("删除", this);
        deleteButton->setStyleSheet("padding: 5px 10px; background-color: #f44336; color: white; border-radius: 5px;");
        resultTable->setCellWidget(row, 3, deleteButton);

        // 连接删除按钮的点击事件
        connect(deleteButton, &QPushButton::clicked, [this, row]() {
            onDeleteButtonClicked(row);
        });

        row++;
    }
}

void RecipeManagementView::onContentDoubleClicked(int row, int column) {
    if (column == 0) { // 双击标题列
        int recipeId = resultTable->item(row, column)->data(Qt::UserRole).toInt();

        // 打开评论窗口
        RecipeCommentsView *commentsView = new RecipeCommentsView(recipeId, this);
        commentsView->setWindowModality(Qt::ApplicationModal); // 设置为模态窗口
        commentsView->exec(); // 显示模态窗口
    } else if (column == 1) { // 双击内容列
        QString content = resultTable->item(row, column)->text();

        // 打开内容窗口
        showContentDialog(content);
    }
}

void RecipeManagementView::showContentDialog(const QString &content) {
    // 创建对话框
    QDialog dialog(this);
    dialog.setWindowTitle("菜谱内容");
    dialog.setMinimumSize(600, 400);

    // 创建布局
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // 创建标题标签
    QLabel *titleLabel = new QLabel("菜谱介绍", &dialog);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 10px;");
    layout->addWidget(titleLabel);

    // 创建内容显示区域
    QTextEdit *contentEdit = new QTextEdit(&dialog);
    contentEdit->setText(content);
    contentEdit->setReadOnly(true);
    contentEdit->setStyleSheet("font-size: 14px; padding: 10px; border: 1px solid #ccc; border-radius: 5px;");
    layout->addWidget(contentEdit);

    // 创建关闭按钮
    QPushButton *closeButton = new QPushButton("关闭", &dialog);
    closeButton->setStyleSheet("padding: 5px 10px; background-color: #4CAF50; color: white; border-radius: 5px;");
    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout->addWidget(closeButton);

    // 显示对话框
    dialog.exec();
}

void RecipeManagementView::onDeleteButtonClicked(int row) {
    QTableWidgetItem *titleItem = resultTable->item(row, 0);
    if (!titleItem) {
        qWarning() << "删除失败：标题项为空";
        return;
    }

    int recipeId = titleItem->data(Qt::UserRole).toInt();
    QString videoPath = resultTable->item(row, 1)->data(Qt::UserRole).toString();
    QString recipeTitle = titleItem->text();

    // 弹出确认对话框
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除", 
                                  QString("确定要删除菜谱 '%1' 吗？").arg(recipeTitle),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) {
        return; // 用户取消删除
    }

    // 删除数据库记录
    if (!deleteRecipeFromDatabase(recipeId)) {
        QMessageBox::critical(this, "错误", "删除菜谱失败！");
        return;
    }

    // 删除文件
    if (!videoPath.isEmpty() && !deleteFile(videoPath)) {
        QMessageBox::warning(this, "警告", "菜谱已删除，但文件删除失败！");
    }

    // 从表格中移除行
    resultTable->removeRow(row);
    QMessageBox::information(this, "成功", "菜谱已成功删除！");
}

bool RecipeManagementView::deleteRecipeFromDatabase(int recipeId) {
    QSqlQuery query;
    query.prepare("DELETE FROM recipes WHERE recipe_id = :recipe_id");
    query.bindValue(":recipe_id", recipeId);

    if (!query.exec()) {
        qDebug() << "删除菜谱失败:" << query.lastError().text();
        return false;
    }
    return true;
}

bool RecipeManagementView::deleteFile(const QString &filePath) {
    QFile file(filePath);
    if (file.exists()) {
        return file.remove();
    }
    return true; // 如果文件不存在，视为成功
}
