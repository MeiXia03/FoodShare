#include "loginview.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    LoginView loginView;
    loginView.show();
    return a.exec();
}