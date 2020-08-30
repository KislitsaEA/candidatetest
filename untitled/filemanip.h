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

class csvJsonContainer
{
public:
    struct myList{
        QString key;
        QString value;
    };
    QList<myList*> csvJsonList;
};

class csvProcessing : public csvJsonContainer
{
public:
    void csvInit(); //Инициализация класса
    void csvFileRead(QString); //Функция чтения csv
    void csvListAppend(QString, QString, int); //Функция добавления значений в массив CSV
    int csvGetSize(); //Функция вывода размера контейнера
    QString csvGetKey(int); //Функция получения ключа из контейнера
    QString csvFindValue(QString); //Функция поиска значения в контейнере
};

class jsonProcessing : public csvJsonContainer
{
public:
    void jsonInit(); //Инициализация
    void jsonFileRead(QString, QString, QString); //Чтение
    void jsonListAppend(QString, QString, int); //Добавление значений
    int jsonGetSize(); //Размер контейнера
    QString jsonGetKey(int); //Получение ключа
private:
    QMap<QString, int> offsetConvertToInt = { {"double", 8}, {"int", 4}, {"bool", 1} }; //Преобразование оффсета в число
    int offset=0; //Переменная для накопления оффсета
};

class getXML : public csvJsonContainer
{
public:
    void exportXML(QList<myList*> csvJsonList);
};

#endif // CSVREAD_H
