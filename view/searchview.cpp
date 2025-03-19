#include "SearchView.h"
#include "../sql/DatabaseManager.h"
#include <QHeaderView>
SearchView::SearchView(QWidget *parent) : QWidget(parent) {
    auto placeholder = !DatabaseManager::instance().connect();
    if (placeholder) {
        qDebug() << "数据库连接失败";
        return;
    }
    setupUI();
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
    resultTable->setColumnCount(5); // 显示标题、内容、视频路径、点赞数、操作
    resultTable->setHorizontalHeaderLabels({"标题", "内容", "视频路径", "点赞数", "操作"});
    resultTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    resultTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑
    resultTable->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选择

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

void SearchView::onResultDoubleClicked(int row, int column) {
    Q_UNUSED(column);

    // 获取当前行的 recipe_id
    int recipeId = resultTable->item(row, 0)->data(Qt::UserRole).toInt();
    int userId = 1; // 假设当前登录用户ID为1

    // 弹出 CommunityView 窗口
    CommunityView *communityView = new CommunityView(recipeId, userId);
    communityView->setWindowTitle(QString("社区评论 - 食谱 ID: %1").arg(recipeId));
    communityView->resize(800, 600); // 设置窗口大小
    communityView->show();
}

void SearchView::loadResults(const QString &keyword) {
    resultTable->setRowCount(0); // 清空表格

    QSqlQuery query;
    QString sql = "SELECT recipe_id, title, content, video_path, likes FROM recipes "
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

        // 显示视频路径
        resultTable->setItem(row, 2, new QTableWidgetItem(query.value("video_path").toString()));

        // 显示点赞数
        resultTable->setItem(row, 3, new QTableWidgetItem(query.value("likes").toString()));

        // 创建点赞按钮
        QPushButton *likeButton = new QPushButton("点赞", this);
        likeButton->setStyleSheet("padding: 5px 10px; background-color: #4CAF50; color: white; border-radius: 5px;");
        resultTable->setCellWidget(row, 4, likeButton);

        // 连接点赞按钮的点击事件
        connect(likeButton, &QPushButton::clicked, [this, row]() {
            onLikeButtonClicked(row);
        });

        row++;
    }
}

void SearchView::onLikeButtonClicked(int row) {
    // 获取当前行的 recipe_id
    QString title = resultTable->item(row, 0)->text();

    QSqlQuery query;
    query.prepare("UPDATE recipes SET likes = likes + 1 WHERE title = :title");
    query.bindValue(":title", title);

    if (!query.exec()) {
        qDebug() << "点赞失败:" << query.lastError().text();
        return;
    }

    // 更新点赞数
    int currentLikes = resultTable->item(row, 3)->text().toInt();
    resultTable->setItem(row, 3, new QTableWidgetItem(QString::number(currentLikes + 1)));

    qDebug() << "点赞成功，标题:" << title;
}
