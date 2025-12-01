#include "data.h"

#include <QFile>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QMessageBox>

// JSON FILE
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


Data::Data(QObject *parent)
    : QObject{parent}
{}

bool Data::getValidFileExist(const QString &filePath)
{
    return QFile::exists( filePath );
}

QString Data::getJasonFilePath(const QString &filename)
{
    QString dir = QCoreApplication::applicationDirPath();
    QDir d(dir);

    if (!d.exists("data")) {
        if (!d.mkpath("data")) {
            qWarning() << "Impossible de créer le dossier 'data'";
        }
    }

    return d.filePath( "data/" + filename);
}

S_GeneralSettings Data::loadGeneralSettings(const QString &filePath)
{
    S_GeneralSettings result;


    QFile file( ( filePath.isEmpty() ) ? getJasonFilePath( getJsonFile_SettingsName() ) : filePath );


    if (!file.open(QIODevice::ReadOnly)){
        return result; // GERER LES ERREUR ...
    }

    const QByteArray data = file.readAll();
    file.close();


    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()){
        return result;
    }

    QJsonObject root = doc.object();

    QJsonArray generalArray = root.value("settings_general").toArray();

    if (!generalArray.isEmpty() && generalArray.first().isObject()) {

        QJsonObject generalObj = generalArray.first().toObject();

        QString pluginDistPath = generalObj.value("plugin_dist_path").toString();

        QList<QString> platformListGeneral;
        QJsonArray platformArray = generalObj.value("platform_list").toArray();

        for (const QJsonValue &v : std::as_const(platformArray) ){
            if (v.isString()){
                platformListGeneral.append(v.toString());
            }
        }

        result.pluginDistPath = pluginDistPath;
        result.platformList = platformListGeneral;
    }

    return result;
}

QList<QString> Data::loadSettings(const QString &filePath)
{
    QList<QString> result;
    QFile file( ( filePath.isEmpty() ) ? getJasonFilePath( getJsonFile_SettingsName() ) : filePath );


    if (!file.open(QIODevice::ReadOnly))
        return result; // GERER LES ERREUR ...

    const QByteArray data = file.readAll();
    file.close();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isArray())
        return result;

    QJsonArray arr = doc.array();
    for (const QJsonValue &val : std::as_const(arr) ) {
        if (!val.isObject())
            continue;
        QJsonObject obj = val.toObject();

        QString v;
        v = obj.value("plugin_dist_path").toString();
        result.append(v);
    }

    return result;
}

QList<S_UnrealVersion> Data::loadUnrealVersions(const QString &filePath)
{

    QList<S_UnrealVersion> result;
    QFile file( ( filePath.isEmpty() ) ? getJasonFilePath( getJsonFile_UnrealVersionName() ) : filePath );


    if (!file.open(QIODevice::ReadOnly))
        return result; // GERER LES ERREUR ...

    const QByteArray data = file.readAll();
    file.close();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isArray())
        return result;

    QJsonArray arr = doc.array();
    for (const QJsonValue &val : std::as_const(arr) ) {
        if (!val.isObject())
            continue;
        QJsonObject obj = val.toObject();

        S_UnrealVersion v;
        v.path = obj.value("path").toString();
        v.name = obj.value("name").toString();
        result.append(v);
    }

    return result;
}

bool Data::saveGeneralSettings( const S_GeneralSettings &generalSettingsIn)
{
    const QString folderPath = getJasonFilePath( getJsonFile_SettingsName() );

    QJsonArray platformArray, generalSettingsArray;
    QJsonObject root, generalObj;

    for (const QString &p : generalSettingsIn.platformList) {
        platformArray.append(p);
    }

    generalObj["platform_list"] = platformArray;
    generalObj["plugin_dist_path"] = generalSettingsIn.pluginDistPath ;

    generalSettingsArray.append(generalObj);

    root["settings_general"] = generalSettingsArray;


    QJsonDocument doc(root);

    QFile file(folderPath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        // gérer l'erreur (message, qWarning, etc.)

        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}

bool Data::saveSettings( const QString &plugin_dist_path)
{
    const QString folderPath = getJasonFilePath( getJsonFile_SettingsName() );

    QJsonArray settings;

    QJsonArray settings_General;

    QJsonObject objectSettings, objectGeneral;


    objectGeneral["plugin_dist_path"] = plugin_dist_path ;


    settings_General.append( objectGeneral );
    objectSettings["settings_general"] = settings_General ;

    settings.append(objectSettings);


    QJsonDocument doc(settings);

    QFile file(folderPath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        // gérer l'erreur (message, qWarning, etc.)

        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}

void Data::saveUnrealVersions(QList<S_UnrealVersion> unrealVersions)
{
    const QString filePath = getJasonFilePath( getJsonFile_UnrealVersionName() ) ;

    QJsonArray arr;

    for (const S_UnrealVersion &v : std::as_const( unrealVersions )) {
        QJsonObject obj;

        obj["path"] = v.path;
        obj["name"] = v.name;
        arr.append(obj);
    }

    QJsonDocument doc(arr);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return; // gérer l’erreur comme tu veux

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}

QString Data::getJsonFile_SettingsName() const
{
    return jsonFile_SettingsName;
}

QString Data::getJsonFile_UnrealVersionName() const
{
    return jsonFile_UnrealVersionName;
}
