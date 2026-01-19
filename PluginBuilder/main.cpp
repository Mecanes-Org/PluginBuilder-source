#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <QFile>
#include <QTextStream>

#include "loadingwindow.h"

static void applyDarkTheme(QApplication& app)
{
    QFile f(":/themes/theme_dark.qss"); // si tu le mets dans un .qrc
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream ts(&f);
        app.setStyleSheet(ts.readAll());
    }
}

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

    app.setStyle("Fusion");
    applyDarkTheme(app);


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
