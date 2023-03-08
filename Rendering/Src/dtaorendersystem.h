#ifndef DTAORENDERSYSTEM_H
#define DTAORENDERSYSTEM_H

#include <QVulkanWindow>
#include <QVulkanWindowRenderer>
#include <QVulkanDeviceFunctions>
#include <vulkan/vulkan.h>

#include "lve_window.hpp"
#include "lve_device.hpp"
#include "lve_game_object.hpp"
#include "simple_render_system.hpp"
#include "lve_camera.hpp"
#include "keyboard_movement_controller.hpp"

#include <iostream>
#include <vector>



class DtaoRenderSystem : public QVulkanWindowRenderer
{
public:
    DtaoRenderSystem(LveWindow *w);
    ~DtaoRenderSystem();

    DtaoRenderSystem(const DtaoRenderSystem &) = delete;
    DtaoRenderSystem &operator=(const DtaoRenderSystem &) = delete;


    void initResources() override;
    void initSwapChainResources() override;
    void releaseSwapChainResources() override;
    void releaseResources() override;

    void startNextFrame() override;

    float getRenderScale() {return this->render_scale;};

public:
    LveCamera camera{};
    KeyboardMovementController cameraController{};

private:
    QVulkanDeviceFunctions *m_devFuncs = nullptr;

    void loadGameObjects();

    LveWindow* lveWindow = nullptr;
    LveDevice* lveDevice = nullptr;
    SimpleRenderSystem* simpleRenderSystem = nullptr;

    std::vector<LveGameObject> gameObjects{};
    bool render_object_created = {false};

    //KeyboardMovementController cameraController;

    float render_scale{1.0f};

private:
    void createLveDevice();
    void deleteLveDevice();
    void createSimpleRenderSystem();
    void deleteSimpleRenderSystem();
    void beginRenderPass(VkCommandBuffer command_buffer);

};

#endif // DTAORENDERSYSTEM_H
