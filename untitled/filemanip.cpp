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
    int i=0;
    QFile file(inputFileName);
    file.open(QIODevice::ReadOnly);
    QJsonDocument doc = QJsonDocument().fromJson(file.readAll());
    QJsonObject jsonObject = doc.object();
    QJsonArray typeInfos = jsonObject.value("TypeInfos").toArray();

    for (auto typeInfo: typeInfos)
    {
        const QJsonObject infoObject = typeInfo.toObject();
        if (infoObject.value("TypeName").toString() == inputValue)
        {
            QMap<QString, QVariant> infoObjec1t = infoObject.value("Propertys").toVariant().toMap();
            for(auto e : infoObjec1t.keys())
            {
                jsonProcessing::jsonListAppend(inputKey+"."+e,infoObjec1t.value(e).toString(), i);
                i++;
            }
        }
    }
}

void jsonProcessing::jsonListAppend(QString key, QString value, int i){
    csvJsonList.append(new myList);
    csvJsonList[i]->key=key;
    csvJsonList[i]->value=QString::number(offset);
    offset += offsetConvertToInt.value(value);
    i++;
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
