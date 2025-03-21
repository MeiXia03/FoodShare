#include "RecipeCommentsView.h"
#include <QHeaderView>
#include <QLabel>
#include <QTextEdit>

RecipeCommentsView::RecipeCommentsView(int recipeId, QWidget *parent)
    : QDialog(parent), recipeId(recipeId) {
    setupUI();
    loadComments(); // 加载评论数据
}

void RecipeCommentsView::setupUI() {
    setWindowTitle("菜谱评论");
    resize(600, 400);

    commentsTable = new QTableWidget(this);
    commentsTable->setColumnCount(2);
    commentsTable->setHorizontalHeaderLabels({"评论内容", "操作"});
    commentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    commentsTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑
    commentsTable->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选择

    connect(commentsTable, &QTableWidget::cellDoubleClicked, this, &RecipeCommentsView::onCommentDoubleClicked);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(commentsTable);
    setLayout(layout);
}

void RecipeCommentsView::loadComments() {
    commentsTable->setRowCount(0); // 清空表格

    QSqlQuery query;
    query.prepare("SELECT comment_id, content, created_at FROM comments WHERE recipe_id = :recipe_id ORDER BY created_at DESC");
    query.bindValue(":recipe_id", recipeId);

    if (!query.exec()) {
        qDebug() << "加载评论失败:" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        commentsTable->insertRow(row);

        // 显示评论内容
        QTableWidgetItem *contentItem = new QTableWidgetItem(query.value("content").toString());
        contentItem->setData(Qt::UserRole, query.value("comment_id").toInt()); // 绑定 comment_id
        commentsTable->setItem(row, 0, contentItem);

        // 创建删除按钮
        QPushButton *deleteButton = new QPushButton("删除", this);
        deleteButton->setStyleSheet("padding: 5px 10px; background-color: #f44336; color: white; border-radius: 5px;");
        commentsTable->setCellWidget(row, 1, deleteButton);

        // 连接删除按钮的点击事件
        connect(deleteButton, &QPushButton::clicked, [this, row]() {
            onDeleteButtonClicked(row);
        });

        row++;
    }
}

void RecipeCommentsView::onDeleteButtonClicked(int row) {
    QTableWidgetItem *contentItem = commentsTable->item(row, 0);
    if (!contentItem) {
        qWarning() << "删除失败：评论内容项为空";
        return;
    }

    int commentId = contentItem->data(Qt::UserRole).toInt();

    // 弹出确认对话框
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除",
                                  "确定要删除这条评论吗？",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) {
        return; // 用户取消删除
    }

    // 删除数据库记录
    if (!deleteCommentFromDatabase(commentId)) {
        QMessageBox::critical(this, "错误", "删除评论失败！");
        return;
    }

    // 从表格中移除行
    commentsTable->removeRow(row);
    QMessageBox::information(this, "成功", "评论已成功删除！");
}

bool RecipeCommentsView::deleteCommentFromDatabase(int commentId) {
    QSqlQuery query;
    query.prepare("DELETE FROM comments WHERE comment_id = :comment_id");
    query.bindValue(":comment_id", commentId);

    if (!query.exec()) {
        qDebug() << "删除评论失败:" << query.lastError().text();
        return false;
    }
    return true;
}

void RecipeCommentsView::onCommentDoubleClicked(int row, int column) {
    if (column == 0) { // 双击评论内容列
        QString content = commentsTable->item(row, column)->text();

        // 创建一个对话框显示评论内容
        QDialog dialog(this);
        dialog.setWindowTitle("评论内容");
        dialog.setMinimumSize(400, 300);

        QVBoxLayout *layout = new QVBoxLayout(&dialog);

        QLabel *titleLabel = new QLabel("评论内容", &dialog);
        titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 10px;");
        layout->addWidget(titleLabel);

        QTextEdit *contentEdit = new QTextEdit(&dialog);
        contentEdit->setText(content);
        contentEdit->setReadOnly(true);
        contentEdit->setStyleSheet("font-size: 14px; padding: 10px; border: 1px solid #ccc; border-radius: 5px;");
        layout->addWidget(contentEdit);

        QPushButton *closeButton = new QPushButton("关闭", &dialog);
        closeButton->setStyleSheet("padding: 5px 10px; background-color: #4CAF50; color: white; border-radius: 5px;");
        connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);
        layout->addWidget(closeButton);

        dialog.exec();
    }
}
