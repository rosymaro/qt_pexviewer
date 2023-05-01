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
#include "T2D.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "PEXResistorModel.h"

#include "ToyCAD/Src/toycad_object.h"

#include <iostream>
#include <vector>


enum DTAO_RENDER_MODE{
    DTAO_RENDER_MODE_EMPTY = 0,
    DTAO_RENDER_MODE_LAYOUT,
    DTAO_RENDER_MODE_PEX_ALL,
    DTAO_RENDER_MODE_PEX_RES,
    DTAO_RENDER_MODE_PEX_CAP,
    DTAO_RENDER_MODE_TOYCAD,
    DTAO_RENDER_MODE_DEFAULT
};

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
    void translateLayerPosition(POS_MONITORING value);
    void emitMonitor();

    void startNextFrame() override;

    float getRenderScale() {return this->trans_info.scale;}
    LayoutModel* getLayoutModel();
    bool isRenderModelLoaded(){return this->render_object_created;}
    std::vector<LveGameObject> &getGameObjects(){return this->gameObjects;}

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
       float minx;
       float miny;
    };
    TRANS_INFORMATION trans_info;
    std::shared_ptr<LayoutModel> layout_model = {nullptr};
    std::shared_ptr<PEXResistorModel> res_model = {nullptr};


public:
    void createNewObject(MODEL_TYPE model_type, const std::string & file_path);

    std::shared_ptr<PEXResistorModel> &getResistorModel() { return this->res_model;};
    void createT2DObject(MODEL_TYPE model_type, T2D t2d);
    void getCustomColor(string layernumber, glm::vec3 rgb);
    void getCustomOpacity(string layernumber, float opacity);
    void getCustomVisiblity(string layernumber, bool visibility);
    void createNewPEXCapObject(const QString & file_path, T2D t2d);
    void createNewPEXResObject(const QString & file_path);


private:
    void createNewLayoutObject(const std::string & file_path);
    void createT2DLayoutObject(T2D & t2d);
    void createNewAxisObject(const QString & file_path);


private:
    void createLveDevice();
    void deleteLveDevice();
    void createSimpleRenderSystem();
    void deleteSimpleRenderSystem();
    void beginRenderPass(VkCommandBuffer command_buffer);
    float aspect;

private:
    std::vector<ToyCADObject> toycad_objects{};

public:
    std::vector<ToyCADObject>& getToyCADObjects(){return this->toycad_objects;}
    void createToyCADObjects(const std::string& file_path);
    int render_mode{DTAO_RENDER_MODE_EMPTY};


};

#endif // DTAORENDERSYSTEM_H
