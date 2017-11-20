#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "math.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_actionC0_triggered();

    void on_actionC1_triggered();

    void on_actionC0_changed();

    void on_actionC1_changed();

    void on_actionAsdf_changed();

private:
    Ui::MainWindow *ui;
};

#endif // MAIN_WINDOW_H
