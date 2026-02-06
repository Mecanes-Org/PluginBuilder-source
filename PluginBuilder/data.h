#ifndef DATA_H
#define DATA_H

#include <QObject>


struct S_UnrealVersion
{
    QString unrealPath;
    QString unrealName;
    bool isObsolete = false;
};

struct S_LastPlugin
{
    QString pluginPath;
    QString pluginName;
    QString pluginVersion;
};

struct S_GeneralSettings {
    QString pluginDistPath;
    S_LastPlugin S_LastPlugin;
    QList<S_UnrealVersion> S_UnrealVersionsList;
    QList<QString> platformList;
    QList<QString> notifications;
    QList<QString> distribution;
};


enum E_Reseaux{
    FAB,
    DISCORD
};



class Data : public QObject
{
    Q_OBJECT
public:
    explicit Data(QObject *parent = nullptr);

    bool getValidFileExist(const QString &filePath);

    // RENVOI LE PATH VERS LE FICHIER JSON
    QString getJasonFilePath( const QString &filename );


    // LOAD DATA
    S_GeneralSettings loadGeneralSettings( const QString &filePath = "" );

    // SAVE DATA
    bool saveGeneralSettings( const S_GeneralSettings &generalSettingsIn);

    QString getJsonFile_SettingsName() const ;
    QString getJsonFile_UnrealVersionName() const;

    // GETTER AND SETTER
    float getSoftwareVersion() const { return softwareVersion; }
    QList<QString> getNotificationNames() const { return notificationNames;}

    // RESEAUX
    void openLink(E_Reseaux reseaux) const;

private :
    QString jsonFile_SettingsName = "settings.json";
    float softwareVersion = 0.8;

    QList<QString> notificationNames;

    // RESEAUX

    QString fabLink = "https://www.fab.com/sellers/Mecanes";
    QString discordLink = "https://discord.com/invite/xb78rVQ7F2";

};

#endif // DATA_H
