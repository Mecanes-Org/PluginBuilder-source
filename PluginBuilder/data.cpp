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


void Data::restoreFontSettings()
{
    QSettings s;

    const QString family = s.value("ui/fontFamily").toString();
    const int pointSize  = s.value("ui/fontPointSize", -1).toInt();

    if (!family.isEmpty() && pointSize > 0)
        setGlobalFont(family, pointSize);
}



void Data::saveFontSettings(const QString& family, int pointSize, bool darkMode)
{
    s.beginGroup("ui");
    s.setValue("fontFamily", family);
    s.setValue("fontPointSize", pointSize);
    s.setValue("darkMode", darkMode);
    s.endGroup();
}



void Data::setGlobalFont(const QString& family, int pointSize)
{
    auto* app = qobject_cast<QApplication*>(QCoreApplication::instance());
    if (!app) return;

    QString qss = app->styleSheet();

    const QString begin = "/*__FONT_OVERRIDE_BEGIN__*/";
    const QString end   = "/*__FONT_OVERRIDE_END__*/";

    // Enlever l'ancien bloc s'il existe
    int b = qss.indexOf(begin);
    int e = qss.indexOf(end);
    if (b != -1 && e != -1 && e > b)
        qss.remove(b, (e - b) + end.size());

    // Si reset demandé => on applique le qss nettoyé (sans override)
    if (family.isEmpty()) {
        app->setStyleSheet(qss);
        return;
    }

    // Sinon on ajoute le bloc override
    qss += "\n" + begin + "\n";
    qss += QString(
               "QWidget {\n"
               "  font-family: \"%1\";\n"
               "  font-size: %2pt;\n"
               "}\n"
               ).arg(family).arg(pointSize);
    qss += end + "\n";

    app->setStyleSheet(qss);
}


void Data::resetFontSettings()
{
    s.remove("ui");
    setGlobalFont();
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

    QString filePath = d.filePath("data/" + filename);
    return filePath;
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

    // UNREAL ENGINE
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

QString Data::getJsonFile_SettingsName() const
{
    return jsonFile_SettingsName;
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
