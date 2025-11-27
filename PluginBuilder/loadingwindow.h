#ifndef LOADINGWINDOW_H
#define LOADINGWINDOW_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class LoadingWindow;
}

class LoadingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoadingWindow(QWidget *parent = nullptr);
    ~LoadingWindow();

signals:
    void loadingFinished();   // pour prévenir que c'est fini

private slots:
    void onTimeout();         // appelé à chaque tick du timer

private:
    Ui::LoadingWindow *ui;

    QTimer m_timer;
    int m_value = 0;
};

#endif // LOADINGWINDOW_H
