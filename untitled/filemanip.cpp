#include "filemanip.h"

void csvProcessing::csvFileRead(QString inputFileName){
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
        csvContainer.insert(inputParts[0], inputParts[1]);
    }
}

//Возврат ключей из контейнера
QList<QString> csvProcessing::csvGetKey(){
    return csvContainer.keys();
}

//Возврат размера контейнера
int csvProcessing::csvGetSize(){
    return csvContainer.size();
}

//Поиск значения по ключу
QString csvProcessing::csvFindValue(QString inputKey){
    return csvContainer.value(inputKey);
}

//Очистка контейнера
void csvProcessing::csvInit(){
    csvContainer.clear();
}

//***JSON***

void jsonProcessing::jsonFileRead(QString inputFileName, QString inputValue, QString inputKey){
    int offsetCounter = 0; //Счетчик оффсета
    QFile file(inputFileName);
    file.open(QIODevice::ReadOnly);

    //Чтение файла и преобразование его в контейнер
    QJsonDocument doc = QJsonDocument().fromJson(file.readAll());
    QJsonObject jsonObject = doc.object();
    QJsonArray typeInfos = jsonObject.value("TypeInfos").toArray();

    //Цикл поиска нужного значения и заполнение контейнера
    for (auto typeInfo: typeInfos)
    {
        QJsonObject infoObject = typeInfo.toObject();
        if (infoObject.value("TypeName").toString() == inputValue)
        {
            QMap<QString, QVariant> infoObjec1t = infoObject.value("Propertys").toVariant().toMap();
            for(auto e : infoObjec1t.keys())
            {
                jsonContainer.insert(inputKey+"."+e, QString::number(offsetCounter));
                offsetCounter+=offsetConvertToInt.value(infoObjec1t.value(e).toString());
            }
        }
    }
}

//Возврат размера контейнера
int jsonProcessing::jsonGetSize(){
    return jsonContainer.size();
}


//Получение ключей
QList<QString> jsonProcessing::jsonGetKey(){
    return jsonContainer.keys();
}


//Очистка
void jsonProcessing::jsonInit(){
    jsonContainer.clear();
}

//***XML***

void getXML::exportXML(QMap<QString, QString> myDataList){
    QFile file("output.xml");
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter XmlWriter(&file);

    XmlWriter.writeStartElement("root");

    for(auto e : myDataList.keys()){
            XmlWriter.writeStartElement("item");
            XmlWriter.writeAttribute("Binding","Introduced");
            XmlWriter.writeStartElement("node-path");
            XmlWriter.writeCharacters(e);
            XmlWriter.writeEndElement();
            XmlWriter.writeStartElement("address");
            XmlWriter.writeCharacters(myDataList.value(e));
            XmlWriter.writeEndElement();
            XmlWriter.writeEndElement();
        }
    XmlWriter.writeEndElement();
}
