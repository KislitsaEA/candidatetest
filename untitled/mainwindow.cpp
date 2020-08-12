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
    CSVList.clear(); //Очистка массива для повторного чтения файла
    int i=0;
    ui->comboBox->clear();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open .csv file"), "", tr(".csv (*.csv)")); //Функция выбора файла .csv
    QFile file(fileName);
    file.open(QIODevice::ReadOnly); //Открытие файла только для чтения
    while (!file.atEnd()){ //Цикл на чтение файла, пока он не закончится
        QString inputLine = file.readLine(); //Чтение построчно
        auto inputParts = inputLine.split(";", Qt::SkipEmptyParts); //Разделение строки на слова, разделитель - ;
        if (inputParts[0]!="Tag"){ //Чтобы в список не попадала строка с заголовками
            CSVList.append(new myList); //Новая структура в массив
            CSVList[i]->key=inputParts[0]; //Данные в массив
            CSVList[i]->value=inputParts[1];
            ui->comboBox->addItem(inputParts[0]); //Данные в список, без типа, только теги.
            i++;
        }
    }
    file.close(); //Закрытие файла
}

void MainWindow::on_pushButton_2_clicked()
{
    jsonList.clear();
    ui->textBrowser->clear();
    int matches=0; //переменная для нахождения начала списка аргументов
    int offset=0;
    int k=0;
    QString findValue;
    QString findKey = ui->comboBox->currentText(); //Выбранный в списке тег
    for (int i=0;i<CSVList.size();i++){ //Поиск в массиве тега и соответствующего ему типа
        if (CSVList[i]->key==findKey)
            findValue = CSVList[i]->value;
    }
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open .json file"), "", tr(".json (*.json)")); //Выбор файла .json
    QFile file(fileName);
    file.open(QIODevice::ReadOnly); //Открытие в режиме чтения
    while(!file.atEnd()){
        QString findString = file.readLine(); //Построчное чтение .json файла и происк нужного типа
        findString.remove(QRegExp("\\s+"));
        if (findString.contains(findValue))
            matches++; //Если найден нужный тип, переменная будет равна 1
        else if (matches>0){
            if (findString=="}")
                break; //Если при чтении массива находится одиночный символ } - выход
            auto parts = findString.split(QRegExp("\\W+"), Qt::SkipEmptyParts); //Разбиение строки на слова с отбросом всех символов
            if (parts.size()>1){ //Если в строке менее 2 подстрок - пропуск строки
                jsonList.append((new myList));
                jsonList[k]->key=findKey+"."+parts[0];
                jsonList[k]->value=QVariant(offset).toString();
                k++;
                if (parts[1]=="double") //Преобразование оффсета в число
                    offset+=8;
                else
                    offset+=4;
                ui->textBrowser->append(findKey+"."+parts[0]);
            }
       }
   }
}

void MainWindow::on_pushButton_3_clicked()
{
    QFile file("output.xml"); //Создание файла в корне с программой
    file.open(QIODevice::WriteOnly);
    QTextStream writeStream(&file); //Открытие в режиме записи
    writeStream << "<root>\n";
    for(int i=0;i<jsonList.size();i++){//Запись всего массива в файл построчно
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

