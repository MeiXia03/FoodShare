#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase> // 提供对数据库连接的支持
#include <QSqlQuery>    // 提供对 SQL 查询的支持
#include <QVariant>     // 用于存储和操作通用数据类型
#include <QString>      // 提供对字符串的支持
#include <QVector>      // 提供动态数组的支持

class DatabaseManager {
public:
    static DatabaseManager& instance(); 
    // 获取 DatabaseManager 的单例实例，用于确保整个应用程序中只有一个数据库连接实例

    bool connect(const QString& dbPath); 
    // 连接到 SQLite 数据库
    // 参数 dbPath：数据库文件的路径
    // 返回值：如果连接成功返回 true，否则返回 false

    void close(); 
    // 关闭数据库连接

    // CRUD operations
    bool addUser(const QString& username, const QString& password, const QString& avatarPath = "default_avatar.png", const QString& signature = ""); 
    // 添加用户
    // 参数 username：用户名
    // 参数 password：密码
    // 参数 avatarPath：用户头像路径，默认为 "default_avatar.png"
    // 参数 signature：用户签名，默认为空字符串
    // 返回值：如果添加成功返回 true，否则返回 false

    bool deleteUser(int userId); 
    // 删除用户
    // 参数 userId：用户 ID
    // 返回值：如果删除成功返回 true，否则返回 false

    QVector<QVariantMap> getUsers(); 
    // 获取所有用户信息
    // 返回值：包含用户信息的 QVector，每个用户信息存储在 QVariantMap 中

    bool updateUser(int userId, const QString& username, const QString& password, const QString& avatarPath, const QString& signature); 
    // 更新用户信息
    // 参数 userId：用户 ID
    // 参数 username：新的用户名
    // 参数 password：新的密码
    // 参数 avatarPath：新的头像路径
    // 参数 signature：新的签名
    // 返回值：如果更新成功返回 true，否则返回 false

private:
    DatabaseManager(); 
    // 构造函数，私有化以实现单例模式

    ~DatabaseManager(); 
    // 析构函数，关闭数据库连接

    QSqlDatabase m_database; 
    // 数据库连接对象
};

#endif // DATABASEMANAGER_H
