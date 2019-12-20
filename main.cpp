#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "linearindicator.h"
#include "artificialhorizon.h"
#include "horizontalsituationindicator.h"
#include "autopilot.h"
#include "autopilotmaplayer.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<LinearIndicator>("io.smth", 1, 0, "LinearIndicator");
    qmlRegisterType<ColorSegment>("io.smth", 1, 0, "ColorSegment");
    qmlRegisterType<ArtificialHorizon>("io.smth", 1, 0, "ArtificialHorizon");
    qmlRegisterType<HorizontalSituationIndicator>("io.smth", 1, 0, "HorizontalSituationIndicator");
    qmlRegisterType<AutopilotMapLayer>("io.smth", 1, 0, "AutopilotMapLayer");

    Autopilot autopilot;
    engine.rootContext()->setContextProperty("autopilot", &autopilot);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));



    return app.exec();
}
