#ifndef RECIPECOMMENTSVIEW_H
#define RECIPECOMMENTSVIEW_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

class RecipeCommentsView : public QDialog {
    Q_OBJECT

public:
    explicit RecipeCommentsView(int recipeId, QWidget *parent = nullptr);

private slots:
    void onDeleteButtonClicked(int row); // 删除按钮点击事件
    void onCommentDoubleClicked(int row, int column); // 双击评论内容事件

private:
    int recipeId; // 当前菜谱的 ID
    QTableWidget *commentsTable; // 显示评论的表格

    void setupUI(); // 初始化界面
    void loadComments(); // 加载评论数据
    bool deleteCommentFromDatabase(int commentId); // 从数据库删除评论
};

#endif // RECIPECOMMENTSVIEW_H