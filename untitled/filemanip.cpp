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
        csvContainer.append(qMakePair(inputParts[0], inputParts[1]));
    }
}

//Возврат ключей из контейнера
QList<QString> csvProcessing::csvGetKey(){
    QList <QString> keys;
    for (auto a : csvContainer){
        keys.append(a.first);
    }
    return keys;
}

//Возврат размера контейнера
int csvProcessing::csvGetSize(){
    return csvContainer.size();
}

//Поиск значения по ключу
QString csvProcessing::csvFindValue(QString inputKey){
    for (auto a : csvContainer){
        if (a.first == inputKey){
            return a.second;
        }
    }
    return 0;
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
    auto jsonDocument = QJsonDocument().fromJson(file.readAll()).object().value("TypeInfos").toArray();

    //Цикл поиска нужного значения и заполнение контейнера
    for (auto typeInfo: jsonDocument)
    {
        QJsonObject infoObject = typeInfo.toObject();
        if (infoObject.value("TypeName").toString() == inputValue)
        {
            infoObject = infoObject.value("Propertys").toObject();
            for(auto e : infoObject.keys())
            {
                jsonContainer.append(qMakePair(inputKey+"."+e, QString::number(offsetCounter)));
                offsetCounter+=offsetConvertToInt.value(infoObject.value(e).toString());
            }
        }
    }
}

//Возврат размера контейнера
int jsonProcessing::jsonGetSize(){ return jsonContainer.size();
}


//Получение ключей
QList<QString> jsonProcessing::jsonGetKey(){
    QList <QString> keys;
    for (auto a : jsonContainer){
        keys.append(a.first);
    }
    return keys;
}


//Очистка
void jsonProcessing::jsonInit(){
    jsonContainer.clear();
}

//***XML***

void getXML::exportXML(QList<QPair<QString, QString>> myDataList){
    QFile file("output.xml");
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter XmlWriter(&file);

    XmlWriter.writeStartElement("root");

    for(auto e : myDataList){
            XmlWriter.writeStartElement("item");
            XmlWriter.writeAttribute("Binding","Introduced");
            XmlWriter.writeStartElement("node-path");
            //XmlWriter.writeCharacters(e);
            XmlWriter.writeEndElement();
            XmlWriter.writeStartElement("address");
            //XmlWriter.writeCharacters(myDataList.value(e));
            XmlWriter.writeEndElement();
            XmlWriter.writeEndElement();
        }
    XmlWriter.writeEndElement();
}
