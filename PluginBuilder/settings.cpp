#include "settings.h"
#include "ui_settings.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QCheckBox>
#include <QDir>
#include <QSettings>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


Settings::Settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Settings)
{
    ui->setupUi(this);


    mainWindow = qobject_cast<MainWindow*>(parent);  // note the *


    if (mainWindow) {
        generalSettings = mainWindow->getGeneralSettings();
    }

    QList<QCheckBox*> checkBoxList_platform = ui->frame_target_platform->findChildren<QCheckBox*>();
    QList<QCheckBox*> distributionList = ui->frame_post_build_tasks->findChildren<QCheckBox*>();

    // qDebug() << "Load A = " << data.getJsonFile_SettingsName() ;
    // qDebug() << "Load B = " << data.getJasonFilePath( data.getJsonFile_SettingsName() ) ;
    // qDebug() << "Load C = " << GeneralSettings.pluginDistPath ;

    if( generalSettings.pluginDistPath.length() > 0 ){
        ui->lineEdit_plugin_dist_path->setText( generalSettings.pluginDistPath );
    }

    foreach (QString val, data.getNotificationNames() ) {
        // parent = widget qui contient verticalLayout_CheckBoxNotif
        QCheckBox *checkboxNotif = new QCheckBox(ui->frame_notifications);
        checkboxNotif->setText(val);
        checkboxNotif->setChecked(notificationNameIsChecked(val));
        checkboxNotif->setCursor(QCursor(Qt::PointingHandCursor));

        ui->verticalLayout_CheckBoxNotif->addWidget(checkboxNotif);
    }

    // COCHE LES PLATFORM COCHER LORS DE LA SAVE
    foreach (QString val, generalSettings.platformList) {
        foreach (QCheckBox *valCheckBox, checkBoxList_platform) {
            if( valCheckBox->text() == val ){
                valCheckBox->setChecked(true);
            }
        }
    }

    // COCHE LES POST BUILDS TASK
    foreach (QString val, generalSettings.distribution) {
        foreach (QCheckBox *valCheckBox, distributionList) {
            if( valCheckBox->text() == val ){
                valCheckBox->setChecked(true);
            }
        }
    }


    // Font Familly
    QSettings s;

    const QString family = s.value("ui/fontFamily").toString();
    const int pointSize  = s.value("ui/fontPointSize", -1).toInt();

    ui->fontComboBox->setCurrentFont( QFont(family) );
    ui->spinBox_TextSize->setValue( pointSize );


}

Settings::~Settings()
{
    delete ui;
}

bool Settings::notificationNameIsChecked(QString &notificationName) const
{
    foreach (QString val, generalSettings.notifications) {
        if( val == notificationName ){
            return true;
        }
    }
    return false;
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
    QList<QString> platformList;
    QList<QString> notificationsList;
    QList<QString> distributionList;

    QList<QCheckBox*> checkBoxes_target_platform = ui->frame_target_platform->findChildren<QCheckBox*>();
    QList<QCheckBox*> checkBoxes_notifications = ui->frame_notifications->findChildren<QCheckBox*>();
    QList<QCheckBox*> checkBoxes_distribution = ui->frame_post_build_tasks->findChildren<QCheckBox*>();

    // qDebug() << "Taille : " << checkBoxes_distribution.length() ;


    foreach (QCheckBox *val, checkBoxes_target_platform) {

        if( val->isChecked() ){
            // qDebug() << "Name : " << val->text() << " \n" ;
            platformList.append( val->text() );
        }
    }

    foreach (QCheckBox *val, checkBoxes_notifications) {

        if( val->isChecked() ){
            // qDebug() << "Name : " << val->text() << " \n" ;
            notificationsList.append( val->text() );
        }
    }

    foreach (QCheckBox *val, checkBoxes_distribution) {

        if( val->isChecked() ){
            //qDebug() << "Name : " << val->text() << " \n" ;
            distributionList.append( val->text() );
        }
    }

    generalSettings.platformList = platformList;
    generalSettings.pluginDistPath = ui->lineEdit_plugin_dist_path->text();
    generalSettings.notifications = notificationsList;
    generalSettings.distribution = distributionList;


    if( !data.saveGeneralSettings( generalSettings ) ){
        QMessageBox::warning(this,tr("ERROR"), tr("Incorrect file."));
        return;
    }

    // UI CONFIG
    QString fam = ui->fontComboBox->currentFont().family();
    int basePt = ui->spinBox_TextSize->value();

    // qDebug() << basePt;

    data.setGlobalFont(fam, basePt );
    data.saveFontSettings( fam, basePt);

}

void Settings::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->buttonRole(button) == QDialogButtonBox::ResetRole){
        data.resetFontSettings();

        // Default Value
        ui->fontComboBox->setCurrentFont( QFont());
        ui->spinBox_TextSize->setValue(9);
    }

}

