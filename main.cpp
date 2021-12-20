#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QtEndian>

#include "framemanager.h"
#include "chart.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    app.setOrganizationName("Rabbit House");
    app.setOrganizationDomain("rabbithouse.org");
    app.setApplicationName("φsap");

    unsigned int a = 0xffeeccdd;
    printf("%x\n", qFromBigEndian(a));

    LinesModel judgeLines;

    NotesModel taps;
    NotesModel drags;
    NotesModel flicks;
    HoldsModel holds;

    FrameManager frameManager;
    frameManager.bindModels(&judgeLines, &taps, &drags, &flicks, &holds);

    auto chart = Chart::loadFile("/home/chino/Chart_IN.json");
    chart.markDualNotes();
    chart.preprocess();

    frameManager.loadChart(&chart);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    auto rootContext = engine.rootContext();
    rootContext->setContextProperty("judgeLines", &judgeLines);
    rootContext->setContextProperty("tapNotes", &taps);
    rootContext->setContextProperty("dragNotes", &drags);
    rootContext->setContextProperty("flickNotes", &flicks);
    rootContext->setContextProperty("holdNotes", &holds);

    rootContext->setContextProperty("frameManager", &frameManager);

    engine.load(url);

    return app.exec();
}
