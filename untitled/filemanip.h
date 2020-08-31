#ifndef CSVREAD_H
#define CSVREAD_H
#include <qstring.h>
#include <qlist.h>
#include <qfile.h>
#include <qmap.h>
#include <qxmlstream.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <qjsonarray.h>

class csvProcessing
{
public:
    void csvInit(); //Инициализация класса
    void csvFileRead(QString); //Функция чтения csv
    void csvListAppend(QString, QString, int); //Функция добавления значений в массив CSV
    int csvGetSize(); //Функция вывода размера контейнера
    QList<QString> csvGetKey(); //Функция получения ключа из контейнера
    QString csvFindValue(QString); //Функция поиска значения в контейнере
    QMap <QString, QString> csvContainer;
};

class jsonProcessing
{
public:
    void jsonInit(); //Инициализация
    void jsonFileRead(QString, QString, QString); //Чтение
    void jsonListAppend(QString, QString, int); //Добавление значений
    int jsonGetSize(); //Размер контейнера
    QList <QString> jsonGetKey(); //Получение ключа
    QMap <QString, QString> jsonContainer;
private:
    QMap<QString, int> offsetConvertToInt = { {"double", 8}, {"int", 4}, {"bool", 1} }; //Преобразование оффсета в число
    int offset=0; //Переменная для накопления оффсета
};

class getXML
{
public:
    void exportXML(QMap<QString, QString> csvJsonList);
};

#endif // CSVREAD_H
