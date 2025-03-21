#include "feedbackview.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QHeaderView>

FeedbackView::FeedbackView(int userId, QWidget *parent)
    : QMainWindow(parent), userId(userId) {
    setupUI();
    loadFeedbacks(); // 加载用户已提交的反馈内容
}

void FeedbackView::setupUI() {
    // 设置窗口标题和大小
    setWindowTitle("反馈界面");
    resize(800, 600);

    // 创建一个中心部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建布局
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // 创建表格
    feedbackTable = new QTableWidget(this);
    feedbackTable->setColumnCount(3);
    feedbackTable->setHorizontalHeaderLabels({"反馈内容", "反馈状态", "提交时间"});
    feedbackTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    feedbackTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑
    feedbackTable->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选择
    layout->addWidget(feedbackTable);

    // 创建输入框
    feedbackInput = new QLineEdit(this);
    feedbackInput->setPlaceholderText("请输入您的反馈...");
    feedbackInput->setStyleSheet("padding: 10px; border: 1px solid #ccc; border-radius: 5px;");
    layout->addWidget(feedbackInput);

    // 创建提交按钮
    submitFeedbackButton = new QPushButton("提交反馈", this);
    submitFeedbackButton->setStyleSheet("padding: 10px; background-color: #4CAF50; color: white; border-radius: 5px;");
    connect(submitFeedbackButton, &QPushButton::clicked, this, &FeedbackView::onSubmitFeedbackClicked);
    layout->addWidget(submitFeedbackButton);

    centralWidget->setLayout(layout);
}

void FeedbackView::loadFeedbacks() {
    feedbackTable->setRowCount(0); // 清空表格

    QSqlQuery query;
    query.prepare("SELECT content, status, created_at FROM feedback WHERE user_id = :user_id ORDER BY created_at DESC");
    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        qDebug() << "加载反馈失败:" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        feedbackTable->insertRow(row);

        // 显示反馈内容
        feedbackTable->setItem(row, 0, new QTableWidgetItem(query.value("content").toString()));

        // 显示反馈状态
        feedbackTable->setItem(row, 1, new QTableWidgetItem(query.value("status").toString()));

        // 显示提交时间
        feedbackTable->setItem(row, 2, new QTableWidgetItem(query.value("created_at").toString()));

        row++;
    }
}

void FeedbackView::onSubmitFeedbackClicked() {
    QString feedbackContent = feedbackInput->text().trimmed();

    if (feedbackContent.isEmpty()) {
        QMessageBox::warning(this, "警告", "反馈内容不能为空！");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO feedback (user_id, content, status, created_at) "
                  "VALUES (:user_id, :content, 'pending', CURRENT_TIMESTAMP)");
    query.bindValue(":user_id", userId);
    query.bindValue(":content", feedbackContent);

    if (!query.exec()) {
        qDebug() << "提交反馈失败:" << query.lastError().text();
        QMessageBox::critical(this, "错误", "提交反馈失败，请稍后重试！");
        return;
    }

    QMessageBox::information(this, "成功", "反馈提交成功！");
    feedbackInput->clear();
    loadFeedbacks(); // 重新加载反馈内容
}
