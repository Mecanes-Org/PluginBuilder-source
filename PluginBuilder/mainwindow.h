#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>

#include "data.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool canBuild();
    void prepareToBuild();
    void startBuild( const QString &engineDir, const QString &pluginPath, QString &outputDir, const QString &unrealVersion, const QString &pluginName, const QString &pluginVersion);


    void setUnrealVersions(QList<S_UnrealVersion> newUnrealVersions);

    // CLEAR LAYOUT
    void clearLayout(QLayout *layout);

    QList<S_UnrealVersion> getUnrealVersions();

private slots:
    void on_pushButton_findPlugin_clicked();

    void on_pushButton_build_clicked();

    void on_actionUnreal_Engine_triggered();

    void on_actionSettings_triggered();

    void onBuildFinished(bool success, int exitCode,const QString &logMessage);

signals:
    void buildFinished(bool success, int exitCode, const QString &logMessage);

private:
    Ui::MainWindow *ui;

    QList<S_UnrealVersion> unrealVersions;
    QList<QCheckBox*> unrealVersionsChecked;
    QString runUatFilePath;

    // l index du dernier plugin qui a été build
    int lastPluginBuildIndex;
    Data data;

};
#endif // MAINWINDOW_H
