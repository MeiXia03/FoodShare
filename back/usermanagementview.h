#ifndef USERMANAGEMENTVIEW_H
#define USERMANAGEMENTVIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class UserManagementView : public QWidget {
    Q_OBJECT

    public:
    explicit UserManagementView(int userId, QWidget *parent = nullptr);

private slots:
    void onSearchClicked(); // 搜索按钮点击事件
    void onDeleteButtonClicked(int row); // 删除按钮点击事件
    void onTableDoubleClicked(int row, int column); // 双击表格事件

private:
    int userId; // 当前登录用户的 ID
    QLineEdit *searchBox; // 搜索框
    QPushButton *searchButton; // 搜索按钮
    QTableWidget *resultTable; // 显示搜索结果的表格

    void setupUI(); // 初始化界面
    void loadResults(const QString &keyword); // 加载搜索结果
    void showAvatarDialog(const QString &avatarPath); // 显示头像弹窗
    void showSignatureDialog(const QString &signature); // 显示个性签名弹窗
    bool deleteUserFromDatabase(int userId); // 从数据库删除用户
};

#endif // USERMANAGEMENTVIEW_H