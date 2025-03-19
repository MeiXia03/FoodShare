#ifndef BACKENDVIEW_H
#define BACKENDVIEW_H

#include <QWidget>

class BackendView : public QWidget {
    Q_OBJECT

public:
    explicit BackendView(QWidget *parent = nullptr);
};

#endif // BACKENDVIEW_H