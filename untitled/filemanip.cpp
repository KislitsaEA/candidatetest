#include "filemanip.h"

void csvProcessing::csvFileRead(QString inputFileName){
    int i=0; //Указатель на элемент массива
    QFile file(inputFileName);
    file.open(QIODevice::ReadOnly);

    while (!file.atEnd()){
        QString inputLine = file.readLine();

        //Разделение строки на слова, разделитель - ;
        auto inputParts = inputLine.split(";", Qt::SkipEmptyParts);

        //Пропуск строки с заголовками
        if (inputParts[0]=="Tag"){
            continue;
        }

        //Заполнение массива
        csvProcessing::csvListAppend(inputParts[0], inputParts[1], i);
        i++;
    }
}

QString csvProcessing::csvGetKey(int i){
    return (csvJsonList[i]->key);
}

int csvProcessing::csvGetSize(){
    return csvJsonList.size();
}

void csvProcessing::csvListAppend(QString key, QString value, int i){
    csvJsonList.append(new myList);
    csvJsonList[i]->key=key;
    csvJsonList[i]->value=value;
    i++;
}

QString csvProcessing::csvFindValue(QString inputValue){
    for (int i=0; i<csvJsonList.size(); i++){
        if (csvJsonList[i]->key == inputValue){
            return csvJsonList[i]->value;
        }
    }
    return 0;
}

void csvProcessing::csvInit(){
    csvJsonList.clear();
}

//***JSON***

void jsonProcessing::jsonFileRead(QString inputFileName, QString inputValue, QString inputKey){
    QFile file(inputFileName);
    int i=0; //Указатель на элемент массива
    bool matches=0; //Флаг для парсинга json
    file.open(QIODevice::ReadOnly);

    while(!file.atEnd()){

        //Построчное чтение .json файла и удаление из строки всех пробельных символов
        QString findString = file.readLine();
        findString.remove(QRegExp("\\s+"));

        //Если найден нужный тип, переменная будет равна 1
        if (findString.contains(inputValue)){
            matches=1;
        }

        //Если нужный тип найден, начинается обработка строк
        else if (matches){

            //Если при чтении строк находится одиночный символ } - выход
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
            jsonProcessing::jsonListAppend(inputKey+"."+parts[0],parts[1], i);
            i++;
        }
    }
}

void jsonProcessing::jsonListAppend(QString key, QString value, int i){
    csvJsonList.append(new myList);
    csvJsonList[i]->key=key;
    csvJsonList[i]->value=QString::number(offset);
    i++;
    offset += offsetConvertToInt.value(value);
}

int jsonProcessing::jsonGetSize(){
    return csvJsonList.size();
}

QString jsonProcessing::jsonGetKey(int i){
    return csvJsonList[i]->key;
}

void jsonProcessing::jsonInit(){
    csvJsonList.clear();
    offset=0;
}

//***XML***

void getXML::exportXML(QList<myList*> myDataList){
    QFile file("output.xml");
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter XmlWriter(&file);

    XmlWriter.writeStartElement("root");

    for(int i=0;i<myDataList.size();i++){

            XmlWriter.writeStartElement("item");
            XmlWriter.writeAttribute("Binding","Introduced");
            XmlWriter.writeStartElement("node-path");
            XmlWriter.writeCharacters(myDataList[i]->key);
            XmlWriter.writeEndElement();
            XmlWriter.writeStartElement("address");
            XmlWriter.writeCharacters(myDataList[i]->value);
            XmlWriter.writeEndElement();
            XmlWriter.writeEndElement();
        }
    XmlWriter.writeEndElement();
}
