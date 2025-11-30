#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QCursor>
#include <QCheckBox>
#include <QPushButton>
#include <QDir>
#include <QFile>

#include "unrealversions.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // INIT VARIABLES
    if( getValidFileExist( getUnrealVersionsFilePath() ) ){
        unrealVersions = loadUnrealVersions( getUnrealVersionsFilePath() );

        for (const S_UnrealVersion &val : std::as_const( unrealVersions ) ) {
            QCheckBox *check = new QCheckBox(val.name, this);
            check->setCursor( QCursor(Qt::PointingHandCursor) );

            // SI C EST OBSELETE ALORS UPDATE THE COLOR
            if( val.isObsolete ){
                check->setStyleSheet(

                    "QCheckBox {"
                        "color: #BB4D1A;"
                    "}"

                    "QCheckBox::indicator {"
                         "border-style: solid;"
                         "border-width: 1px;"
                         "border-color: #BB4D1A;"
                         "color: #BB4D1A;"
                    "}"

                    );

            }

            // Ajouter le checkbox dans le layout du scrollArea
            if (ui->scrollAreaWidgetContents_UE5->layout()) {
                ui->scrollAreaWidgetContents_UE5->layout()->addWidget(check);
            }
        }
    }


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

void MainWindow::setUnrealVersions(QList<S_UnrealVersion> newUnrealVersions)
{
    unrealVersions = newUnrealVersions;

    clearLayout(ui->scrollAreaWidgetContents_UE5->layout());

    for (const S_UnrealVersion &val : std::as_const(unrealVersions) ) {
        QCheckBox *check = new QCheckBox(val.name, this);
        check->setCursor( QCursor(Qt::PointingHandCursor) );

        // Ajouter le checkbox dans le layout du scrollArea
        if (ui->scrollAreaWidgetContents_UE5->layout()) {
            ui->scrollAreaWidgetContents_UE5->layout()->addWidget(check);
        }
    }
}

void MainWindow::clearLayout(QLayout *layout)
{
    if (!layout)
        return;

    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (QWidget *w = item->widget()) {
            w->deleteLater();   // détruire le widget
        }
        if (QLayout *childLayout = item->layout()) {
            clearLayout(childLayout);   // si sous-layout
        }
        delete item;
    }
}

QString MainWindow::getUnrealVersionsFilePath()
{
    QString dir = QCoreApplication::applicationDirPath();
    QDir d(dir);

    // sous-dossier "data"
    d.mkpath("data");


    return d.filePath("data/unreal_versions.json");
}

bool MainWindow::getValidFileExist(const QString &filePath)
{
    if (QFile::exists(filePath)) {
        return true;
    }

    return false;
}

void MainWindow::saveUnrealVersions()
{

    QString dir = QCoreApplication::applicationDirPath();
    QDir d(dir);
    // Optionnel: sous-dossier "data"
    d.mkpath("data");

    const QString filePath = getUnrealVersionsFilePath();

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

QList<S_UnrealVersion> MainWindow::loadUnrealVersions( const QString &filePath)
{

    QList<S_UnrealVersion> result;
    QFile file( ( filePath.isEmpty() ) ? getUnrealVersionsFilePath() : filePath );


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


    UnrealVersions unrealVersions(this);
    unrealVersions.exec();

}



void MainWindow::on_actionSettings_triggered()
{
    // AFFICHE UI SUR LES SETTINGS DU LOGICIEL
}


void MainWindow::on_actionUnreal_Engine_changed()
{

}

