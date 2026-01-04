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

// EXTERNAL LINK
#include <QDesktopServices>
#include <QUrl>


Data::Data(QObject *parent)
    : QObject{parent}
{

    notificationNames.append("Build");
    notificationNames.append("Software update");
    notificationNames.append("Popup");
}

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
        QString pluginPath = generalObj.value("pluginPath").toString();
        QString pluginName = generalObj.value("pluginName").toString();
        QString pluginVersion = generalObj.value("pluginVersion").toString();


        QList<QString> platformListGeneral, notificationsList, distributionList;
        QJsonArray platformArray = generalObj.value("platform_list").toArray();
        QJsonArray notificationArray = generalObj.value("notifications").toArray();
        QJsonArray distributionArray = generalObj.value("distribution").toArray();
        QJsonArray unrealEngineArray = generalObj.value("unrealEngine").toArray();


        for (const QJsonValue &v : std::as_const(platformArray) ){
            if (v.isString()){
                platformListGeneral.append(v.toString());
            }
        }

        for (const QJsonValue &v : std::as_const(notificationArray) ){
            if (v.isString()){
                notificationsList.append(v.toString());
            }
        }

        for (const QJsonValue &v : std::as_const(distributionArray) ){
            if (v.isString()){
                distributionList.append(v.toString());
            }
        }

        for (const QJsonValue &val : std::as_const(unrealEngineArray) ) {
            if (!val.isObject())
                continue;
            QJsonObject obj = val.toObject();

            S_UnrealVersion v;
            v.unrealPath = obj.value("unrealPath").toString();
            v.unrealName = obj.value("unrealName").toString();
            result.S_UnrealVersionsList.append(v);
        }

        result.pluginDistPath = pluginDistPath;
        result.S_LastPlugin.pluginPath = pluginPath;
        result.S_LastPlugin.pluginName = pluginName;
        result.S_LastPlugin.pluginVersion = pluginVersion;

        result.platformList = platformListGeneral;
        result.notifications = notificationsList;
        result.distribution = distributionList;
    }

    return result;
}

// QList<QString> Data::loadSettings(const QString &filePath)
// {
//     QList<QString> result;
//     QFile file( ( filePath.isEmpty() ) ? getJasonFilePath( getJsonFile_SettingsName() ) : filePath );


//     if (!file.open(QIODevice::ReadOnly))
//         return result; // GERER LES ERREUR ...

//     const QByteArray data = file.readAll();
//     file.close();

//     QJsonParseError err;
//     QJsonDocument doc = QJsonDocument::fromJson(data, &err);
//     if (err.error != QJsonParseError::NoError || !doc.isArray())
//         return result;

//     QJsonArray arr = doc.array();
//     for (const QJsonValue &val : std::as_const(arr) ) {
//         if (!val.isObject())
//             continue;
//         QJsonObject obj = val.toObject();

//         QString v;
//         v = obj.value("plugin_dist_path").toString();
//         result.append(v);
//     }

//     return result;
// }

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
        v.unrealPath = obj.value("path").toString();
        v.unrealName = obj.value("name").toString();
        result.append(v);
    }

    return result;
}

bool Data::saveGeneralSettings( const S_GeneralSettings &generalSettingsIn)
{
    const QString folderPath = getJasonFilePath( getJsonFile_SettingsName() );

    QJsonArray platformArray, notificationArray,distributionArray, unrealEngineArray,
        generalSettingsArray;

    QJsonObject root, generalObj;

    for (const QString &p : generalSettingsIn.platformList) {
        platformArray.append(p);
    }

    for (const QString &p : generalSettingsIn.notifications) {
        notificationArray.append(p);
    }

    for (const QString &p : generalSettingsIn.distribution) {
        distributionArray.append(p);
    }

    for (const S_UnrealVersion &v : std::as_const( generalSettingsIn.S_UnrealVersionsList )) {
        QJsonObject obj;

        obj["unrealPath"] = v.unrealPath;
        obj["unrealName"] = v.unrealName;
        unrealEngineArray.append(obj);
    }

    generalObj["plugin_dist_path"] = generalSettingsIn.pluginDistPath ;

    // LAST PLUGIN
    generalObj["pluginPath"] = generalSettingsIn.S_LastPlugin.pluginPath ;
    generalObj["pluginName"] = generalSettingsIn.S_LastPlugin.pluginName ;
    generalObj["pluginVersion"] = generalSettingsIn.S_LastPlugin.pluginVersion;

    // UNREAL ENGINE
    // generalObj["unrealPath"] = generalSettingsIn.S_UnrealVersionsList.unrealPath ;
    // generalObj["unrealName"] = generalSettingsIn.S_UnrealVersionsList.unrealName ;
    // generalObj["isObsolete"] = generalSettingsIn.S_UnrealVersionsList.isObsolete;

    // OTHERS
    generalObj["platform_list"] = platformArray;
    generalObj["notifications"] = notificationArray;
    generalObj["distribution"] = distributionArray;
    generalObj["unrealEngine"] = unrealEngineArray;

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

// bool Data::saveSettings( const QString &plugin_dist_path)
// {
//     const QString folderPath = getJasonFilePath( getJsonFile_SettingsName() );

//     QJsonArray settings;

//     QJsonArray settings_General;

//     QJsonObject objectSettings, objectGeneral;


//     objectGeneral["plugin_dist_path"] = plugin_dist_path ;


//     settings_General.append( objectGeneral );
//     objectSettings["settings_general"] = settings_General ;

//     settings.append(objectSettings);


//     QJsonDocument doc(settings);

//     QFile file(folderPath);

//     if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
//         // gérer l'erreur (message, qWarning, etc.)

//         return false;
//     }

//     file.write(doc.toJson(QJsonDocument::Indented));
//     file.close();

//     return true;
// }

void Data::saveUnrealVersions(QList<S_UnrealVersion> unrealVersions)
{
    const QString filePath = getJasonFilePath( getJsonFile_UnrealVersionName() ) ;

    QJsonArray arr;

    for (const S_UnrealVersion &v : std::as_const( unrealVersions )) {
        QJsonObject obj;

        obj["path"] = v.unrealPath;
        obj["name"] = v.unrealName;
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

void Data::openLink(E_Reseaux reseaux) const
{
    QString reseau;

    switch (reseaux) {
    case FAB:
        reseau = fabLink;
        break;
    case DISCORD:
        reseau = discordLink;
        break;
    default:
        break;
    }

    QDesktopServices::openUrl( QUrl(reseau) );
}
