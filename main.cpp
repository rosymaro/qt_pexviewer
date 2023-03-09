#include "mainwindow.h"

#include <QApplication>
#include <QVulkanInstance>
#include <QLoggingCategory>
#include <fstream>
#include <string>
#include <iostream>
#include "formtop.h"

#include "Rendering/Src/lve_window.hpp"
#include "Rendering/Src/simple_render_system.hpp"

Q_LOGGING_CATEGORY(lcVk, "qt.vulkan")

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QLoggingCategory::setFilterRules(QStringLiteral("qt.vulkan=true"));

    QVulkanInstance inst;
    LveWindow *vulkanWindow = new LveWindow;

    inst.setLayers(QByteArrayList() << "VK_LAYER_LUNARG_standard_validation");


    if (!inst.create())
        qFatal("Failed to create Vulkan instance: %d", inst.errorCode());

    vulkanWindow->setVulkanInstance(&inst);

    MainWindow mainWindow(vulkanWindow);
    QObject::connect(vulkanWindow, &LveWindow::signalInfoText, &mainWindow, &MainWindow::slotInfoText);

    QRect size = mainWindow.geometry();
    mainWindow.shareGeo(size);
    qDebug() << "Main geo : " << mainWindow.geometry();

    mainWindow.show();





    return app.exec();
}





