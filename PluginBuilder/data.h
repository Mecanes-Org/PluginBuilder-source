#ifndef DATA_H
#define DATA_H

#include <QObject>


struct S_UnrealVersion
{
    QString path;
    QString name;
    bool isObsolete;
};

struct S_GeneralSettings {
    QString pluginDistPath;
    QList<QString> platformList;
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
    QList<QString> loadSettings( const QString &filePath );
    QList<S_UnrealVersion> loadUnrealVersions( const QString &filePath );

    // SAVE DATA
    bool saveGeneralSettings( const S_GeneralSettings &generalSettingsIn);
    bool saveSettings( const QString &plugin_dist_path );
    void saveUnrealVersions(QList<S_UnrealVersion> unrealVersions);

    QString getJsonFile_SettingsName() const ;
    QString getJsonFile_UnrealVersionName() const;

    // GETTER AND SETTER
    float getSoftwareVersion() const { return softwareVersion; }

private :
    QString jsonFile_SettingsName = "settings.json";
    QString jsonFile_UnrealVersionName = "unreal_versions.json";
    float softwareVersion;
};

#endif // DATA_H
