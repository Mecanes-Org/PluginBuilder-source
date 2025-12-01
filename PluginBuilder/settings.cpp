#include "settings.h"
#include "ui_settings.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QCheckBox>
#include <QDir>


#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

Settings::Settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Settings)
{
    ui->setupUi(this);

    GeneralSettings = data.loadGeneralSettings( data.getJasonFilePath( data.getJsonFile_SettingsName() ) );

    // qDebug() << "Load A = " << data.getJsonFile_SettingsName() ;
    // qDebug() << "Load B = " << data.getJasonFilePath( data.getJsonFile_SettingsName() ) ;
    // qDebug() << "Load C = " << GeneralSettings.pluginDistPath ;

    if( data.getValidFileExist( data.getJasonFilePath( data.getJsonFile_SettingsName() ) ) ){
        ui->lineEdit_plugin_dist_path->setText( GeneralSettings.pluginDistPath );
    }

}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_pushButton_find_plugin_dist_clicked()
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
    S_GeneralSettings generalSettings ;

    QList<QString> platformList;

    QList<QCheckBox*> checkBoxes = this->findChildren<QCheckBox*>();


    // qDebug() << "Taille : " << checkBoxes.length() ;


    foreach (QCheckBox *val, checkBoxes) {

        if( val->isChecked() ){
            // qDebug() << "Name : " << val->text() << " \n" ;
            platformList.append( val->text() );
        }
    }

    generalSettings.platformList = platformList;
    generalSettings.pluginDistPath = ui->lineEdit_plugin_dist_path->text();


    if( !data.saveGeneralSettings( generalSettings ) ){
        QMessageBox::warning(this,tr("ERROR"), tr("Incorrect file."));
        return;
    }


}

