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
    resultTable->setColumnCount(5); 
    resultTable->setHorizontalHeaderLabels({"用户名", "头像路径", "个性签名", "删除", "封禁"});
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

        int userId = query.value("user_id").toInt();

        // 创建删除按钮
        QPushButton *deleteButton = new QPushButton("删除", this);
        deleteButton->setStyleSheet("padding: 5px 10px; background-color: #f44336; color: white; border-radius: 5px;");
        resultTable->setCellWidget(row, 3, deleteButton);

        // 连接删除按钮的点击事件
        connect(deleteButton, &QPushButton::clicked, [this, row]() {
            onDeleteButtonClicked(row);
        });
        
        // 检查用户是否在黑名单中
        QSqlQuery blacklistQuery;
        blacklistQuery.prepare("SELECT block_id FROM blacklist WHERE user_id = :user_id");
        blacklistQuery.bindValue(":user_id", userId);

        bool isBlocked = false;
        if (blacklistQuery.exec() && blacklistQuery.next()) {
            isBlocked = true;
        }

        // 创建封禁/取消封禁按钮
        QPushButton *blockButton = new QPushButton(this);
        blockButton->setText(isBlocked ? "取消封禁" : "封禁");
        blockButton->setStyleSheet(isBlocked
                                   ? "padding: 5px 10px; background-color: #4CAF50; color: white; border-radius: 5px;"
                                   : "padding: 5px 10px; background-color: #FF9800; color: white; border-radius: 5px;");
        resultTable->setCellWidget(row, 4, blockButton);

        // 连接封禁/取消封禁按钮的点击事件
        connect(blockButton, &QPushButton::clicked, [this, row, isBlocked]() {
            if (isBlocked) {
                onUnblockButtonClicked(row); // 取消封禁
            } else {
                onBlockButtonClicked(row); // 封禁
            }
        });

        row++;
    }
}

bool UserManagementView::blockUserInDatabase(int userId, const QString &reason) {
    QSqlQuery query;
    query.prepare("INSERT INTO blacklist (user_id, admin_id, reason, created_at) "
                  "VALUES (:user_id, :admin_id, :reason, CURRENT_TIMESTAMP)");
    query.bindValue(":user_id", userId);
    query.bindValue(":admin_id", 2);
    query.bindValue(":reason", reason);

    if (!query.exec()) {
        qDebug() << "封禁用户失败:" << query.lastError().text();
        return false;
    }
    return true;
}

void UserManagementView::onBlockButtonClicked(int row) {
    QTableWidgetItem *usernameItem = resultTable->item(row, 0);
    if (!usernameItem) {
        qWarning() << "封禁失败：用户名项为空";
        return;
    }

    int userId = usernameItem->data(Qt::UserRole).toInt();
    QString username = usernameItem->text();

    // 弹出输入封禁原因的对话框
    QDialog dialog(this);
    dialog.setWindowTitle("封禁用户");
    dialog.setMinimumSize(400, 200);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *label = new QLabel(QString("请输入封禁用户 '%1' 的原因：").arg(username), &dialog);
    layout->addWidget(label);

    QLineEdit *reasonInput = new QLineEdit(&dialog);
    reasonInput->setPlaceholderText("输入封禁原因...");
    layout->addWidget(reasonInput);

    QPushButton *confirmButton = new QPushButton("确认封禁", &dialog);
    connect(confirmButton, &QPushButton::clicked, [&]() {
        QString reason = reasonInput->text().trimmed();
        if (reason.isEmpty()) {
            QMessageBox::warning(&dialog, "警告", "封禁原因不能为空！");
            return;
        }

        // 将封禁信息存储到黑名单表
        if (blockUserInDatabase(userId, reason)) {
            QMessageBox::information(&dialog, "成功", "用户已成功封禁！");
            dialog.accept();

            // 更新按钮为“取消封禁”
            QPushButton *blockButton = qobject_cast<QPushButton *>(resultTable->cellWidget(row, 4));
            if (blockButton) {
                blockButton->setText("取消封禁");
                blockButton->setStyleSheet("padding: 5px 10px; background-color: #4CAF50; color: white; border-radius: 5px;");
                disconnect(blockButton, &QPushButton::clicked, nullptr, nullptr); // 断开旧的信号连接
                connect(blockButton, &QPushButton::clicked, [this, row]() {
                    onUnblockButtonClicked(row); // 重新连接取消封禁事件
                });
            }
        } else {
            QMessageBox::critical(&dialog, "错误", "封禁用户失败！");
        }
    });
    layout->addWidget(confirmButton);

    QPushButton *cancelButton = new QPushButton("取消", &dialog);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);
    layout->addWidget(cancelButton);

    dialog.exec();
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

    // 检查并删除黑名单表中的记录
    if (!unblockUserFromDatabase(userId)) {
        qDebug() << "黑名单中没有该用户或删除黑名单记录失败";
    } else {
        qDebug() << "黑名单记录已删除";
    }

    // 删除用户记录
    if (!deleteUserFromDatabase(userId)) {
        QMessageBox::critical(this, "错误", "删除用户失败！");
        return;
    }

    // 从表格中移除行
    resultTable->removeRow(row);
    QMessageBox::information(this, "成功", "用户及其黑名单记录已成功删除！");
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

void UserManagementView::onUnblockButtonClicked(int row) {
    QTableWidgetItem *usernameItem = resultTable->item(row, 0);
    if (!usernameItem) {
        qWarning() << "取消封禁失败：用户名项为空";
        return;
    }

    int userId = usernameItem->data(Qt::UserRole).toInt();

    // 弹出确认对话框
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认取消封禁",
                                  "确定要取消封禁该用户吗？",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) {
        return; // 用户取消操作
    }

    // 删除黑名单记录
    if (!unblockUserFromDatabase(userId)) {
        QMessageBox::critical(this, "错误", "取消封禁失败！");
        return;
    }

    // 更新按钮为“封禁”
    QPushButton *blockButton = qobject_cast<QPushButton *>(resultTable->cellWidget(row, 4));
    if (blockButton) {
        blockButton->setText("封禁");
        blockButton->setStyleSheet("padding: 5px 10px; background-color: #FF9800; color: white; border-radius: 5px;");
        disconnect(blockButton, &QPushButton::clicked, nullptr, nullptr); // 断开旧的信号连接
        connect(blockButton, &QPushButton::clicked, [this, row]() {
            onBlockButtonClicked(row); // 重新连接封禁事件
        });
    }

    QMessageBox::information(this, "成功", "用户已成功取消封禁！");
}

bool UserManagementView::unblockUserFromDatabase(int userId) {
    QSqlQuery query;
    query.prepare("DELETE FROM blacklist WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        qDebug() << "取消封禁失败:" << query.lastError().text();
        return false;
    }
    return true;
}
