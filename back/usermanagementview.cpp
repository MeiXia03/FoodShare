#include "usermanagementview.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QFile>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>

UserManagementView::UserManagementView(int userId, QWidget *parent)
    : QWidget(parent), userId(userId) {
    setupUI();

    // 默认加载所有用户
    onSearchClicked();
}

void UserManagementView::setupUI() {
    // 创建搜索框
    searchBox = new QLineEdit(this);
    searchBox->setPlaceholderText("输入用户名搜索用户...");
    searchBox->setStyleSheet("padding: 5px; border-radius: 5px; border: 1px solid #ccc;");

    // 创建搜索按钮
    searchButton = new QPushButton("搜索", this);
    searchButton->setStyleSheet("padding: 5px 10px; background-color: #03A9F4; color: white; border-radius: 5px;");
    connect(searchButton, &QPushButton::clicked, this, &UserManagementView::onSearchClicked);

    // 创建结果表格
    resultTable = new QTableWidget(this);
    resultTable->setColumnCount(4);
    resultTable->setHorizontalHeaderLabels({"用户名", "头像路径", "个性签名", "删除"});
    resultTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    resultTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止编辑
    resultTable->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选择

    // 连接双击事件
    connect(resultTable, &QTableWidget::cellDoubleClicked, this, &UserManagementView::onTableDoubleClicked);

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(searchBox);
    layout->addWidget(searchButton);
    layout->addWidget(resultTable);
    setLayout(layout);
}

void UserManagementView::onSearchClicked() {
    QString keyword = searchBox->text();
    loadResults(keyword);
}

void UserManagementView::loadResults(const QString &keyword) {
    resultTable->setRowCount(0); // 清空表格

    QSqlQuery query;
    QString sql = "SELECT user_id, username, avatar_path, signature FROM users "
                  "WHERE username LIKE :keyword AND user_id != :user_id";
    query.prepare(sql);
    query.bindValue(":keyword", "%" + keyword + "%");
    query.bindValue(":user_id", userId); // 排除当前登录用户

    if (!query.exec()) {
        qDebug() << "搜索失败:" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        resultTable->insertRow(row);

        // 显示用户名
        QTableWidgetItem *usernameItem = new QTableWidgetItem(query.value("username").toString());
        usernameItem->setData(Qt::UserRole, query.value("user_id").toInt()); // 绑定 user_id
        resultTable->setItem(row, 0, usernameItem);

        // 显示头像路径
        resultTable->setItem(row, 1, new QTableWidgetItem(query.value("avatar_path").toString()));

        // 显示个性签名
        resultTable->setItem(row, 2, new QTableWidgetItem(query.value("signature").toString()));

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

void UserManagementView::onTableDoubleClicked(int row, int column) {
    if (column == 1) { // 双击头像路径列
        QString avatarPath = resultTable->item(row, column)->text();
        showAvatarDialog(avatarPath);
    } else if (column == 2) { // 双击个性签名列
        QString signature = resultTable->item(row, column)->text();
        showSignatureDialog(signature);
    }
}

void UserManagementView::showAvatarDialog(const QString &avatarPath) {
    QDialog dialog(this);
    dialog.setWindowTitle("头像预览");
    dialog.setMinimumSize(300, 300);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *imageLabel = new QLabel(&dialog);
    QPixmap pixmap(avatarPath);
    if (pixmap.isNull()) {
        imageLabel->setText("无法加载头像图片！");
    } else {
        imageLabel->setPixmap(pixmap.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    layout->addWidget(imageLabel);

    QPushButton *closeButton = new QPushButton("关闭", &dialog);
    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout->addWidget(closeButton);

    dialog.exec();
}

void UserManagementView::showSignatureDialog(const QString &signature) {
    QDialog dialog(this);
    dialog.setWindowTitle("个性签名");
    dialog.setMinimumSize(400, 200);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *signatureLabel = new QLabel(signature, &dialog);
    signatureLabel->setWordWrap(true);
    layout->addWidget(signatureLabel);

    QPushButton *closeButton = new QPushButton("关闭", &dialog);
    connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout->addWidget(closeButton);

    dialog.exec();
}

void UserManagementView::onDeleteButtonClicked(int row) {
    QTableWidgetItem *usernameItem = resultTable->item(row, 0);
    if (!usernameItem) {
        qWarning() << "删除失败：用户名项为空";
        return;
    }

    int userId = usernameItem->data(Qt::UserRole).toInt();
    QString username = usernameItem->text();

    // 弹出确认对话框
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除",
                                  QString("确定要删除用户 '%1' 吗？").arg(username),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) {
        return; // 用户取消删除
    }

    // 删除数据库记录
    if (!deleteUserFromDatabase(userId)) {
        QMessageBox::critical(this, "错误", "删除用户失败！");
        return;
    }

    // 从表格中移除行
    resultTable->removeRow(row);
    QMessageBox::information(this, "成功", "用户已成功删除！");
}

bool UserManagementView::deleteUserFromDatabase(int userId) {
    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        qDebug() << "删除用户失败:" << query.lastError().text();
        return false;
    }
    return true;
}