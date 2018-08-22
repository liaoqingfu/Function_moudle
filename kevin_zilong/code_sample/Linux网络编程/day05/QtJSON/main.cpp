
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>

#if 0
{
    "key1": 1,
    "key2": true,
    "key3": false,
    "key4": none,
    "key5": { "key51": "hello", "key52":"world" },
    "key6": [
            {"key611": true},
            {"key621": false},
            {"key631":true},
            40
        ]
}

value: none, bool, number, string, object, array
#endif

QByteArray get_json()
{
    QJsonObject root;
    root["key1"] = 1;
    root["key2"] = true;
    root["key3"] = false;
    root["key4"] = QJsonValue();
    QJsonObject key5;
    key5["key51"] = "hello";
    key5["key52"] = "world";
    root["key5"] = key5;
    QJsonArray key6;
    QJsonObject key61;
    key61["key611"] = true;
    QJsonObject key62;
    key62["key621"] = false;
    QJsonObject key63;
    key63["key631"] = true;
    key6.append(key61);
    key6.append(key62);
    key6.append(key63);
    key6.append(40);
    root["key6"] = key6;

    QJsonDocument doc(root);
    return doc.toJson();
}

int main()
{
    QByteArray buf = get_json();

    QJsonDocument doc = QJsonDocument::fromJson(buf); // cJSON_Parse(...)
    QJsonObject root = doc.object();// root object

//    qDebug() << root["key6"].toArray().at(0).toObject()["key611"].toBool();
    QJsonValue key6_v = root["key6"];
    QJsonArray key6 = key6_v.toArray();
    QJsonValue key61_v = key6.at(0);
    QJsonObject key61 = key61_v.toObject();
    QJsonValue key611_v = key61["key611"];
    bool result = key611_v.toBool();
    qDebug() << result;
}












