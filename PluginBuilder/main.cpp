#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "loadingwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "PluginBuilder_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    LoadingWindow loading;




    // 3) Fenêtre principale
    MainWindow w;/*

    // &loading, &w; { ... }
    // - C’est une lambda (fonction anonyme C++).
    // Le bloc { loading.close(); w.show(); } est ce qui sera exécuté quand loadingFinished est émis.

    QObject::connect(&loading, &LoadingWindow::loadingFinished,
                     [&loading, &w]() {
                         loading.close();
                         w.show();
                     });

    loading.show();*/


    w.show();

    return app.exec();
}
