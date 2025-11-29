#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QCursor>
#include <QCheckBox>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // INIT VARIABLES

    // QList<QString> unrealVersion;
    // unrealVersion.append("UE_5.5");
    // unrealVersion.append("UE_5.4");
    // unrealVersion.append("UE_5.3");
    // unrealVersion.append("UE_5.2");
    // unrealVersion.append("UE_5.1");
    // unrealVersion.append("UE_5.0");
    // unrealVersion.append("UE_5.5");
    // unrealVersion.append("UE_5.4");
    // unrealVersion.append("UE_5.3");
    // unrealVersion.append("UE_5.2");
    // unrealVersion.append("UE_5.1");
    // unrealVersion.append("UE_5.0");
    // unrealVersion.append("UE_5.5");
    // unrealVersion.append("UE_5.4");
    // unrealVersion.append("UE_5.3");
    // unrealVersion.append("UE_5.2");
    // unrealVersion.append("UE_5.1");
    // unrealVersion.append("UE_5.0");

    // for (const QString &val : unrealVersion) {
    //     QCheckBox *check = new QCheckBox(val, this);
    //     check->setCursor( QCursor(Qt::PointingHandCursor) );

    //     // Ajouter le checkbox dans le layout du scrollArea
    //     if (ui->scrollAreaWidgetContents_UE5->layout()) {
    //         ui->scrollAreaWidgetContents_UE5->layout()->addWidget(check);
    //     }
    // }





    ui->pushButton_build->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;

}

bool MainWindow::canBuild()
{
    QString label_pluginPath = ui->label_pluginPath->text();
    if( label_pluginPath.isEmpty() && label_pluginPath == "Plugin link" ){
        QMessageBox::warning(this, tr("Plugin Link"), tr("Incorrect Plugin Link"));
        return false;
    }

    bool unrealVersionChecked = false;

    QList<QCheckBox*> checkboxes = this->findChildren<QCheckBox*>();

    foreach (QCheckBox *cb, checkboxes) {
        if (cb->isChecked()) {
            unrealVersionChecked = true;
            break;
        }
    }

    if( !unrealVersionChecked ){
        QMessageBox::warning(this, tr("Unreal Version"), tr("Unreal version is not valid"));
        return false;
    }


    return true;
}

void MainWindow::on_pushButton_findPlugin_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Search your plugin"), "");
    QFile file(file_name);

    if(file_name.isEmpty()){
        QMessageBox::warning(this, tr("Error"), tr("Incorrect file"));
        return;
    }

    ui->label_pluginPath->setText(file_name);
    ui->pushButton_build->setDisabled(false);
}




void MainWindow::on_pushButton_build_clicked()
{

    QString label_pluginPath = ui->label_pluginPath->text();

    if( canBuild() ) {
        bool disabled = true;

        if( ui->pushButton_build->isEnabled() ){
            // Curseur attente sur toute la fenêtre
            this->setCursor(Qt::WaitCursor);

            QList<QCheckBox*> checkboxes = this->findChildren<QCheckBox*>();
            foreach (QCheckBox *cb, checkboxes) {
                cb->setDisabled( disabled );
            }

            QList<QPushButton*> pushButtons = this->findChildren<QPushButton*>();
            foreach (QPushButton *pushButton, pushButtons) {
                pushButton->setDisabled( disabled );
            }


            // ... ton traitement
            // Quand c'est fini :
            // this->unsetCursor();
            // ui->pushButton_build->setEnabled(true);
        }
    }



}

void MainWindow::on_actionUnreal_Engine_triggered()
{
    // AFFICHE UI SUR LES VERSION D UNREAL

}



void MainWindow::on_actionSettings_triggered()
{
    // AFFICHE UI SUR LES SETTINGS DU LOGICIEL
}

