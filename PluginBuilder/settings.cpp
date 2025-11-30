#include "settings.h"
#include "ui_settings.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>


#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

Settings::Settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Settings)
{
    ui->setupUi(this);

    settingsList = data.loadSettings( data.getJasonFilePath( data.getJsonFile_SettingsName() ) );

    if( data.getValidFileExist( data.getJasonFilePath( data.getJsonFile_SettingsName() ) ) ){
        ui->lineEdit_plugin_dist_path->setText( settingsList.at(0) );
    }

}

Settings::~Settings()
{
    delete ui;
}


void Settings::saveSettings()
{
    const QString folderPath = data.getJasonFilePath( data.getJsonFile_SettingsName() );

    QJsonArray arr;

    QJsonObject obj;

    obj["plugin_dist_path"] = ui->lineEdit_plugin_dist_path->text();
    arr.append(obj);


    QJsonDocument doc(arr);

    QFile file(folderPath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        // gérer l'erreur (message, qWarning, etc.)
        QMessageBox::warning(this,tr("ERROR"), tr("Incorrect file."));
        return;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}

void Settings::on_pushButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Unreal ( Ex : UE_5.7)"), defaultDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if( !dir.isEmpty() ){
        ui->lineEdit_plugin_dist_path->setText(dir);
    }else{
        QMessageBox::warning(this, tr("Error"), tr("Incorrect file"));
    }
}


void Settings::on_buttonBox_accepted()
{
    saveSettings();
}

