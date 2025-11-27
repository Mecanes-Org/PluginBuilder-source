#include "loadingwindow.h"
#include "ui_loadingwindow.h"

#include <QRandomGenerator>

LoadingWindow::LoadingWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoadingWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::SplashScreen);

    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);

    connect(&m_timer, &QTimer::timeout, this, &LoadingWindow::onTimeout);
    m_timer.start(100); // 100 ms entre chaque incrément
}

LoadingWindow::~LoadingWindow()
{
    delete ui;
}

void LoadingWindow::onTimeout()
{
    m_value += QRandomGenerator::global()->bounded(1,4); // entre 1 et 4
    ui->progressBar->setValue(m_value);

    if (m_value >= 100) {
        m_timer.stop();
        emit loadingFinished(); // signal pour dire que c'est terminé
    }
}
