#include "DatabaseManager.h"
#include <QSqlError> // 用于处理数据库错误
#include <QDebug>    // 用于输出调试信息
#include <QSqlDatabase> // 提供对数据库连接的支持
#include <QSqlQuery>    // 提供对 SQL 查询的支持

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance; // 静态实例，确保全局唯一
    return instance;
}
// 单例模式的实现，确保整个应用程序中只有一个 DatabaseManager 实例。

DatabaseManager::DatabaseManager() {}
// 构造函数，初始化 DatabaseManager 对象。

DatabaseManager::~DatabaseManager() {
    close();
}
// 析构函数，确保在对象销毁时关闭数据库连接。

bool DatabaseManager::connect(const QString& dbPath) {
    if (m_database.isOpen()) {
        return true; // 如果数据库已经打开，则直接返回 true
    }

    m_database = QSqlDatabase::addDatabase("QSQLITE"); // 添加 SQLite 数据库驱动
    m_database.setDatabaseName(dbPath); // 设置数据库文件路径

    if (!m_database.open()) { // 尝试打开数据库
        qDebug() << "Failed to connect to database:" << m_database.lastError().text(); // 输出错误信息
        return false; // 如果打开失败，返回 false
    }
    return true; // 数据库连接成功，返回 true
}
// 连接到 SQLite 数据库，指定数据库文件路径。如果连接成功返回 true，否则返回 false。

void DatabaseManager::close() {
    if (m_database.isOpen()) {
        m_database.close(); // 如果数据库已打开，则关闭连接
    }
}
// 关闭数据库连接。

bool DatabaseManager::addUser(const QString& username, const QString& password, const QString& avatarPath, const QString& signature) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password, avatar_path, signature) VALUES (:username, :password, :avatar_path, :signature)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.bindValue(":avatar_path", avatarPath);
    query.bindValue(":signature", signature);

    if (!query.exec()) { // 执行插入操作
        qDebug() << "Failed to add user:" << query.lastError().text(); // 输出错误信息
        return false; // 如果插入失败，返回 false
    }
    return true; // 插入成功，返回 true
}
// 添加用户到数据库，插入用户名、密码、头像路径和签名。如果操作成功返回 true，否则返回 false。

bool DatabaseManager::deleteUser(int userId) {
    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);

    if (!query.exec()) { // 执行删除操作
        qDebug() << "Failed to delete user:" << query.lastError().text(); // 输出错误信息
        return false; // 如果删除失败，返回 false
    }
    return true; // 删除成功，返回 true
}
// 根据用户 ID 删除用户。如果操作成功返回 true，否则返回 false。

QVector<QVariantMap> DatabaseManager::getUsers() {
    QVector<QVariantMap> users;
    QSqlQuery query("SELECT * FROM users"); // 查询所有用户信息

    while (query.next()) { // 遍历查询结果
        QVariantMap user;
        user["user_id"] = query.value("user_id");
        user["username"] = query.value("username");
        user["password"] = query.value("password");
        user["avatar_path"] = query.value("avatar_path");
        user["signature"] = query.value("signature");
        user["is_admin"] = query.value("is_admin");
        user["created_at"] = query.value("created_at");
        users.append(user); // 将用户信息添加到结果列表
    }
    return users; // 返回包含所有用户信息的 QVector
}
// 获取所有用户信息，返回一个 QVector，其中每个用户信息存储在 QVariantMap 中。

bool DatabaseManager::updateUser(int userId, const QString& username, const QString& password, const QString& avatarPath, const QString& signature) {
    QSqlQuery query;
    query.prepare("UPDATE users SET username = :username, password = :password, avatar_path = :avatar_path, signature = :signature WHERE user_id = :user_id");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.bindValue(":avatar_path", avatarPath);
    query.bindValue(":signature", signature);
    query.bindValue(":user_id", userId);

    if (!query.exec()) { // 执行更新操作
        qDebug() << "Failed to update user:" << query.lastError().text(); // 输出错误信息
        return false; // 如果更新失败，返回 false
    }
    return true; // 更新成功，返回 true
}
// 更新用户信息，根据用户 ID 修改用户名、密码、头像路径和签名。如果操作成功返回 true，否则返回 false。