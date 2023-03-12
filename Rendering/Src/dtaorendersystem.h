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


class LayoutModel;

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

    float getRenderScale() {return this->trans_info.scale;}
    LayoutModel* getLayoutModel();
    bool isRenderModelLoaded(){return this->render_object_created;}

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


    struct TRANS_INFORMATION{
       float trans_x;
       float trans_y;
       float trans_z;
       float scale;
    };
    TRANS_INFORMATION trans_info;
    std::shared_ptr<LayoutModel> layout_model = {nullptr};

public:
    void createNewObject(MODEL_TYPE model_type, const std::string & file_path);

private:
    void createNewLayoutObject(const std::string & file_path);
    void createNewPEXCapObject(const std::string & file_path);
    void createNewPEXResObject(const std::string & file_path);
    void createNewAxisObject(const std::string & file_path);


private:
    void createLveDevice();
    void deleteLveDevice();
    void createSimpleRenderSystem();
    void deleteSimpleRenderSystem();
    void beginRenderPass(VkCommandBuffer command_buffer);

};

#endif // DTAORENDERSYSTEM_H
