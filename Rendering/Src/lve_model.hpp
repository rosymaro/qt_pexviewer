#pragma once

#include "lve_device.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <vector>

#include <string>
#include <iostream>

#include <map>
//#include <ranges>
#include "LayoutPEXData.h"

typedef enum MODEL_TYPE {
    MODEL_TYPE_LAYOUT = 0,
    MODEL_TYPE_AXIS,
    MODEL_TYPE_PEX_RESISTOR,
    MODEL_TYPE_PEX_CAPACITOR,
    MODEL_TYPE_DEFAULT
} MODEL_TYPE;




class LveModel {
public:
    struct Vertex {
        glm::vec3 position{};
        glm::vec3 color{};
        glm::vec3 normal{};

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };

    struct LayerProperty {
        bool visiblity;
        glm::vec3 color{};
        float opacity;
    };

    LveModel(LveDevice& device, MODEL_TYPE model_type);
    ~LveModel();

    LveModel() = delete;
    LveModel(const LveModel&) = delete;
    LveModel& operator=(const LveModel&) = delete;

    //void bind(VkCommandBuffer commandBuffer);
    void bindVertexBuffer(VkCommandBuffer commandBuffer);
    void bindVertexBuffer_map(VkCommandBuffer commandBuffer);
    void bindIndexBufferForFace(VkCommandBuffer commandBuffer);
    void bindIndexBufferForFace_map(VkCommandBuffer commandBuffer);
    void bindDrawVertexIndexBufferForFace_map(VkCommandBuffer commandBuffer);
    void bindDrawVertexIndexBufferForFace_layer(VkCommandBuffer commandBuffer, const float layer);
    void bindDrawIndexBufferForEdge_layer(VkCommandBuffer commandBuffer, const float layer);
    void bindIndexBufferForEdge(VkCommandBuffer commandBuffer);
    void drawForFace(VkCommandBuffer commandBuffer);
    void drawForEdge(VkCommandBuffer commandBuffer);


public:
    void createBuffers();
    void destroyBuffers();
    void createVertexBuffers(const std::vector<Vertex>& vertices, VkBuffer& buffer, VkDeviceMemory& memory);
    void createIndexBuffers(const std::vector<uint32_t>& indices, VkBuffer & buffer, VkDeviceMemory & memory);
    LveDevice& lveDevice;
    VkBuffer vertexBuffer{};
    VkDeviceMemory vertexBufferMemory{};
    std::map<float, VkBuffer> layerbyVertexBuffers;
    std::map<float, VkDeviceMemory> layerbyVertexBuffersMemory;
    uint32_t vertexCount{};

    VkBuffer indexBufferForFace{};
    VkDeviceMemory indexBufferMemoryForFace{};
    VkBuffer indexBufferForEdge{};
    VkDeviceMemory indexBufferMemoryForEdge{};

    std::map<float, VkBuffer> layerbyFaceIndexBuffers;
    std::map<float, VkDeviceMemory> layerbyFaceIndexBuffersMemory;
    std::map<float, VkBuffer> layerbyEdgeIndexBuffers;
    std::map<float, VkDeviceMemory> layerbyEdgeIndexBuffersMemory;

    MODEL_TYPE model_type{};

public:
    //std::string layout_info_file;
    std::vector<cube_info> cube_infos{};
    std::vector<cube_vertex> cube_vertices{};

    std::vector<Vertex> vertices{};
    std::vector<uint32_t> indices_face{};
    std::vector<uint32_t> indices_edge{};
    std::vector<float> drawing_order_layerby{};

    std::map<float, std::vector<Vertex>> layerby_vertices;
    std::map<float, std::vector<uint32_t>> layerby_face;
    std::map<float, std::vector<uint32_t>> layerby_edge;
    std::map<float, LayerProperty> layerList;
    std::map<float, glm::vec3> init_layerby_color;
    void changeLayerColor(float layernumber, glm::vec3 rgb);
    void changeLayerOpacity(float layernumber, float opacity);
    void changeLayerVisiblity(float layernumber, bool visiblity);
    bool visible{true};
    float opacity{ 1.0f };

public:
    MODEL_TYPE getModelType() { return this->model_type; }

public:
    //void setLayoutInfoFile(const std::string file_path) { this->layout_info_file = file_path; }
    virtual void makeRenderingData(const std::string& file_path = "") {(void)file_path;};
    virtual void loadData(const std::string& file_path = "") {(void)file_path;};
    virtual void makeVertices() {};
    virtual void makeIndices() {};

    void makeAxisData(const float axis_length = (2.0f));
    void setVisible(bool flag) { this->visible = flag; }
    bool getVisible() { return this->visible; }
    void toggleVisible() { this->visible = !this->visible; }
    void updateOpacity(float amount);
    float getOpacity() { return this->opacity; }
    std::map<float, LayerProperty> getLayer();
};

