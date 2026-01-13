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
    MainWindow w;

#ifdef QT_DEBUG

    w.setWindowIcon(QIcon(":/resources/images/logo/PluginBuilder.png"));

    // &loading, &w; { ... }
    // - C’est une lambda (fonction anonyme C++).

    w.show();

#else
    QObject::connect(&loading, &LoadingWindow::loadingFinished,
                     [&loading, &w]() {
                         loading.close();
                         w.show();
                     });

    loading.show();
#endif

    return app.exec();
}
