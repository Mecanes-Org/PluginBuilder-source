#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>

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

// VARIABLES
public:

private slots:
    void on_pushButton_findPlugin_clicked();

    void on_pushButton_build_clicked();

    void on_actionUnreal_Engine_triggered();

    void on_actionSettings_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
