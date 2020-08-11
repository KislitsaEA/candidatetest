#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qfile.h>
#include <qfiledialog.h>
#include <qtextstream.h>
#include <qmessagebox.h>

struct myList{
    QString key;
    QString value;
};
QList<myList*> CSVList;
QList<myList*> jsonList;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textBrowser->setText("1. Open a .csv file\n\n"
                             "2. Select Tag\n\n"
                             "3. Open a .json file\n\n"
                             "4. Export result\n\n");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    CSVList.clear();
    int i=0;
    ui->comboBox->clear();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open .csv file"), "", tr(".csv (*.csv)"));
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    while (!file.atEnd()){
        QString inputLine = file.readLine();
        auto inputParts = inputLine.split(";", Qt::SkipEmptyParts);
        if (inputParts[0]!="Tag"){
            CSVList.append(new myList);
            CSVList[i]->key=inputParts[0];
            CSVList[i]->value=inputParts[1];
            ui->comboBox->addItem(inputParts[0]);
            i++;
        }
    }
    file.close();
}

void MainWindow::on_pushButton_2_clicked()
{
    jsonList.clear();
    ui->textBrowser->clear();
    int matches=0;
    int offset=0;
    int k=0;
    QString findValue;
    QString findKey = ui->comboBox->currentText();
    for (int i=0;i<CSVList.size();i++){
        if (CSVList[i]->key==findKey)
            findValue = CSVList[i]->value;
    }
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open .json file"), "", tr(".json (*.json)"));
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    while(!file.atEnd()){
        QString findString = file.readLine();
        if (findString.contains(findValue)){
            matches++;
        }
        else if (findString.contains("}") and matches==1){
            break;
        }
        else if (((findString.contains("double")) or (findString.contains("int"))) and matches==1){
            auto parts = findString.split(QRegExp("\\W+"), Qt::SkipEmptyParts);
            jsonList.append((new myList));
            jsonList[k]->key=findKey+"."+parts[0];
            jsonList[k]->value=QVariant(offset).toString();
            k++;
            if (parts[1]=="double")
                offset+=8;
            else
                offset+=4;
            ui->textBrowser->append(findKey+"."+parts[0]);
       }
   }
}

void MainWindow::on_pushButton_3_clicked()
{
    QFile file("output.xml");
    file.open(QIODevice::WriteOnly);
    QTextStream writeStream(&file);
    writeStream << "<root>\n";
    for(int i=0;i<jsonList.size();i++){
        writeStream << "<item Binding='Introduced'>\n";
        writeStream << "    <node-path>"+jsonList[i]->key+"</node-path>\n";
        writeStream << "    <address>"+jsonList[i]->value+"</address>\n";
        writeStream << "</item>\n";
    }
    writeStream << "</root>\n";
    file.close();
    ui->textBrowser->clear();
    ui->textBrowser->setText("Done!\n"
                             "Result saved to output.xml");
}

