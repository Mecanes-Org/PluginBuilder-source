#include "data.h"

#include <QFile>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>

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
