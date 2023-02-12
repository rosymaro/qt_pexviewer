#include "mainwindow.h"

#include <QApplication>
#include <QVulkanInstance>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QVulkanInstance inst;

#ifndef Q_OS_ANDROID
    inst.setLayers(QByteArrayList() << "VK_LAYER_LUNARG_standard_validation");
#else
    inst.setLayers(QByteArrayList()
                   << "VK_LAYER_GOOGLE_threading"
                   << "VK_LAYER_LUNARG_parameter_validation"
                   << "VK_LAYER_LUNARG_object_tracker"
                   << "VK_LAYER_LUNARG_core_validation"
                   << "VK_LAYER_LUNARG_image"
                   << "VK_LAYER_LUNARG_swapchain"
                   << "VK_LAYER_GOOGLE_unique_objects");
#endif

    if (!inst.create())
        qFatal("Failed to create Vulkan instance: %d", inst.errorCode());

    VulkanWindow *vulkanWindow = new VulkanWindow;
    vulkanWindow->setVulkanInstance(&inst);

    QSize mainWindowSize = {1024,768};

    MainWindow mainWindow(vulkanWindow);
    QObject::connect(vulkanWindow, &VulkanWindow::signalInfoText, &mainWindow, &MainWindow::slotInfoText);

    mainWindow.winSize = &mainWindowSize;
    qDebug() << "main :: winSize " << mainWindow.winSize;
    qDebug() << "main :: winSize " << *mainWindow.winSize;


    mainWindow.show();
    mainWindow.resize(mainWindowSize);
    qDebug() << "main :: geometry : " << mainWindow.geometry();
    qDebug() << "main :: mainWindow : " << &mainWindowSize;
    qDebug() << "main :: mainWindow : " << mainWindowSize;


    return app.exec();
}
