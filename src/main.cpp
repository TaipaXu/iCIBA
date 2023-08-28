#include <QCoreApplication>
#include <csignal>
#include "core.hpp"
#include "./config.hpp"

void signalHandler(int signal)
{
    qApp->quit();
}

int main(int argc, char *argv[])
{
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    QCoreApplication app(argc, argv);

    app.setOrganizationDomain(ORGANIZATION_DOMAIN);
    app.setApplicationName(PROJECT_NAME);
    app.setApplicationVersion(PROJECT_VERSION);

    Core core;
    QObject::connect(&core, &Core::done, qApp, &QCoreApplication::quit);
    core.start();

    return app.exec();
}
