#pragma once
#include <vulkan/vulkan.h>

#include "lve_camera.hpp"
#include "lve_device.hpp"
#include "lve_game_object.hpp"
#include "lve_pipeline.hpp"

#include "ToyCAD/Src/toycad_object.h"

// std
#include <memory>
#include <vector>


class SimpleRenderSystem {


public:

    SimpleRenderSystem(LveDevice& device, VkRenderPass renderPass);
    ~SimpleRenderSystem();

    SimpleRenderSystem(const SimpleRenderSystem&) = delete;
    SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

    void renderGameObjects(
            VkCommandBuffer commandBuffer,
            std::vector<LveGameObject>& gameObjects,
            const LveCamera& camera);

    void renderToyCADObjects(
            VkCommandBuffer commandBuffer,
            std::vector<ToyCADObject>& objects,
            const LveCamera& camera);

private:
    void createPipelineLayout(VkPipelineLayout & pipeline_layout);
    void createPipelineForFace(VkRenderPass renderPass);
    void createPipelineForEdge(VkRenderPass renderPass);
    void createPipelineForPEXResistor(VkRenderPass renderPass);
    void createPipelineForPEXResistorEdge(VkRenderPass renderPass);
    void createPipelineForPEXCapacitorLine(VkRenderPass renderPass);
    void createPipelineForPEXCapacitor(VkRenderPass renderPass);

    void createPipelineForToyCADFace(VkRenderPass renderPass);
    void createPipelineForToyCADEdge(VkRenderPass renderPass);

    LveDevice& lveDevice;

    std::unique_ptr<LvePipeline> lvePipelineForLayoutFace;
    VkPipelineLayout pipelineLayoutForLayoutFace;

    std::unique_ptr<LvePipeline> lvePipelineForLayoutEdge;
    VkPipelineLayout pipelineLayoutForLayoutEdge;

    std::unique_ptr<LvePipeline> lvePipelineForPEXResistor;
    VkPipelineLayout pipelineLayoutForPEXResistor;

    std::unique_ptr<LvePipeline> lvePipelineForPEXResistorEdge;
    VkPipelineLayout pipelineLayoutForPEXResistorEdge;

    std::unique_ptr<LvePipeline> lvePipelineForPEXCapacitorLine;
    VkPipelineLayout pipelineLayoutForPEXCapacitorLine;

    std::unique_ptr<LvePipeline> lvePipelineForPEXCapacitor;
    VkPipelineLayout pipelineLayoutForPEXCapacitor;

    std::unique_ptr<LvePipeline> lvePipelineForToyCADFace;
    VkPipelineLayout pipelineLayoutForToyCADFace;
    std::unique_ptr<LvePipeline> lvePipelineForToyCADEdge;
    VkPipelineLayout pipelineLayoutForToyCADEdge;


};

