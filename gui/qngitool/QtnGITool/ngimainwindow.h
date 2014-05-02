#ifndef NGIMAINWINDOW_H
#define NGIMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class nGIMainWindow;
}

class nGIMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit nGIMainWindow(QWidget *parent = 0);
    ~nGIMainWindow();
    
private:
    Ui::nGIMainWindow *ui;
};

#endif // NGIMAINWINDOW_H
