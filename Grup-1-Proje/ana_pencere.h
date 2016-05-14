#ifndef ANA_PENCERE_H
#define ANA_PENCERE_H

#include <QMainWindow>
#include "math.h"

namespace Ui {
class Ana_Pencere;
}

class Ana_Pencere : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Ana_Pencere(QWidget *parent = 0);
    ~Ana_Pencere();
    
private slots:
    void on_pushButton_clicked();

    void on_actionC0_triggered();

    void on_actionC1_triggered();

    void on_actionC0_changed();

    void on_actionC1_changed();

private:
    Ui::Ana_Pencere *ui;
};

#endif // ANA_PENCERE_H
