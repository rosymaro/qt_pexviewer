

#include <QVulkanWindow>
#include <QVulkanInstance>
#include <vulkan/vulkan.h>

#include <QFileDialog>

#include "dtaorendersystem.h"

#include "lve_model.hpp"
#include "LayoutModel.h"
#include "PEXResistorModel.h"
#include "PEXCapacitorModel.h"


DtaoRenderSystem::DtaoRenderSystem(LveWindow *w)
    : lveWindow(w)
{
    this->trans_info = {};
    qDebug() << "\n$$$$$ DtaoRenderSystem()";
}

DtaoRenderSystem::~DtaoRenderSystem()
{
    deleteSimpleRenderSystem();
    deleteLveDevice();
}

void DtaoRenderSystem::beginRenderPass(VkCommandBuffer command_buffer){
    const QSize sz = this->lveWindow->swapChainImageSize();

    VkClearColorValue clearColor = {{ 0, 0, 0, 1 }};
    VkClearDepthStencilValue clearDS = { 1, 0 };
    VkClearValue clearValues[3];
    memset(clearValues, 0, sizeof(clearValues));
    clearValues[0].color = clearValues[2].color = clearColor;
    clearValues[1].depthStencil = clearDS;

    VkRenderPassBeginInfo rpBeginInfo;
    memset(&rpBeginInfo, 0, sizeof(rpBeginInfo));
    rpBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rpBeginInfo.renderPass = lveWindow->defaultRenderPass();
    rpBeginInfo.framebuffer = lveWindow->currentFramebuffer();
    rpBeginInfo.renderArea.extent.width = sz.width();
    rpBeginInfo.renderArea.extent.height = sz.height();
    rpBeginInfo.clearValueCount = lveWindow->sampleCountFlagBits() > VK_SAMPLE_COUNT_1_BIT ? 3 : 2;
    rpBeginInfo.pClearValues = clearValues;
    m_devFuncs->vkCmdBeginRenderPass(command_buffer, &rpBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport;
    viewport.x = viewport.y = 0;
    viewport.width = sz.width();
    viewport.height = sz.height();
    viewport.minDepth = 0;
    viewport.maxDepth = 1;
    m_devFuncs->vkCmdSetViewport(command_buffer, 0, 1, &viewport);

    VkRect2D scissor;
    scissor.offset.x = scissor.offset.y = 0;
    scissor.extent.width = viewport.width;
    scissor.extent.height = viewport.height;
    m_devFuncs->vkCmdSetScissor(command_buffer, 0, 1, &scissor);
}

void DtaoRenderSystem::startNextFrame(){
    this->cameraController.moveCamera(
                1.0, this->camera, this->getRenderScale(), this->gameObjects);
    this->cameraController.moveCameraMouse(
                this->camera, this->getRenderScale(), this->gameObjects);

    VkCommandBuffer command_buffer = this->lveWindow->currentCommandBuffer();
    beginRenderPass(command_buffer);

    this->simpleRenderSystem->renderGameObjects(
                command_buffer, this->gameObjects, this->camera);

    m_devFuncs->vkCmdEndRenderPass(command_buffer);

    lveWindow->frameReady();
    lveWindow->requestUpdate(); // render continuously, throttled by the presentation rate
}

void DtaoRenderSystem::initResources() {
    qDebug() << "\n$$$$$ DtaoRenderSystem::initResources()";
    createLveDevice();
    createSimpleRenderSystem();
    //if(!this->render_object_created) loadGameObjects();

    this->camera.setViewTarget(
                glm::vec3(1.0f,1.0f,1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f));

    VkDevice dev = this->lveWindow->device();
    m_devFuncs = this->lveWindow->vulkanInstance()->deviceFunctions(dev);

    qDebug() << "\tDevice = " << this->lveDevice->device();

}

void DtaoRenderSystem::releaseResources() {
    qDebug() << "\n$$$$$ DtaoRenderSystem::releaseResources()";

    deleteSimpleRenderSystem();
    deleteLveDevice();
}

void DtaoRenderSystem::initSwapChainResources() {
    qDebug() << "\n$$$$$ DtaoRenderSystem::initSwapChainResources()";
    const QSize sz = this->lveWindow->swapChainImageSize();
    float aspect = sz.width()/(double)sz.height();
    this->camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);
}


void DtaoRenderSystem::releaseSwapChainResources() {
    qDebug() << "\n$$$$$ DtaoRenderSystem::releaseSwapChainResources()";
}

void DtaoRenderSystem::createLveDevice(){
    qDebug() << "\n$$$$$ DtaoRenderSystem::createLveDevice()";
    this->lveDevice = new LveDevice(this->lveWindow, this->lveWindow->vulkanInstance());
}

void DtaoRenderSystem::deleteLveDevice(){
    qDebug() << "\n$$$$$ DtaoRenderSystem::deleteLveDevice()";
    delete this->lveDevice;
}

void DtaoRenderSystem::createSimpleRenderSystem(){
    qDebug() << "\n$$$$$ DtaoRenderSystem::createSimpleRenderSystem()";
    VkRenderPass renderpass = this->lveWindow->defaultRenderPass();
    this->simpleRenderSystem = new SimpleRenderSystem(*this->lveDevice, renderpass);
}

void DtaoRenderSystem::deleteSimpleRenderSystem(){
    qDebug() << "\n$$$$$ DtaoRenderSystem::deleteSimpleRenderSystem()";
    delete this->simpleRenderSystem;
}

void DtaoRenderSystem::createNewObject(MODEL_TYPE model_type, const std::string & file_path) {
    if( model_type == MODEL_TYPE::MODEL_TYPE_LAYOUT) createNewLayoutObject(file_path);
    else if( model_type == MODEL_TYPE::MODEL_TYPE_PEX_CAPACITOR) createNewPEXCapObject(file_path);
    else if( model_type == MODEL_TYPE::MODEL_TYPE_PEX_RESISTOR) createNewPEXResObject(file_path);
    else if( model_type == MODEL_TYPE::MODEL_TYPE_AXIS) createNewAxisObject(file_path);
}

void DtaoRenderSystem::createNewLayoutObject(const std::string & file_path){

    //Layout model
    std::string layout_info_file_path = file_path;
    std::shared_ptr<LayoutModel> model
            = std::make_unique<LayoutModel>(
                *this->lveDevice, MODEL_TYPE_LAYOUT, layout_info_file_path);
    model->opacity = 0.5f;

    LayoutDataManager* layout_data = model->getLayoutDataManager();
    this->trans_info.trans_x = static_cast<float>( layout_data->getMinX());
    this->trans_info.trans_y = static_cast<float>( layout_data->getMinY());
    this->trans_info.trans_z = static_cast<float>( layout_data->getMinZ());
    this->trans_info.scale = static_cast<float>( layout_data->getScale());

    //auto new_object = DTAOObject::createObject();
    auto new_object = LveGameObject::createGameObject();
    new_object.model = model;
    new_object.transform.translation = {
        -this->trans_info.trans_x, -this->trans_info.trans_y, -this->trans_info.trans_z};
    new_object.transform.scale = {
        this->trans_info.scale, this->trans_info.scale, this->trans_info.scale };

    //this->dtao_objects.push_back(std::move(new_object));
    this->gameObjects.push_back(std::move(new_object));
    this->render_object_created = true;
    this->layout_model = model;
}

void DtaoRenderSystem::createNewPEXResObject( const std::string & file_path){
    //PEX Resistor model
    std::string res_info_file_path = file_path;
    std::shared_ptr<PEXResistorModel> model
            = std::make_unique<PEXResistorModel>(
                *this->lveDevice, MODEL_TYPE_PEX_RESISTOR, res_info_file_path);

    //Resistor Object
    //auto new_object = DTAOObject::createObject();
    auto new_object = LveGameObject::createGameObject();
    new_object.model = model;
    new_object.transform.translation = {
        -this->trans_info.trans_x, -this->trans_info.trans_y, -this->trans_info.trans_z};
    new_object.transform.scale = {
        this->trans_info.scale, this->trans_info.scale, this->trans_info.scale };

    //this->dtao_objects.push_back(std::move(new_object));
    this->gameObjects.push_back(std::move(new_object));
}

void DtaoRenderSystem::createNewPEXCapObject( const std::string & file_path){
    //PEX Capacitor model
    std::string cap_info_file_path = file_path;
    std::shared_ptr<PEXCapacitorModel> model
            = std::make_unique<PEXCapacitorModel>(
                *this->lveDevice, MODEL_TYPE_PEX_CAPACITOR,
                cap_info_file_path, this->layout_model->getLayoutDataManager());

    //Capacitor Object
    //auto new_object = DTAOObject::createObject();
    auto new_object = LveGameObject::createGameObject();
    new_object.model = model;
    new_object.transform.translation = {
        -this->trans_info.trans_x, -this->trans_info.trans_y, -this->trans_info.trans_z};
    new_object.transform.scale = {
        this->trans_info.scale, this->trans_info.scale, this->trans_info.scale };

    //this->dtao_objects.push_back(std::move(new_object));
    this->gameObjects.push_back(std::move(new_object));
}

void DtaoRenderSystem::createNewAxisObject(const std::string & file_path){
    (void)(file_path);
    //Axis model
    std::shared_ptr<LveModel> model
            = std::make_unique<LveModel>(*this->lveDevice, MODEL_TYPE_AXIS);

    //auto new_object = DTAOObject::createObject();
    auto new_object = LveGameObject::createGameObject();
    new_object.model = model;
    new_object.transform.translation = {0.0f, 0.0f, 0.0f};
    new_object.transform.scale = { 1.0f, 1.0f, 1.0f };

    //this->dtao_objects.push_back(std::move(new_object));
    this->gameObjects.push_back(std::move(new_object));
}

/*
void DtaoRenderSystem::loadGameObjects() {
    qDebug() << "\n$$$$$ DtaoRenderSystem::loadGameObjects()";
    return;

    auto cube = LveGameObject::createGameObject();

    //Layout model
    std::string layout_info_file_path = "Rendering/Data/layout_input_data.csv";
    //std::string layout_info_file_path = "Rendering/Data/big_test_espin.csv";
    std::shared_ptr<LayoutModel> layout_model
            = std::make_unique<LayoutModel>(*this->lveDevice, MODEL_TYPE_LAYOUT, layout_info_file_path);
    layout_model->opacity = 0.5f;

    LayoutDataManager* layout_data = layout_model->getLayoutDataManager();
    float trans_x = static_cast<float>(layout_data->getMinX());
    float trans_y = static_cast<float>(layout_data->getMinY());
    float trans_z = static_cast<float>(layout_data->getMinZ());
    float scale = static_cast<float>(layout_data->getScale());

    this->render_scale = scale;

    //Axis model
    std::shared_ptr<LveModel> axis_model
            = std::make_unique<LveModel>(*this->lveDevice, MODEL_TYPE_AXIS);

    //PEX Resistor model
    std::string res_info_file_path = "Rendering/Data/MVP_PEX_RC_r1_0130_0208version.csv";
    std::shared_ptr<PEXResistorModel> res_model
            = std::make_unique<PEXResistorModel>(*this->lveDevice, MODEL_TYPE_PEX_RESISTOR, res_info_file_path);

    //PEX Capacitor model
    std::string cap_info_file_path = "Rendering/Data/MVP_PEX_RC_r1_0130_0208version.csv";
    std::shared_ptr<PEXCapacitorModel> cap_model
            = std::make_unique<PEXCapacitorModel>(*this->lveDevice, MODEL_TYPE_PEX_CAPACITOR, cap_info_file_path, layout_model->getLayoutDataManager());


    //Resistor Object
    cube = LveGameObject::createGameObject();
    cube.model = res_model;
    cube.transform.translation = { -trans_x, -trans_y, -trans_z };
    cube.transform.scale = { scale, scale, scale };
    gameObjects.push_back(std::move(cube));

    //Capacitor Object
    cube = LveGameObject::createGameObject();
    cube.model = cap_model;
    cube.transform.translation = { -trans_x, -trans_y, -trans_z };
    cube.transform.scale = { scale, scale, scale };
    gameObjects.push_back(std::move(cube));

    //Layout Object
    cube = LveGameObject::createGameObject();
    cube.model = layout_model;
    cube.transform.translation = { -trans_x, -trans_y, -trans_z };
    cube.transform.scale = { scale, scale, scale };
    gameObjects.push_back(std::move(cube));

    //Axis Object
    cube = LveGameObject::createGameObject();
    cube.model = axis_model;
    cube.transform.translation = { 0.0f, 0.0f, 0.0f };
    cube.transform.scale = { 1.0f, 1.0f, 1.0f };
    gameObjects.push_back(std::move(cube));

    this->render_object_created = true;
}
*/
