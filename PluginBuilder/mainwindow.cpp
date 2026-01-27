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
#include "aboutus.h"
#include "changelog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // INIT VARIABLES
    lastPluginBuildIndex = 0;
    generalSettings = getGeneralSettings();

    runUatFilePath = R"(\Engine\Build\BatchFiles\RunUAT.bat)";

    if( data.getValidFileExist( data.getJasonFilePath( data.getJsonFile_SettingsName() ) ) ){

        for (const S_UnrealVersion &val : std::as_const( generalSettings.S_UnrealVersionsList ) ) {
            QCheckBox *check = new QCheckBox(val.unrealName, this);
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

    // UI



    // IS BETA VERSION ?
    if( data.getSoftwareVersion() < 1){

        QString textVersion = QString::number( data.getSoftwareVersion() );

        QLabel *label = new QLabel(this);
        label->setText("Beta Version - " +  QString::number( data.getSoftwareVersion() ));
        label->setStyleSheet(""
                             "color: yellow;"
                             "margin: 10px 0 10px 7px;"
                             "font-size: 15px;"
                             "");

        ui->statusbar->addPermanentWidget( label, 1 );
    }


    // UI - INIT
    ui->label_pluginPath->setText(
        generalSettings.S_LastPlugin.pluginPath.isEmpty()
            ? QString("Plugin Path")
            : generalSettings.S_LastPlugin.pluginPath
        );

    ui->lineEdit_pluginName->setText(
        generalSettings.S_LastPlugin.pluginName.isEmpty()
            ? QString()
            : generalSettings.S_LastPlugin.pluginName
        );


    ui->lineEdit_pluginVersion->setText(
        generalSettings.S_LastPlugin.pluginVersion.isEmpty()
            ? QString()
            : generalSettings.S_LastPlugin.pluginVersion
        );

    if( ui->label_pluginPath->text().length() < 1 ){
        ui->pushButton_build->setDisabled(true);
    }


    // SIGNAL
    connect(this, &MainWindow::buildFinished, this, &MainWindow::onBuildFinished);
}

MainWindow::~MainWindow()
{
    delete ui;
}

S_GeneralSettings MainWindow::getGeneralSettings()
{
    generalSettings = data.loadGeneralSettings();

    return generalSettings;
}

bool MainWindow::canBuild()
{
    QString label_pluginPath = ui->label_pluginPath->text();
    if( label_pluginPath.isEmpty() && label_pluginPath == "Plugin Path" ){
        QMessageBox::warning(this, tr("Plugin Path"), tr("Incorrect Plugin Path"));
        return false;
    }

    bool unrealVersionChecked = false;

    QList<QCheckBox*> checkboxes = ui->groupBox_UE5->findChildren<QCheckBox*>();

    if( checkboxes.length() < 1 ){
        QMessageBox::warning(this, tr("Unreal Version"), tr("Select at least one verified version of Unreal Engine"));
        return false;
    }

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
    QString pluginVersion = ui->lineEdit_pluginVersion->text();

    if( pluginName.isEmpty() ){
        QMessageBox::warning(this, tr("Plugin Name"), tr("Plugin Name is ivalid") );
        return;
    }

    if( pluginVersion.isEmpty() ){
        QMessageBox::warning(this, tr("Plugin Version"), tr("Plugin Version is ivalid") );
        return;
    }

    if( label_pluginPath == QString("Plugin Path")){
        QMessageBox::warning(this, tr("Plugin Path"), tr("Invalid plugin path") );
        return;
    }

    if( data.loadGeneralSettings( data.getJasonFilePath( data.getJsonFile_SettingsName() ) ).pluginDistPath.isEmpty() ){
        QMessageBox::warning(this, tr("Plugin-dist"), tr("The Plugin-dist folder could not be found."
                                                         "\n"
                                                         "Go to Edit > Settings > General.") );
        return;
    }


    if( unrealVersionsChecked.length() < 1 ){
        QMessageBox::warning(this, tr("Unreal Version"), tr("Be sure to select a version of Unreal Engine."
                                                            "\n"
                                                            "Add a version of Unreal Engine (if you don't have one)."
                                                            "\n"
                                                            "Go to Edit > Unreal Engine."
                                                            "\n"
                                                            "Otherwise, check one of the versions in the list (on the left) of versions.")  );
        return;
    }

    if (!isValidIndex(lastPluginBuildIndex, unrealVersionsChecked.size())) {
        QMessageBox::warning(this, tr("Unreal Version"),
                             tr("Internal error: invalid Unreal version index."));
        return;
    }

    // QString dirOutput = R"(C:/Users/MEC/Project/Programmation/)";

    QString dirOutput = "";

    if( data.getValidFileExist( data.getJasonFilePath( data.getJsonFile_SettingsName() ) ) ){
        dirOutput = generalSettings.pluginDistPath;

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


    // qDebug() << " 2 - " << label_pluginPath << "\n" ;
    // qDebug() << " 3 - " << dirOutput << "\n" ;
    // qDebug() << " 5 - " << pluginName << "\n" ;



    QCheckBox *currentCb = unrealVersionsChecked.at(lastPluginBuildIndex);
    QString unrealPath;


    foreach (S_UnrealVersion val, generalSettings.S_UnrealVersionsList) {

        if( currentCb->text() == val.unrealName ){
            unrealPath = val.unrealPath;

            break;
        }
    }

    if (unrealPath.isEmpty()) {
        QMessageBox::warning(this, tr("Unreal Version"),
                             tr("The selected Unreal Engine version is not configured."));
        return;
    }

    // qDebug() << " 6 - " << unrealVersionsChecked.at( lastPluginBuildIndex )->text() << "\n" ;
    // qDebug() << " Unreal path - " << unrealPath << "\n" ;


    if( canBuild() ) {
        bool disabled = true;

        if( ui->pushButton_build->isEnabled() ){

            // DISABLED UI
            ui->frame_unrealVersionsList->setDisabled(true);
            ui->frame_pluginConfig->setDisabled(true);
            this->menuBar()->setDisabled(true) ;

            this->setCursor(Qt::WaitCursor);

            startBuild(
                unrealPath,
                label_pluginPath,
                dirOutput,
                unrealVersionsChecked.at( lastPluginBuildIndex )->text(),
                pluginName,
                ("v"+pluginVersion ) );

            generalSettings.S_LastPlugin.pluginName = ui->lineEdit_pluginName->text();
            generalSettings.S_LastPlugin.pluginPath =  ui->label_pluginPath->text();
            generalSettings.S_LastPlugin.pluginVersion = ui->lineEdit_pluginVersion->text();

            data.saveGeneralSettings( generalSettings );

            // ... ton traitement
            // Quand c'est fini :
            // this->unsetCursor();
            // ui->pushButton_build->setEnabled(true);
        }
    }
}

void MainWindow::startBuild( const QString &engineDir, const QString &pluginPath, QString &outputDir, const QString &unrealVersion, const QString &pluginName, const QString &pluginVersion)
{
    QString concatPlatformeList;

    ui->label_log->setText( tr("Build ... \n"));

    QString runUatPathComplete = engineDir + runUatFilePath;
    QFileInfo runUatFolderPath(runUatPathComplete);

    outputDir = outputDir + pluginName + "_" + unrealVersion + "_" + pluginVersion;
    QString zipFileName =  pluginName + "_" + unrealVersion + "_" + pluginVersion;

    // qDebug() << QDir::toNativeSeparators(runUatPathComplete);
    // qDebug() << "-Plugin=" + QDir::toNativeSeparators( pluginPath );
    // qDebug() <<  "-Package=" + QDir::toNativeSeparators( outputDir );
    // qDebug() << "-TargetPlatforms=Win64";

    foreach (QString platform, generalSettings.platformList) {
        concatPlatformeList += ( "+" + platform );
    }

    concatPlatformeList = ( !concatPlatformeList.isEmpty() && concatPlatformeList.at(0) == '+')
                        ? concatPlatformeList.mid(1)
                        : concatPlatformeList;


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
        // qDebug().noquote() << data;

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

    // STDOUT
    QObject::connect(proc,
                     QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                     [this, unrealVersion, proc, outputDir, zipFileName](int exitCode, QProcess::ExitStatus status) {

                         bool ok = (status == QProcess::NormalExit && exitCode == 0);

                         // qDebug() << "Build finished, exitCode =" << exitCode << ", status =" << status;

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

                         if( ok ){
                            postBuild( outputDir, zipFileName );
                            showBuildNotification(true, unrealVersion);
                         }else{
                             // qDebug("Error");
                             showBuildNotification(false, unrealVersion);
                         }

                         emit buildFinished(ok, exitCode, QString("\nBuild finished (exitCode=%1)").arg(exitCode));

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

void MainWindow::postBuild( QString outputDir, QString zipFileName )
{
    if( generalSettings.distribution.length() < 1 ){
        return;
    }

    QDir dir;

    if( dir.exists( outputDir + "/Intermediate" ) ){
        dir.setPath( outputDir + "/Intermediate" );
        dir.removeRecursively();


    }

    if( dir.exists( outputDir + "/Binaries" ) ){
        dir.setPath( outputDir + "/Binaries" );
        dir.removeRecursively();
    };

    // ZIP
    zipFileExtern(outputDir, zipFileName);

}

void MainWindow::zipFileExtern(const QString &filePath, const QString &zipFileName)
{

    QString message = "Impossible de supprimer le dossier : " + filePath;

    if (!QFile::exists(filePath)) {
        //qWarning() << "Fichier/dossier introuvable:" << filePath;
        message = tr("File/folder not found %1").arg(filePath);
        showBuildNotification(false, "", message);
        return;
    }

    QString program = QCoreApplication::applicationDirPath() + "/zipper.exe";

    if (!QFile::exists(program)) {
        // qWarning() << "zipper.exe introuvable:" << program;
        message = tr("zipper.exe not found: %1").arg(program);
        showBuildNotification(false, "", message);
        return;
    }

    QString outputZip = QDir::toNativeSeparators(
        QFileInfo(filePath).absolutePath() + "/" + zipFileName + ".zip");

    QStringList args;
    args << filePath << outputZip;

    QProcess process;
    process.start(program, args);

    if (!process.waitForFinished()) {
        // qWarning() << "Le processus zipper n'a pas pu se terminer correctement";
        // qWarning() << process.errorString();

        message = tr("The zipper process could not be completed successfully.").arg( process.errorString() );
        showBuildNotification(false, "", message);
        return;
    }

    // RETOUR DU ZIPPER
    int exitCode = process.exitCode();

    if (exitCode == 0) {
        // ici tu es sûr que zipper s'est terminé correctement
        QDir dirToRemove(filePath);
        if (dirToRemove.exists()) {
            if (dirToRemove.removeRecursively()) {
                // message = tr("Successfully zipped directory %1").arg(zipFileName);
                // showBuildNotification(true, "", message);
            }else{
                message = tr("Cannot delete folder %1").arg(zipFileName);
                showBuildNotification(false, "", message);
            };
        } else {
            QFile fileToRemove(filePath);
            if (!fileToRemove.remove()) {
                message = tr("Cannot delete file %1").arg(zipFileName);
                showBuildNotification(false, "", message);
            }
        }
    }

    // qDebug() << "Exit code:" << process.exitCode();
    // qDebug() << "Stdout:" << process.readAllStandardOutput();
    // qDebug() << "Stderr:" << process.readAllStandardError();

    // qDebug() << "Debug - Archive" << outputZip;
}


bool MainWindow::isValidIndex(int &index, int arraySize)
{
    return !( index < 0 || index >= arraySize );
}


void MainWindow::setUnrealVersions(QList<S_UnrealVersion> newUnrealVersions)
{
    // unrealVersions = newUnrealVersions;
    generalSettings.S_UnrealVersionsList = newUnrealVersions;

    clearLayout(ui->scrollAreaWidgetContents_UE5->layout());


    for (const S_UnrealVersion &val : std::as_const(generalSettings.S_UnrealVersionsList) ) {
        QCheckBox *check = new QCheckBox(val.unrealName, this);
        check->setCursor( QCursor(Qt::PointingHandCursor) );

        // Ajouter le checkbox dans le layout du scrollArea
        if (ui->scrollAreaWidgetContents_UE5->layout()) {
            ui->scrollAreaWidgetContents_UE5->layout()->addWidget(check);
        }
    }
}

void MainWindow::showBuildNotification(const bool &success, const QString &unrealVersion, const QString &text)
{
    // bool enabledBuildNotif = false;

    // foreach (QString text , generalSettings.notifications) {
    //     enabledBuildNotif = ( text.toLower() == "build" );
    // }

    // if( !enabledBuildNotif ){
    //     // qDebug() << "NO NOTIF";
    //     return;
    // }


    QLabel *textToShow = new QLabel(this);

    if( !unrealVersion.isEmpty() ){
        textToShow->setText( "[ " + unrealVersion + " ]"  );
    }

    if( success ){
        textToShow->setText( textToShow->text() + ( text.isEmpty() ? " Build successful" : text ) );
        textToShow->setStyleSheet(""
                                  "color: rgba(0, 255, 0, 0.85);"
                                  "");
    }else{
        textToShow->setText( textToShow->text() + ( text.isEmpty() ? " Build failed" : text ) );
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
    lastPluginBuildIndex = 0;
    unrealVersionsChecked.clear();

    // CLEAR LAYOUT AVANT D AJOUTER DES LOGS
    QLayout *layout = ui->scrollAreaWidgetContents_Notification->layout();
    clearLayout(layout);

    QList<QCheckBox*> checkboxes = ui->groupBox_UE5->findChildren<QCheckBox*>();

    foreach (QCheckBox *cb, checkboxes) {
        if (cb->isChecked()) {
            unrealVersionsChecked.append(cb);
        }
    }

    // qDebug() << "Length : " << unrealVersionsChecked.length();

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

void MainWindow::on_actioninfos_triggered()
{
    AboutUs aboutUs(this);
    aboutUs.exec();
}


void MainWindow::on_actionChangelog_triggered()
{
    ChangeLog changelog;
    changelog.exec();
}

