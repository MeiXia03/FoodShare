#include "SearchView.h"
#include "../sql/DatabaseManager.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QTextEdit>
#include <QPropertyAnimation>
#include <QTimer>

SearchView::SearchView(QWidget *parent) : QWidget(parent) {
    if (!DatabaseManager::instance().connect()) {
        qDebug() << "数据库连接失败";
        return;
    }
    setupUI();

    onSearchClicked();
}

void SearchView::refreshData() {
    // 调用搜索方法，刷新数据
    onSearchClicked();
}

void SearchView::setupUI() {
    // 创建搜索框
    searchBox = new QLineEdit(this);
    searchBox->setPlaceholderText("输入关键字搜索食谱...");
    searchBox->setStyleSheet("padding: 5px; border-radius: 5px; border: 1px solid #ccc;");

    // 创建搜索按钮
    searchButton = new QPushButton("搜索", this);
    searchButton->setStyleSheet("padding: 5px 10px; background-color: #ff5722; color: white; border-radius: 5px;");
    connect(searchButton, &QPushButton::clicked, this, &SearchView::onSearchClicked);

    // 创建结果表格
    resultTable = new QTableWidget(this);
    resultTable->setColumnCount(5);
    resultTable->setHorizontalHeaderLabels({"标题", "内容", "点赞数", "点赞", "评论"});
    resultTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    resultTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑
    resultTable->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选择

    // 连接双击事件
    connect(resultTable, &QTableWidget::cellDoubleClicked, this, &SearchView::onContentDoubleClicked);

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(searchBox);
    layout->addWidget(searchButton);
    layout->addWidget(resultTable);
    setLayout(layout);
}

void SearchView::onSearchClicked() {
    QString keyword = searchBox->text();
    loadResults(keyword);
}

void SearchView::onContentDoubleClicked(int row, int column) {
    // 检查是否双击了内容列
    if (column == 1) { // 内容列的索引为 1
        QString content = resultTable->item(row, column)->text();
        showContentDialog(content); // 调用美化后的弹窗
    }
}

void SearchView::showContentDialog(const QString &content) {
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

void SearchView::onCommentButtonClicked(int row) {
    // 获取当前行的 recipe_id
    QTableWidgetItem *item = resultTable->item(row, 0);
    if (!item) {
        qWarning() << "Invalid row or column";
        return;
    }

    int recipeId = item->data(Qt::UserRole).toInt();
    int userId = 1; // 假设当前登录用户ID为1

    // 创建或激活 CommunityView 窗口
    if (!communityView) {
        communityView = new CommunityView(recipeId, userId, this);

        // 设置为独立窗口
        communityView->setWindowFlags(Qt::Window);
        communityView->setWindowModality(Qt::NonModal);
        communityView->setAttribute(Qt::WA_DeleteOnClose);

        // 设置窗口属性
        communityView->setWindowTitle(QString("社区评论 - 食谱 ID: %1").arg(recipeId));
        communityView->resize(800, 600);

        // 窗口关闭时自动置空指针
        connect(communityView, &CommunityView::destroyed, this, [this]() {
            communityView = nullptr;
        });
    }

    // 显示窗口
    communityView->show();
    communityView->raise();
    communityView->activateWindow();
}

void SearchView::loadResults(const QString &keyword) {
    resultTable->setRowCount(0); // 清空表格

    QSqlQuery query;
    QString sql = "SELECT recipe_id, title, content, likes FROM recipes "
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

        // 创建点赞按钮
        QPushButton *likeButton = new QPushButton("点赞", this);
        likeButton->setStyleSheet("padding: 5px 10px; background-color: #4CAF50; color: white; border-radius: 5px;");
        resultTable->setCellWidget(row, 3, likeButton);

        // 连接点赞按钮的点击事件
        connect(likeButton, &QPushButton::clicked, [this, row]() {
            onLikeButtonClicked(row);
        });

        // 创建评论按钮
        QPushButton *commentButton = new QPushButton("评论", this);
        commentButton->setStyleSheet("padding: 5px 10px; background-color: #2196F3; color: white; border-radius: 5px;");
        resultTable->setCellWidget(row, 4, commentButton);

        // 连接评论按钮的点击事件
        connect(commentButton, &QPushButton::clicked, [this, row]() {
            onCommentButtonClicked(row);
        });

        row++;
    }
}

void SearchView::animateLikeButton(QPushButton *button) {
    // 设置按钮颜色变化
    button->setStyleSheet("background-color: #FF5722; color: white; border-radius: 5px;");
    QTimer::singleShot(200, [button]() {
        button->setStyleSheet("background-color: #4CAF50; color: white; border-radius: 5px;");
    });

    // 创建缩放动画
    QPropertyAnimation *animation = new QPropertyAnimation(button, "geometry");
    QRect originalGeometry = button->geometry();
    animation->setDuration(200);
    animation->setStartValue(originalGeometry);
    animation->setKeyValueAt(0.5, QRect(originalGeometry.x() - 5, originalGeometry.y() - 5,
                                        originalGeometry.width() + 10, originalGeometry.height() + 10));
    animation->setEndValue(originalGeometry);
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void SearchView::onLikeButtonClicked(int row) {
    // 获取当前行的标题
    QTableWidgetItem *titleItem = resultTable->item(row, 0);
    if (!titleItem) {
        qWarning() << "点赞失败：标题项为空";
        return;
    }

    QString title = titleItem->text();

    // 更新数据库中的点赞数
    QSqlQuery query;
    query.prepare("UPDATE recipes SET likes = likes + 1 WHERE title = :title");
    query.bindValue(":title", title);

    if (!query.exec()) {
        qDebug() << "点赞失败:" << query.lastError().text();
        return;
    }

    // 更新表格中的点赞数
    QTableWidgetItem *likesItem = resultTable->item(row, 2); // 点赞数的列索引是 2
    if (!likesItem) {
        qWarning() << "点赞失败：点赞数项为空";
        return;
    }

    int currentLikes = likesItem->text().toInt();
    likesItem->setText(QString::number(currentLikes + 1));

    // 获取点赞按钮
    QPushButton *likeButton = qobject_cast<QPushButton *>(resultTable->cellWidget(row, 3));
    if (likeButton) {
        animateLikeButton(likeButton); // 调用动画效果
    }

    qDebug() << "点赞成功，标题:" << title;
}


