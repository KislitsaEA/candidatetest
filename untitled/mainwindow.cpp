#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qfile.h>
#include <qfiledialog.h>
#include <qtextstream.h>

//Структура для массивов
struct myList{
    QString key;
    QString value;
};
//Массив для хранения списка из файла .csv
QList<myList*> csvList;
//Массив для хранения списка из файла .json
QList<myList*> jsonList;
//Словарь для преобразования оффсета в число
QMap<QString, int> offsetConvert = { {"double", 8}, {"int", 4}, {"bool", 1} };

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
    delete ui;
}

void MainWindow::on_csvOpen_clicked()
{
    csvList.clear();
    int i=0;
    ui->comboBox->clear();

    //Функция выбора файла .csv
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open .csv file"), "", tr(".csv (*.csv)"));
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);

    //Цикл на чтение файла построчно, пока он не закончится
    while (!file.atEnd()){
        QString inputLine = file.readLine();

        //Разделение строки на слова, разделитель - ;
        auto inputParts = inputLine.split(";", Qt::SkipEmptyParts);

        //Пропуск строки с заголовками
        if (inputParts[0]=="Tag"){
            continue;
        }

        //Заполнение массива
        csvList.append(new myList);
        csvList[i]->key=inputParts[0];
        csvList[i]->value=inputParts[1];
        ui->comboBox->addItem(inputParts[0]);
        i++;

    }
    file.close();
    ui->jsonOpen->setEnabled(true);
}

void MainWindow::on_jsonOpen_clicked()
{
    jsonList.clear();
    ui->textBrowser->clear();

    bool matches=0; //Переменная для нахождения начала списка аргументов
    int offset=0; //Переменная для накопления оффсета
    int k=0; //Указатель на элемент массива
    QString findValue; //Переменная для хранения типа

    //Поиск в массиве тега и соответствующего ему типа
    QString findKey = ui->comboBox->currentText();
    for (int i=0;i<csvList.size();i++){
        if (csvList[i]->key==findKey)
            findValue = csvList[i]->value;
    }

    //Выбор файла .json и открытие в режиме чтения
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open .json file"), "", tr(".json (*.json)"));
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);

    //Цикл чтения файла
    while(!file.atEnd()){

        //Построчное чтение .json файла и удаление из строки всех пробельных символов
        QString findString = file.readLine();
        findString.remove(QRegExp("\\s+"));

        //Если найден нужный тип, переменная будет равна 1
        if (findString.contains(findValue)){
            matches=1;
        }

        //Если нужный тип найден, начинается обработка строк
        else if (matches){

            //Если при чтении массива находится одиночный символ } - выход
            if (findString=="}"){
                break;
            }

            //Разбиение строки на слова с отбросом всех символов не слова
            auto parts = findString.split(QRegExp("\\W+"), Qt::SkipEmptyParts);

            //Если в строке менее 2 подстрок - пропуск строки
            if (parts.size()<2){
                continue;
            }

            //Заполнение массива и текстбраузера значениями
            jsonList.append((new myList));
            jsonList[k]->key=findKey+"."+parts[0];
            jsonList[k]->value=QVariant(offset).toString();
            k++;
            offset += offsetConvert.value(parts[1]);
            ui->textBrowser->append(findKey+"."+parts[0]);
        }
    }
    ui->exportButton->setEnabled(true);
}

void MainWindow::on_exportButton_clicked()
{
    //Создание файла в корне с программой. Открытие в режиме записи
    QFile file("output.xml");
    file.open(QIODevice::WriteOnly);
    QTextStream writeStream(&file);

    //Запись всего массива в файл построчно
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
