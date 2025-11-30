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

    void setUnrealVersions(QList<S_UnrealVersion> newUnrealVersions);

    // CLEAR LAYOUT
    void clearLayout(QLayout *layout);

public:
    // GETTER & SETTER
    QList<S_UnrealVersion> getUnrealVersions() const {return unrealVersions;};

    // CHOISI UNE ROOT POUR LA SAVE DE UNREAL VERSION
    QString getUnrealVersionsFilePath();
    bool getValidFileExist(const QString &filePath);

    // SAVE TO UNREAL VERSIONS
    void saveUnrealVersions();
    QList<S_UnrealVersion> loadUnrealVersions( const QString &filePath );

private slots:
    void on_pushButton_findPlugin_clicked();

    void on_pushButton_build_clicked();

    void on_actionUnreal_Engine_triggered();

    void on_actionSettings_triggered();

    void on_actionUnreal_Engine_changed();

private:
    Ui::MainWindow *ui;

    QList<S_UnrealVersion> unrealVersions;
};
#endif // MAINWINDOW_H
