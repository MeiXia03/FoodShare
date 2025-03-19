#include "DatabaseManager.h"
#include <QSqlError>
#include <QCoreApplication>

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::DatabaseManager() {}

DatabaseManager::~DatabaseManager() {
    close();
}

bool DatabaseManager::connect() {
    if (m_database.isOpen()) {
        return true;
    }

    QString dbPath = QCoreApplication::applicationDirPath() + "/foodie.db";
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(dbPath);

    if (!m_database.open()) {
        qDebug() << "Failed to connect to database:" << m_database.lastError().text();
        return false;
    }

    qDebug() << "Connected to database:" << dbPath;
    return true;
}

void DatabaseManager::close() {
    if (m_database.isOpen()) {
        m_database.close();
    }
}

// 用户表操作
bool DatabaseManager::addUser(const QString& username, const QString& password, const QString& avatarPath, const QString& signature) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password, avatar_path, signature) VALUES (:username, :password, :avatar_path, :signature)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.bindValue(":avatar_path", avatarPath);
    query.bindValue(":signature", signature);
    if (!query.exec()) {
        qDebug() << "Failed to add user:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::deleteUser(int userId) {
    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);
    if (!query.exec()) {
        qDebug() << "Failed to delete user:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::updateUser(int userId, const QString& username, const QString& password, const QString& avatarPath, const QString& signature) {
    QSqlQuery query;
    query.prepare("UPDATE users SET username = :username, password = :password, avatar_path = :avatar_path, signature = :signature WHERE user_id = :user_id");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.bindValue(":avatar_path", avatarPath);
    query.bindValue(":signature", signature);
    query.bindValue(":user_id", userId);
    if (!query.exec()) {
        qDebug() << "Failed to update user:" << query.lastError().text();
        return false;
    }
    return true;
}

QVector<QVariantMap> DatabaseManager::getUsers() {
    QVector<QVariantMap> users;
    QSqlQuery query("SELECT * FROM users");
    while (query.next()) {
        QVariantMap user;
        user["user_id"] = query.value("user_id");
        user["username"] = query.value("username");
        user["password"] = query.value("password");
        user["avatar_path"] = query.value("avatar_path");
        user["signature"] = query.value("signature");
        user["is_admin"] = query.value("is_admin");
        user["created_at"] = query.value("created_at");
        users.append(user);
    }
    return users;
}

// 食谱表操作
bool DatabaseManager::addRecipe(int userId, const QString& title, const QString& content, const QString& category, const QString& type, const QString& videoPath) {
    QSqlQuery query;
    query.prepare("INSERT INTO recipes (user_id, title, content, category, type, video_path) "
                  "VALUES (:user_id, :title, :content, :category, :type, :video_path)");
    query.bindValue(":user_id", userId);
    query.bindValue(":title", title);
    query.bindValue(":content", content);
    query.bindValue(":category", category);
    query.bindValue(":type", type);
    query.bindValue(":video_path", videoPath);

    // 调试输出绑定的参数
    qDebug() << "Executing query:" << query.executedQuery();
    qDebug() << "Bound values:" << query.boundValues();

    if (!query.exec()) {
        qDebug() << "Failed to add recipe:" << query.lastError().text();
        return false;
    }
    qDebug() << "Recipe added successfully!";
    return true;
}

bool DatabaseManager::deleteRecipe(int recipeId) {
    QSqlQuery query;
    query.prepare("DELETE FROM recipes WHERE recipe_id = :recipe_id");
    query.bindValue(":recipe_id", recipeId);
    if (!query.exec()) {
        qDebug() << "Failed to delete recipe:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::updateRecipe(int recipeId, const QString& title, const QString& content, const QString& category, const QString& type, const QString& videoPath) {
    QSqlQuery query;
    query.prepare("UPDATE recipes SET title = :title, content = :content, category = :category, type = :type, video_path = :video_path WHERE recipe_id = :recipe_id");
    query.bindValue(":title", title);
    query.bindValue(":content", content);
    query.bindValue(":category", category);
    query.bindValue(":type", type);
    query.bindValue(":video_path", videoPath);
    query.bindValue(":recipe_id", recipeId);
    if (!query.exec()) {
        qDebug() << "Failed to update recipe:" << query.lastError().text();
        return false;
    }
    return true;
}

QVector<QVariantMap> DatabaseManager::getRecipes() {
    QVector<QVariantMap> recipes;
    QSqlQuery query("SELECT * FROM recipes");
    while (query.next()) {
        QVariantMap recipe;
        recipe["recipe_id"] = query.value("recipe_id");
        recipe["user_id"] = query.value("user_id");
        recipe["title"] = query.value("title");
        recipe["content"] = query.value("content");
        recipe["category"] = query.value("category");
        recipe["type"] = query.value("type");
        recipe["video_path"] = query.value("video_path");
        recipe["likes"] = query.value("likes");
        recipe["created_at"] = query.value("created_at");
        recipes.append(recipe);
    }
    return recipes;
}

bool DatabaseManager::addComment(int userId, int recipeId, const QString& content) {
    QSqlQuery query;
    query.prepare("INSERT INTO comments (user_id, recipe_id, content) VALUES (:user_id, :recipe_id, :content)");
    query.bindValue(":user_id", userId);
    query.bindValue(":recipe_id", recipeId);
    query.bindValue(":content", content);

    if (!query.exec()) {
        qDebug() << "Failed to add comment:" << query.lastError().text();
        return false;
    }
    return true;
}

QVector<QVariantMap> DatabaseManager::getComments(int recipeId) {
    QVector<QVariantMap> comments;
    QSqlQuery query;
    query.prepare("SELECT content, created_at FROM comments WHERE recipe_id = :recipe_id ORDER BY created_at DESC");
    query.bindValue(":recipe_id", recipeId);

    if (!query.exec()) {
        qDebug() << "Failed to get comments:" << query.lastError().text();
        return comments;
    }

    while (query.next()) {
        QVariantMap comment;
        comment["content"] = query.value("content");
        comment["created_at"] = query.value("created_at");
        comments.append(comment);
    }
    return comments;
}

// 其他表的操作方法类似，省略重复代码