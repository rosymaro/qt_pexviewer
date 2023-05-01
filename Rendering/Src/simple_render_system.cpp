#include <vulkan/vulkan.h>
#include "simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <stdexcept>



struct SimplePushConstantData {
    glm::mat4 transform{ 1.f };
    alignas(16) glm::vec3 color{};
    glm::float32 alpha;
};

SimpleRenderSystem::SimpleRenderSystem(LveDevice& device, VkRenderPass renderPass)
    : lveDevice{ device } {
    createPipelineLayout(this->pipelineLayoutForLayoutFace);
    createPipelineForFace(renderPass);
    createPipelineLayout(this->pipelineLayoutForLayoutEdge);
    createPipelineForEdge(renderPass);
    createPipelineLayout(this->pipelineLayoutForPEXResistor);
    createPipelineForPEXResistor(renderPass);
    createPipelineLayout(this->pipelineLayoutForPEXResistorEdge);
    createPipelineForPEXResistorEdge(renderPass);
    createPipelineLayout(this->pipelineLayoutForPEXCapacitor);
    createPipelineForPEXCapacitor(renderPass);

    createPipelineLayout(this->pipelineLayoutForToyCADFace);
    createPipelineForToyCADFace(renderPass);
    createPipelineLayout(this->pipelineLayoutForToyCADEdge);
    createPipelineForToyCADEdge(renderPass);
}

SimpleRenderSystem::~SimpleRenderSystem() {
    vkDestroyPipelineLayout(lveDevice.device(), pipelineLayoutForLayoutFace, nullptr);
    vkDestroyPipelineLayout(lveDevice.device(), pipelineLayoutForLayoutEdge, nullptr);
    vkDestroyPipelineLayout(lveDevice.device(), pipelineLayoutForPEXResistor, nullptr);
    vkDestroyPipelineLayout(lveDevice.device(), pipelineLayoutForPEXResistorEdge, nullptr);
    vkDestroyPipelineLayout(lveDevice.device(), pipelineLayoutForPEXCapacitor, nullptr);

    vkDestroyPipelineLayout(lveDevice.device(), pipelineLayoutForToyCADFace, nullptr);
    vkDestroyPipelineLayout(lveDevice.device(), pipelineLayoutForToyCADEdge, nullptr);
}


void SimpleRenderSystem::createPipelineLayout(VkPipelineLayout& pipeline_layout) {
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipeline_layout) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void SimpleRenderSystem::createPipelineForFace(VkRenderPass renderPass) {
    assert(pipelineLayoutForLayoutFace != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    LvePipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayoutForLayoutFace;

    pipelineConfig.colorBlendAttachment.blendEnable = VK_TRUE;
    pipelineConfig.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    pipelineConfig.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;

    this->lvePipelineForLayoutFace = std::make_unique<LvePipeline>(
        lveDevice,
        "Rendering/shaders/simple_shader.vert.spv",
        "Rendering/shaders/simple_shader.frag.spv",
        pipelineConfig, LveModel::Vertex::getBindingDescriptions(), LveModel::Vertex::getAttributeDescriptions());
}

void SimpleRenderSystem::createPipelineForEdge(VkRenderPass renderPass) {
    assert(pipelineLayoutForLayoutEdge != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    LvePipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayoutForLayoutEdge;

    pipelineConfig.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;

    this->lvePipelineForLayoutEdge = std::make_unique<LvePipeline>(
        lveDevice,
        "Rendering/shaders/simple_shader.vert_edge.spv",
        "Rendering/shaders/simple_shader.frag_edge.spv",
        pipelineConfig, LveModel::Vertex::getBindingDescriptions(), LveModel::Vertex::getAttributeDescriptions());
}

void SimpleRenderSystem::createPipelineForPEXResistor(VkRenderPass renderPass) {
    assert(pipelineLayoutForPEXResistor != nullptr && "Cannot create pipeline before pipeline layout");
    PipelineConfigInfo pipelineConfigResistor{};
    LvePipeline::defaultPipelineConfigInfo(pipelineConfigResistor);
    pipelineConfigResistor.renderPass = renderPass;
    pipelineConfigResistor.pipelineLayout = pipelineLayoutForPEXResistor;
    pipelineConfigResistor.colorBlendAttachment.blendEnable = VK_TRUE;

    pipelineConfigResistor.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    pipelineConfigResistor.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;

    this->lvePipelineForPEXResistor = std::make_unique<LvePipeline>(
        lveDevice,
        "Rendering/shaders/simple_shader.vert_pex.spv",
        "Rendering/shaders/simple_shader.frag_pex.spv",
        pipelineConfigResistor, LveModel::VertexResistor::getBindingDescriptions(), LveModel::VertexResistor::getAttributeDescriptions());
}


void SimpleRenderSystem::createPipelineForPEXResistorEdge(VkRenderPass renderPass) {
    assert(pipelineLayoutForPEXResistorEdge != nullptr && "Cannot create pipeline before pipeline layout");
    PipelineConfigInfo pipelineConfigResistorEdge{};
    LvePipeline::defaultPipelineConfigInfo(pipelineConfigResistorEdge);
    pipelineConfigResistorEdge.renderPass = renderPass;
    pipelineConfigResistorEdge.pipelineLayout = pipelineLayoutForPEXResistorEdge;

    pipelineConfigResistorEdge.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;

    this->lvePipelineForPEXResistorEdge = std::make_unique<LvePipeline>(
        lveDevice,
        "Rendering/shaders/simple_shader.vert_pex.spv",
        "Rendering/shaders/simple_shader.frag_pex.spv",
        pipelineConfigResistorEdge, LveModel::VertexResistor::getBindingDescriptions(), LveModel::VertexResistor::getAttributeDescriptions());
}

void SimpleRenderSystem::createPipelineForPEXCapacitorLine(VkRenderPass renderPass) {
    assert(pipelineLayoutForPEXResistor != nullptr && "Cannot create pipeline before pipeline layout");
    PipelineConfigInfo pipelineConfigCapacitorLine{};
    LvePipeline::defaultPipelineConfigInfo(pipelineConfigCapacitorLine);
    pipelineConfigCapacitorLine.renderPass = renderPass;
    pipelineConfigCapacitorLine.pipelineLayout = pipelineLayoutForPEXCapacitorLine;
    pipelineConfigCapacitorLine.colorBlendAttachment.blendEnable = VK_TRUE;

    pipelineConfigCapacitorLine.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    pipelineConfigCapacitorLine.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;

    this->lvePipelineForPEXCapacitorLine = std::make_unique<LvePipeline>(
        lveDevice,
        "Rendering/shaders/simple_shader.vert_pex.spv",
        "Rendering/shaders/simple_shader.frag_pex.spv",
        pipelineConfigCapacitorLine, LveModel::Vertex::getBindingDescriptions(), LveModel::Vertex::getAttributeDescriptions());
}

void SimpleRenderSystem::createPipelineForPEXCapacitor(VkRenderPass renderPass) {
    assert(pipelineLayoutForPEXCapacitor != nullptr && "Cannot create pipeline before pipeline layout");
    PipelineConfigInfo pipelineConfigCapacitor{};
    LvePipeline::defaultPipelineConfigInfo(pipelineConfigCapacitor);
    pipelineConfigCapacitor.renderPass = renderPass;
    pipelineConfigCapacitor.pipelineLayout = pipelineLayoutForPEXCapacitor;

    pipelineConfigCapacitor.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;

    this->lvePipelineForPEXCapacitor = std::make_unique<LvePipeline>(
        lveDevice,
        "Rendering/shaders/simple_shader.vert_pex.spv",
        "Rendering/shaders/simple_shader.frag_pex.spv",
        pipelineConfigCapacitor, LveModel::Vertex::getBindingDescriptions(), LveModel::Vertex::getAttributeDescriptions());
}

void SimpleRenderSystem::renderGameObjects(
    VkCommandBuffer commandBuffer,
    std::vector<LveGameObject>& gameObjects,
    const LveCamera& camera) {

    auto proj = camera.getProjection();
    proj[1][1] *= -1;
    auto projectionView = proj * camera.getView();

    SimplePushConstantData push{};
    for (auto& obj : gameObjects) {
        push.transform = projectionView * obj.transform.mat4();

        if (obj.model->getModelType() == MODEL_TYPE::MODEL_TYPE_LAYOUT && obj.model->getVisible()) {
            std::map<string, LveModel::LayerProperty> layer_info = obj.model->getLayer();

            for (auto ordered_key : obj.model->drawing_order_layerby) {
                if (layer_info[ordered_key].visiblity) {
                    lvePipelineForLayoutFace->bind(commandBuffer);
                    push.alpha = layer_info[ordered_key].opacity;
                    push.color = layer_info[ordered_key].color;
                    vkCmdPushConstants(
                        commandBuffer, pipelineLayoutForLayoutFace,
                        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
                        sizeof(SimplePushConstantData), &push);
                    obj.model->bindDrawVertexIndexBufferForFace_layer(commandBuffer, ordered_key);
                    lvePipelineForLayoutEdge->bind(commandBuffer);
                    obj.model->bindDrawIndexBufferForEdge_layer(commandBuffer, ordered_key);
                }
            }

        }

        if (obj.model->getModelType() == MODEL_TYPE::MODEL_TYPE_AXIS && obj.model->getVisible()) {
            lvePipelineForLayoutEdge->bind(commandBuffer);
            push.color = glm::vec3(1.0f, 1.0f, 1.0f);
            vkCmdPushConstants(
                commandBuffer, pipelineLayoutForLayoutEdge,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
                sizeof(SimplePushConstantData), &push);
            obj.model->bindVertexBuffer(commandBuffer);
            obj.model->bindIndexBufferForEdge(commandBuffer);
            obj.model->drawForEdge(commandBuffer);
        }

        if (obj.model->getModelType() == MODEL_TYPE::MODEL_TYPE_PEX_RESISTOR && obj.model->getVisible()) {
            //*

            push.alpha = 0.1f;
            push.color = glm::vec3(1.0f, 1.0f, 1.0f);
            lvePipelineForPEXResistor->bind(commandBuffer);
            vkCmdPushConstants(
                commandBuffer, pipelineLayoutForPEXResistor,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
                sizeof(SimplePushConstantData), &push);
            obj.model->bindVertexBuffer(commandBuffer);
            obj.model->bindIndexBufferForFace(commandBuffer);
            obj.model->drawForFace(commandBuffer);
            obj.model->bindIndexBufferForEdge(commandBuffer);
            lvePipelineForPEXResistorEdge->bind(commandBuffer);
            vkCmdPushConstants(
                commandBuffer, pipelineLayoutForPEXResistorEdge,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
                sizeof(SimplePushConstantData), &push);


            obj.model->drawForEdge(commandBuffer);

        }

        if (obj.model->getModelType() == MODEL_TYPE::MODEL_TYPE_PEX_CAPACITOR && obj.model->getVisible()) {
            //*
            lvePipelineForPEXCapacitor->bind(commandBuffer);
            vkCmdPushConstants(
                commandBuffer, pipelineLayoutForPEXCapacitor,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
                sizeof(SimplePushConstantData), &push);
            obj.model->bindVertexBuffer(commandBuffer);
            obj.model->bindIndexBufferForEdge(commandBuffer);
            obj.model->drawForEdge(commandBuffer);
            //*/
        }
    }
}

void SimpleRenderSystem::createPipelineForToyCADFace(VkRenderPass renderPass) {
    assert(pipelineLayoutForToyCADFace != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    LvePipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayoutForToyCADFace;

    pipelineConfig.colorBlendAttachment.blendEnable = VK_TRUE;
    pipelineConfig.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    pipelineConfig.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;

    this->lvePipelineForToyCADFace = std::make_unique<LvePipeline>(
                lveDevice,
                "ToyCAD/shaders/toycad.vert.spv",
                "ToyCAD/shaders/toycad.frag.spv",
                pipelineConfig,
                ToyCADModel::ToyCADVertex::getBindingDescriptions(),
                ToyCADModel::ToyCADVertex::getAttributeDescriptions());
}

void SimpleRenderSystem::createPipelineForToyCADEdge(VkRenderPass renderPass) {
    assert(pipelineLayoutForToyCADEdge != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    LvePipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayoutForToyCADEdge;

    pipelineConfig.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;

    this->lvePipelineForToyCADEdge = std::make_unique<LvePipeline>(
                lveDevice,
                "ToyCAD/shaders/toycad.vert.spv",
                "ToyCAD/shaders/toycad.frag.spv",
                pipelineConfig,
                ToyCADModel::ToyCADVertex::getBindingDescriptions(),
                ToyCADModel::ToyCADVertex::getAttributeDescriptions());
}

void SimpleRenderSystem::renderToyCADObjects(
    VkCommandBuffer commandBuffer,
    std::vector<ToyCADObject>& objects,
    const LveCamera& camera) {

    auto proj = camera.getProjection();
    proj[1][1] *= -1;
    auto projectionView = proj * camera.getView();

    SimplePushConstantData push{};
    for (auto& obj : objects) {
        if(obj.getVisibility()){
            push.transform = projectionView * obj.transform.mat4();
            push.alpha = obj.opacity;
            push.color = obj.color;

            lvePipelineForToyCADFace->bind(commandBuffer);
            vkCmdPushConstants(
                commandBuffer, pipelineLayoutForToyCADFace,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
                sizeof(SimplePushConstantData), &push);
            obj.model->bindVertexBuffer(commandBuffer);
            obj.model->bindIndexBufferForFace(commandBuffer);
            obj.model->drawForFace(commandBuffer);

            push.color = glm::vec3{obj.color.r*0.8, obj.color.g*0.8, obj.color.b*0.8,};

            lvePipelineForToyCADEdge->bind(commandBuffer);
            vkCmdPushConstants(
                commandBuffer, pipelineLayoutForToyCADEdge,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
                sizeof(SimplePushConstantData), &push);
            obj.model->bindIndexBufferForEdge(commandBuffer);
            obj.model->drawForEdge(commandBuffer);
        }//if(obj.getVisibility())
    }//for (auto& obj : objects)
}//void SimpleRenderSystem::renderToyCADObjects
