#include "main_window.h"
#include "ui_main_window.h"
#include "ray_tracing.h"

#include "QMessageBox"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int set = 0;
char *const_char_stage = ".............................................................................................................................";

void MainWindow::on_pushButton_clicked()
{
    if (set == 0)
    {
        ui->statusBar->showMessage("Please select stage file from menu above.", 4000);
        return;
    }
    else
    {
        clearScreen();
        clearStage();
        QPixmap pix;
        ui->statusBar->showMessage(interpretStage(const_char_stage));
        Light light1(LIGHTS[0][0], LIGHTS[0][1], LIGHTS[0][2], LIGHTS[0][3], LIGHTS[0][4], LIGHTS[0][5]);
        ui->statusBar->showMessage("Scaning the stage...", 6000);
        rayTraceTriangles(light1);
        createPicture(const_char_stage);
        ui->statusBar->showMessage("Image saved.", 6000);
        // OMG, I was so lazy at university, I didn't make it open custom file and didn't write code for stage file check xD
        if (strcmp(const_char_stage, "c0.txt") == 0) pix = QPixmap("c0.txt.png");
        else if (strcmp(const_char_stage, "c1.txt") == 0) pix = QPixmap("c1.txt.png");
        else if (strcmp(const_char_stage, "c2.txt") == 0) pix = QPixmap("c2.txt.png");
        else ui->statusBar->showMessage("Wrong file?", 6000);
        ui->label_2->setPixmap(pix);
    }
}

void MainWindow::on_actionC0_changed()
{
    if (ui->actionC0->isChecked())
    {
        const_char_stage = "c0.txt";
        set = 1;
        ui->label->setText("c0.txt");
        ui->actionC1->setChecked(false);
        ui->actionAsdf->setChecked(false);
    }
}

void MainWindow::on_actionC1_changed()
{
    if (ui->actionC1->isChecked())
    {
        const_char_stage = "c1.txt";
        set = 1;
        ui->label->setText("c1.txt");
        ui->actionC0->setChecked(false);
        ui->actionAsdf->setChecked(false);
    }
}

void MainWindow::on_actionAsdf_changed()
{
    if (ui->actionAsdf->isChecked())
    {
        const_char_stage = "c2.txt";
        set = 1;
        ui->label->setText("c2.txt");
        ui->actionC0->setChecked(false);
        ui->actionC1->setChecked(false);
    }
}

void MainWindow::on_actionC0_triggered()
{

}

void MainWindow::on_actionC1_triggered()
{

}

