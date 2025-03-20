#include "feedbackcollectionview.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>

FeedbackCollectionView::FeedbackCollectionView(QWidget *parent) : QWidget(parent) {
    setupUI();

    // 默认加载所有反馈
    onSearchClicked();
}

void FeedbackCollectionView::setupUI() {
    // 创建搜索框
    searchBox = new QLineEdit(this);
    searchBox->setPlaceholderText("输入用户名搜索反馈...");
    searchBox->setStyleSheet("padding: 5px; border-radius: 5px; border: 1px solid #ccc;");

    // 创建搜索按钮
    searchButton = new QPushButton("搜索", this);
    searchButton->setStyleSheet("padding: 5px 10px; background-color: #03A9F4; color: white; border-radius: 5px;");
    connect(searchButton, &QPushButton::clicked, this, &FeedbackCollectionView::onSearchClicked);

    // 创建结果表格
    resultTable = new QTableWidget(this);
    resultTable->setColumnCount(5);
    resultTable->setHorizontalHeaderLabels({"用户名", "反馈内容", "反馈状态", "提交时间", "确认"});
    resultTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    resultTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑
    resultTable->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选择

    // 连接双击事件
    connect(resultTable, &QTableWidget::cellDoubleClicked, this, &FeedbackCollectionView::onContentDoubleClicked);

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(searchBox);
    layout->addWidget(searchButton);
    layout->addWidget(resultTable);
    setLayout(layout);
}

void FeedbackCollectionView::onSearchClicked() {
    QString keyword = searchBox->text();
    loadResults(keyword);
}

void FeedbackCollectionView::loadResults(const QString &keyword) {
    resultTable->setRowCount(0); // 清空表格

    QSqlQuery query;
    QString sql = "SELECT feedback.feedback_id, users.username, feedback.content, feedback.status, feedback.created_at "
                  "FROM feedback "
                  "JOIN users ON feedback.user_id = users.user_id "
                  "WHERE users.username LIKE :keyword";
    query.prepare(sql);
    query.bindValue(":keyword", "%" + keyword + "%");

    if (!query.exec()) {
        qDebug() << "搜索失败:" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        resultTable->insertRow(row);

        // 显示用户名
        resultTable->setItem(row, 0, new QTableWidgetItem(query.value("username").toString()));

        // 显示反馈内容
        QTableWidgetItem *contentItem = new QTableWidgetItem(query.value("content").toString());
        contentItem->setData(Qt::UserRole, query.value("feedback_id").toInt()); // 绑定 feedback_id
        resultTable->setItem(row, 1, contentItem);

        // 显示反馈状态
        resultTable->setItem(row, 2, new QTableWidgetItem(query.value("status").toString()));

        // 显示提交时间
        resultTable->setItem(row, 3, new QTableWidgetItem(query.value("created_at").toString()));

        // 创建确认按钮
        QPushButton *confirmButton = new QPushButton("确认", this);
        confirmButton->setStyleSheet("padding: 5px 10px; background-color: #4CAF50; color: white; border-radius: 5px;");
        resultTable->setCellWidget(row, 4, confirmButton);

        // 连接确认按钮的点击事件
        connect(confirmButton, &QPushButton::clicked, [this, row]() {
            onConfirmButtonClicked(row);
        });

        row++;
    }
}

void FeedbackCollectionView::onContentDoubleClicked(int row, int column) {
    if (column == 1) { // 双击反馈内容列
        QString content = resultTable->item(row, column)->text();
        showContentDialog(content);
    }
}

void FeedbackCollectionView::showContentDialog(const QString &content) {
    QDialog dialog(this);
    dialog.setWindowTitle("反馈内容");
    dialog.setMinimumSize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *contentLabel = new QLabel("反馈内容", &dialog);
    contentLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 10px;");
    layout->addWidget(contentLabel);

    QTextEdit *contentEdit = new QTextEdit(&dialog);
    contentEdit->setText(content);
    contentEdit->setReadOnly(true);
    contentEdit->setStyleSheet("font-size: 14px; padding: 10px; border: 1px solid #ccc; border-radius: 5px;");
    layout->addWidget(contentEdit);

    QPushButton *closeButton = new QPushButton("关闭", &dialog);
    closeButton->setStyleSheet("padding: 5px 10px; background-color: #03A9F4; color: white; border-radius: 5px;");
    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout->addWidget(closeButton);

    dialog.exec();
}

void FeedbackCollectionView::onConfirmButtonClicked(int row) {
    QTableWidgetItem *contentItem = resultTable->item(row, 1);
    if (!contentItem) {
        qWarning() << "确认失败：反馈内容项为空";
        return;
    }

    int feedbackId = contentItem->data(Qt::UserRole).toInt();

    // 更新反馈状态
    if (!updateFeedbackStatus(feedbackId)) {
        QMessageBox::critical(this, "错误", "确认反馈失败！");
        return;
    }

    // 更新表格中的状态
    resultTable->setItem(row, 2, new QTableWidgetItem("accepted"));
    QMessageBox::information(this, "成功", "反馈已确认！");
}

bool FeedbackCollectionView::updateFeedbackStatus(int feedbackId) {
    QSqlQuery query;
    query.prepare("UPDATE feedback SET status = 'accepted' WHERE feedback_id = :feedback_id");
    query.bindValue(":feedback_id", feedbackId);

    if (!query.exec()) {
        qDebug() << "更新反馈状态失败:" << query.lastError().text();
        return false;
    }
    return true;
}