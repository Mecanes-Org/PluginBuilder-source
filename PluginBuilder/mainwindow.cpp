#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QCursor>
#include <QCheckBox>
#include <QFrame>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QDebug>

#include "unrealversions.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // INIT VARIABLES

    runUatFilePath = R"(\Engine\Build\BatchFiles\RunUAT.bat)";

    if( data.getValidFileExist( data.getJasonFilePath( data.getJsonFile_UnrealVersionName() ) ) ){
        unrealVersions = data.loadUnrealVersions( data.getJasonFilePath( data.getJsonFile_UnrealVersionName() ) );

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


    // SIGNAL
    connect(this, &MainWindow::buildFinished, this, &MainWindow::onBuildFinished);
}

MainWindow::~MainWindow()
{
    delete ui;

}

S_GeneralSettings MainWindow::getGeneralSettings()
{
    return data.loadGeneralSettings();
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

void MainWindow::prepareToBuild()
{
    QString label_pluginPath = ui->label_pluginPath->text();
    QString pluginName = ui->lineEdit_pluginName->text();

    if( pluginName.isEmpty() ){
        QMessageBox::warning(this, tr("Plugin Name"), tr("Plugin Name is ivalid") );
        return;
    }

    if( data.loadGeneralSettings( data.getJasonFilePath( data.getJsonFile_SettingsName() ) ).pluginDistPath.isEmpty() ){
        QMessageBox::warning(this, tr("Plugin-dist"), tr("The Plugin-dist folder could not be found."
                                                         "\n"
                                                         "Go to Edit > Settings > General.") );
        return;
    }

    S_GeneralSettings general_settings = getGeneralSettings();

    // QString dirOutput = R"(C:/Users/MEC/Project/Programmation/)";

    QString dirOutput = "";

    if( data.getValidFileExist( data.getJasonFilePath( data.getJsonFile_SettingsName() ) ) ){
        dirOutput = general_settings.pluginDistPath;

        dirOutput.replace('\\', '/');

        if (!dirOutput.endsWith('/'))
            dirOutput += '/';
    }

    if( dirOutput.isEmpty() ){
        QMessageBox::warning(this, "Plugin - Dist", tr("The location where the compiled plugins are stored is invalid."
                                                       "\n"
                                                       "Go to Edit > Settings > General."));
        return;
    }


    if( canBuild() ) {
        bool disabled = true;

        if( ui->pushButton_build->isEnabled() ){

            // DISABLED UI
            ui->frame_unrealVersionsList->setDisabled(true);
            ui->frame_pluginConfig->setDisabled(true);
            this->menuBar()->setDisabled(true) ;

            this->setCursor(Qt::WaitCursor);

            startBuild(
                unrealVersions.at( lastPluginBuildIndex ).path,
                label_pluginPath,
                dirOutput,
                unrealVersions.at( lastPluginBuildIndex ).name,
                pluginName,
                "v0.4");

            // ... ton traitement
            // Quand c'est fini :
            // this->unsetCursor();
            // ui->pushButton_build->setEnabled(true);
        }
    }
}

void MainWindow::startBuild( const QString &engineDir, const QString &pluginPath, QString &outputDir, const QString &unrealVersion, const QString &pluginName, const QString &pluginVersion)
{
    S_GeneralSettings general_settings = getGeneralSettings();
    QString concatPlatformeList;

    ui->label_log->setText( tr("Build ... \n"));

    QString runUatPathComplete = engineDir + runUatFilePath;
    QFileInfo runUatFolderPath(runUatPathComplete);

    outputDir = outputDir + pluginName + "_" + unrealVersion + "_" + pluginVersion;

    // qDebug() << QDir::toNativeSeparators(runUatPathComplete);
    // qDebug() << "-Plugin=" + QDir::toNativeSeparators( pluginPath );
    // qDebug() <<  "-Package=" + QDir::toNativeSeparators( outputDir );
    // qDebug() << "-TargetPlatforms=Win64";

    foreach (QString platform, general_settings.platformList) {
        concatPlatformeList += ( "+" + platform );
    }

    // qDebug() << "++++++++++++++++++ \n";
    // qDebug() << concatPlatformeList;
    // qDebug() << "++++++++++++++++++ \n";


    QStringList args;
    args << "BuildPlugin"
         << "-Plugin=" + pluginPath
         << "-Package=" + outputDir
         << "-Rocket"
         << "-VS2022"
         << "-Progress"
         << "-TargetPlatforms=" + ( concatPlatformeList.isEmpty() ? "Win64" : concatPlatformeList );


    QProcess *proc = new QProcess;

    // voir les logs en temps réel dans la console Qt
    // STDOUT
    QObject::connect(proc, &QProcess::readyReadStandardOutput, [this, proc]() {

        // Lire une seule fois le buffer
        QByteArray data = proc->readAllStandardOutput();

        // Affiche en debug
        qDebug().noquote() << data;

        // Convertir en QString
        QString text = QString::fromLocal8Bit(data);

        // Ajoute a chaque fois
        ui->label_log->setText( ui->label_log->text() + text );


    });

    // STDERR
    QObject::connect(proc, &QProcess::readyReadStandardError, [this, &unrealVersion, proc]() {
        // qDebug().noquote() << proc->readAllStandardError();

        QString text = QString::fromLocal8Bit(proc->readAllStandardError());

        // Ajouter les erreurs à la suite
        ui->label_log->setText( ui->label_log->text() + "\n[ERR] " + text );


        showBuildNotification(false, unrealVersion);

    });

    QObject::connect(proc,
                     QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                     [this, unrealVersion, proc](int exitCode, QProcess::ExitStatus status) {

                         bool ok = (status == QProcess::NormalExit && exitCode == 0);

                         qDebug() << "Build finished, exitCode =" << exitCode << ", status =" << status;

                         ui->label_log->setText(
                             ui->label_log->text()
                             + QString("\nBuild finished (exitCode=%1)").arg(exitCode)
                             );


                         // DISABLED UI
                         ui->frame_unrealVersionsList->setDisabled(false);
                         ui->frame_pluginConfig->setDisabled(false);
                         this->menuBar()->setDisabled(false) ;
                         this->setCursor(QCursor(Qt::ArrowCursor));

                         proc->deleteLater();

                         emit buildFinished(ok, exitCode, QString("\nBuild finished (exitCode=%1)").arg(exitCode));

                         showBuildNotification(false, unrealVersion);
                     });

    proc->setProgram( runUatPathComplete );
    proc->setArguments(args);
    proc->setWorkingDirectory( runUatFolderPath.absolutePath() );
    proc->start();

    if (!proc->waitForStarted(3000)) {
        // qDebug() << "QProcess failed to start:" << proc->errorString();
        ui->label_log->setText(
            ui->label_log->text()
            + QString("\nQProcess failed to start:").arg( proc->errorString() )
            );

        return;
    }



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

void MainWindow::showBuildNotification(const bool &success, const QString &unrealVersion, const QString &text)
{
    QLabel *textToShow = new QLabel(this);
    textToShow->setText( "[ " + unrealVersion + " ]"  );

    if( success ){
        textToShow->setText( textToShow->text() + ( text.isEmpty() ? " Build Success" : text ) );
    }else{
        textToShow->setText( textToShow->text() + ( text.isEmpty() ? " Error Build" : text ) );
        textToShow->setStyleSheet(""
                                 "color: rgba(255,0, 0, 0.85);"
                                 "");
    }

    ui->scrollAreaWidgetContents_Notification->layout()->addWidget( textToShow );
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

QList<S_UnrealVersion> MainWindow::getUnrealVersions()
{
    return unrealVersions;
}


void MainWindow::on_pushButton_findPlugin_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Search your plugin"), "", "file (*.uplugin)");
    QFile file(file_name);

    if(file_name.isEmpty()){
        QMessageBox::warning(this, tr("Error"), tr("No file selected"));
        return;
    }

    ui->label_pluginPath->setText(file_name);
    ui->pushButton_build->setDisabled(false);
}


void MainWindow::on_pushButton_build_clicked()
{


    // CLEAR LAYOUT AVANT D AJOUTER DES LOGS
    QLayout *layout = ui->scrollAreaWidgetContents_Notification->layout();
    clearLayout(layout);

    QList<QCheckBox*> checkboxes = this->findChildren<QCheckBox*>();

    foreach (QCheckBox *cb, checkboxes) {
        if (cb->isChecked()) {
            unrealVersionsChecked.append(cb);
        }
    }

    prepareToBuild();

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
    Settings settingsUI(this);
    settingsUI.exec();
}

// APRES LA FIN DU BUILD
void MainWindow::onBuildFinished(bool success, int exitCode,const QString &logMessage)
{
    // QString title = success ? "Build succeeded" : "Build failed";
    // QString text  = success
    //                    ? QString("Plugin build succeeded.\nExit code: %1").arg(exitCode)
    //                    : QString("Plugin build failed.\nExit code: %1\n\nLog:\n%2")
    //                          .arg(exitCode)
    //                          .arg(logMessage);

    // QMessageBox::information(this, title, text);
    // ou un QDialog personnalisé si tu préfères

    if( lastPluginBuildIndex < ( unrealVersionsChecked.length() -1 )  ){
        ++lastPluginBuildIndex;
        prepareToBuild();

    }else{
        unrealVersionsChecked.clear();
    }

}
