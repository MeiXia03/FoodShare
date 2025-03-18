#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QMap>

class DatabaseManager {
public:
    static DatabaseManager& instance(); // 单例模式

    bool connect(); // 连接到数据库
    void close();   // 关闭数据库连接

    // 用户表操作
    bool addUser(const QString& username, const QString& password, const QString& avatarPath = "default_avatar.png", const QString& signature = "");
    bool deleteUser(int userId);
    bool updateUser(int userId, const QString& username, const QString& password, const QString& avatarPath, const QString& signature);
    QVector<QVariantMap> getUsers();

    // 食谱表操作
    bool addRecipe(int userId, const QString& title, const QString& content, const QString& category, const QString& type, const QString& videoPath);
    bool deleteRecipe(int recipeId);
    bool updateRecipe(int recipeId, const QString& title, const QString& content, const QString& category, const QString& type, const QString& videoPath);
    QVector<QVariantMap> getRecipes();

    // 评论表操作
    bool addComment(int userId, int recipeId, const QString& content);
    bool deleteComment(int commentId);
    QVector<QVariantMap> getComments(int recipeId);

    // 反馈表操作
    bool addFeedback(int userId, const QString& content);
    bool updateFeedbackStatus(int feedbackId, const QString& status);
    QVector<QVariantMap> getFeedback();

    // 黑名单表操作
    bool addToBlacklist(int userId, int adminId, const QString& reason);
    bool removeFromBlacklist(int blockId);
    QVector<QVariantMap> getBlacklist();

private:
    DatabaseManager();  // 构造函数
    ~DatabaseManager(); // 析构函数

    QSqlDatabase m_database; // 数据库连接对象
};

#endif // DATABASEMANAGER_H