#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <QFontComboBox>

#include "loadingwindow.h"
#include "data.h"


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
    app.setWindowIcon(QIcon(":/resources/images/logo/PluginBuilder.png"));

    QCoreApplication::setOrganizationName("Mecanes");
    QCoreApplication::setApplicationName("PluginBuilder");


    // Versionning
    Data data;

    QCoreApplication::setApplicationVersion( QString::number( data.getSoftwareVersion() ) );

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "PluginBuilder_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    // qDebug() << QCoreApplication::applicationVersion();

    app.setStyle("Fusion");
    applyDarkTheme(app);


    LoadingWindow loading;

    // 3) Fenêtre principale
    MainWindow w;
    w.setWindowIcon(QIcon(":/resources/images/logo/PluginBuilder.png"));

#ifdef QT_DEBUG

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


    data.restoreFontSettings();

    return app.exec();
}
