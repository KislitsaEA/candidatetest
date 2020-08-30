#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qfile.h>
#include <qfiledialog.h>
#include <qtextstream.h>
#include <qxmlstream.h>
#include <filemanip.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <qjsonarray.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Вывод начальной информации в текстбраузер
    ui->textBrowser->setText("1. Open a .csv file\n\n"
                             "2. Select Tag\n\n"
                             "3. Open a .json file\n\n"
                             "4. Export result\n\n");
}

MainWindow::~MainWindow()
{
    delete myJSON;
    delete myCSV;
    delete ui;
}



void MainWindow::on_csvOpen_clicked()
{
    myCSV->csvInit();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open .csv file"), "", tr(".csv (*.csv)"));
    myCSV->csvFileRead(fileName);

    for(int i=0;i<myCSV->csvGetSize();i++){
        ui->comboBox->addItem(myCSV->csvGetKey(i));
    }

    ui->jsonOpen->setEnabled(true);
}

void MainWindow::on_jsonOpen_clicked()
{
    myJSON->jsonInit();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open .json file"), "", tr(".json (*.json)"));
    QString comboBoxSelectedValue = ui->comboBox->currentText();
    myJSON->jsonFileRead(fileName, myCSV->csvFindValue(comboBoxSelectedValue), comboBoxSelectedValue);

    ui->textBrowser->clear();

    for(int i=0;i<myJSON->jsonGetSize();i++){
        ui->textBrowser->append(myJSON->jsonGetKey(i));

    }

    ui->exportButton->setEnabled(true);
}

void MainWindow::on_exportButton_clicked()
{
    myXML->exportXML(myJSON->csvJsonList);

    ui->textBrowser->clear();
    ui->textBrowser->setText("Done!\n"
                             "Result saved to output.xml");

    myJSON->jsonInit();
    ui->exportButton->setEnabled(false);
}
