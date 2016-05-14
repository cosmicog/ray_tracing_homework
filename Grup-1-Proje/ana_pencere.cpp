#include "ana_pencere.h"
#include "ui_ana_pencere.h"
#include "isin_izleme.h"

#include "QMessageBox"

Ana_Pencere::Ana_Pencere(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Ana_Pencere)
{
    ui->setupUi(this);
}

Ana_Pencere::~Ana_Pencere()
{
    delete ui;
}

int set = 0;
const char* const_char_sahne= ".............................................................................................................................";

void Ana_Pencere::on_pushButton_clicked()
{
    if (set == 0)
    {
        ui->statusBar->showMessage("Lütfen yukarıdaki menüden sahne seçin.", 4000);
        return;
    }
    else
    {
        ekranTemizle();
        sahneTemizle();
        QPixmap pix;
        ui->statusBar->showMessage(sahneYorumla(const_char_sahne));
        Isik isik1(ISIKLAR[0][0], ISIKLAR[0][1], ISIKLAR[0][2], ISIKLAR[0][3], ISIKLAR[0][4], ISIKLAR[0][5]);
        ui->statusBar->showMessage("Sahne Taranıyor...", 6000);
        isinIzleUcgenleri(isik1);
        resimOlustur(const_char_sahne);
        ui->statusBar->showMessage("Resim Kaydedildi.", 6000);
        if (const_char_sahne == "c0.txt") pix = QPixmap("c0.txt.png");
        if (const_char_sahne == "c1.txt") pix = QPixmap("c1.txt.png");
        ui->label_2->setPixmap(pix);
        set = 0;
    }
}

void Ana_Pencere::on_actionC0_changed()
{
    if(ui->actionC0->isChecked())
    {
        const_char_sahne = "c0.txt";
        set = 1;
        ui->label->setText("c0.txt");
        ui->actionC1->setChecked(false);
    }

    if(ui->actionC1->isChecked())
    {
        const_char_sahne = "c1.txt";
        set = 1;
        ui->label->setText("c1.txt");
        ui->actionC0->setChecked(false);
    }
}

void Ana_Pencere::on_actionC1_changed()
{
    if(ui->actionC0->isChecked())
    {
        const_char_sahne = "c0.txt";
        set = 1;
        ui->label->setText("c0.txt");
        ui->actionC1->setChecked(false);
    }

    if(ui->actionC1->isChecked())
    {
        const_char_sahne = "c1.txt";
        set = 1;
        ui->label->setText("c1.txt");
        ui->actionC0->setChecked(false);
    }
}
