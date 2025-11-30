#ifndef DATA_H
#define DATA_H

#include <QObject>


struct S_UnrealVersion
{
    QString path;
    QString name;
    bool isObsolete;
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
    QList<QString> loadSettings( const QString &filePath );
    QList<S_UnrealVersion> loadUnrealVersions( const QString &filePath );

    // SAVE DATA
    void saveUnrealVersions(QList<S_UnrealVersion> unrealVersions);




    // GETTER & SETTER
    QString getJsonFile_SettingsName() const ;

    QString getJsonFile_UnrealVersionName() const;

private :
    QString jsonFile_SettingsName = "settings.json";
    QString jsonFile_UnrealVersionName = "unreal_versions.json";

};

#endif // DATA_H
