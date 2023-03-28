#include <vulkan/vulkan.h>

#include "lve_model.hpp"
#include "rapidcsv.h"

// std
#include <cassert>
#include <cstring>

using namespace std;

inline float coord_normalize(float in_value, float move, float scale) {
    //return 2 * (in_value - min_value) / (max_value - min_value) - 1.0f;
    return (2 * (in_value - move) / (scale)-1.0f);
}



LveModel::LveModel(LveDevice& device, MODEL_TYPE type)
    : lveDevice{ device }, model_type(type) {
    if (type == MODEL_TYPE::MODEL_TYPE_AXIS) {
        makeAxisData();
        createBuffers();
    }

    //createBuffers();
}

LveModel::~LveModel() {
    destroyBuffers();

    this->cube_infos.clear();
    this->cube_vertices.clear();
    this->vertices.clear();
    this->layerby_vertices.clear();
    this->indices_face.clear();
    this->layerby_face.clear();
    this->indices_edge.clear();
    this->layerby_edge.clear();
}

void LveModel::createBuffers() {

    if (this->model_type == MODEL_TYPE::MODEL_TYPE_LAYOUT) {
        for (auto const& [key, val] : layerby_vertices) {
            std::map<float, VkBuffer>& buffers = this->layerbyVertexBuffers;
            std::map<float, VkDeviceMemory>& buffersmemory = this->layerbyVertexBuffersMemory;
            createVertexBuffers(val, buffers[key], buffersmemory[key]);

            std::map<float, VkBuffer>& facebuffers = this->layerbyFaceIndexBuffers;
            std::map<float, VkDeviceMemory>& facebuffersmemory = this->layerbyFaceIndexBuffersMemory;
            createIndexBuffers(layerby_face[key], facebuffers[key], facebuffersmemory[key]);

            std::map<float, VkBuffer>& edgebuffers = this->layerbyEdgeIndexBuffers;
            std::map<float, VkDeviceMemory>& edgebuffersmemory = this->layerbyEdgeIndexBuffersMemory;
            createIndexBuffers(layerby_edge[key], edgebuffers[key], edgebuffersmemory[key]);
        }
    }
    else if (this->model_type == MODEL_TYPE::MODEL_TYPE_AXIS) {
        createVertexBuffers(vertices, vertexBuffer, vertexBufferMemory);
        createIndexBuffers(this->indices_edge, this->indexBufferForEdge, this->indexBufferMemoryForEdge);
    }
    else if (this->model_type == MODEL_TYPE::MODEL_TYPE_PEX_RESISTOR) {
        createVertexBuffers(vertices, vertexBuffer, vertexBufferMemory);
        createIndexBuffers(this->indices_face, this->indexBufferForFace, this->indexBufferMemoryForFace);
    }
    else if (this->model_type == MODEL_TYPE::MODEL_TYPE_PEX_CAPACITOR) {
        createVertexBuffers(vertices, vertexBuffer, vertexBufferMemory);
        createIndexBuffers(this->indices_edge, this->indexBufferForEdge, this->indexBufferMemoryForEdge);
    }
}

void LveModel::destroyBuffers() {
    if (this->model_type == MODEL_TYPE::MODEL_TYPE_LAYOUT) {
        for (auto const& [key, val] : layerbyFaceIndexBuffers) {
            vkDestroyBuffer(lveDevice.device(), layerbyVertexBuffers[key], nullptr);
            vkFreeMemory(lveDevice.device(), layerbyVertexBuffersMemory[key], nullptr);
            vkDestroyBuffer(lveDevice.device(), val, nullptr);
            vkFreeMemory(lveDevice.device(), layerbyFaceIndexBuffersMemory[key], nullptr);
            vkDestroyBuffer(lveDevice.device(), layerbyEdgeIndexBuffers[key], nullptr);
            vkFreeMemory(lveDevice.device(), layerbyEdgeIndexBuffersMemory[key], nullptr);
        }

        //vkDestroyBuffer(lveDevice.device(), indexBufferForEdge, nullptr);
        //vkFreeMemory(lveDevice.device(), indexBufferMemoryForEdge, nullptr);
    }
    else if (this->model_type == MODEL_TYPE::MODEL_TYPE_AXIS) {
        vkDestroyBuffer(lveDevice.device(), vertexBuffer, nullptr);
        vkFreeMemory(lveDevice.device(), vertexBufferMemory, nullptr);
        vkDestroyBuffer(lveDevice.device(), indexBufferForEdge, nullptr);
        vkFreeMemory(lveDevice.device(), indexBufferMemoryForEdge, nullptr);
    }
    else if (this->model_type == MODEL_TYPE::MODEL_TYPE_PEX_RESISTOR) {
        vkDestroyBuffer(lveDevice.device(), vertexBuffer, nullptr);
        vkFreeMemory(lveDevice.device(), vertexBufferMemory, nullptr);
        vkDestroyBuffer(lveDevice.device(), indexBufferForFace, nullptr);
        vkFreeMemory(lveDevice.device(), indexBufferMemoryForFace, nullptr);
    }
    else if (this->model_type == MODEL_TYPE::MODEL_TYPE_PEX_CAPACITOR) {
        vkDestroyBuffer(lveDevice.device(), vertexBuffer, nullptr);
        vkFreeMemory(lveDevice.device(), vertexBufferMemory, nullptr);
        vkDestroyBuffer(lveDevice.device(), indexBufferForEdge, nullptr);
        vkFreeMemory(lveDevice.device(), indexBufferMemoryForEdge, nullptr);
    }

}
void LveModel::createVertexBuffers(
        const std::vector<Vertex>& vertices,
        VkBuffer& buffer,
        VkDeviceMemory& memory) {
    vertexCount = static_cast<uint32_t>(vertices.size());
    assert(vertexCount >= 3 && "Vertex count must be at least 3");
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    lveDevice.createBuffer(
                bufferSize,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                stagingBuffer,
                stagingBufferMemory);

    void* data;
    vkMapMemory(lveDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(lveDevice.device(), stagingBufferMemory);

    lveDevice.createBuffer(
                bufferSize,
                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                buffer,
                memory);

    lveDevice.copyBuffer(stagingBuffer, buffer, bufferSize);

    vkDestroyBuffer(lveDevice.device(), stagingBuffer, nullptr);
    vkFreeMemory(lveDevice.device(), stagingBufferMemory, nullptr);
}

void LveModel::createIndexBuffers(
        const std::vector<uint32_t>& indices,
        VkBuffer& buffer,
        VkDeviceMemory& memory) {
    assert( !indices.empty() && "Index size can't be zero");
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    lveDevice.createBuffer(
                bufferSize,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                stagingBuffer,
                stagingBufferMemory);

    void* data;
    vkMapMemory(lveDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(lveDevice.device(), stagingBufferMemory);

    lveDevice.createBuffer(
                bufferSize,
                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                buffer,
                memory);

    lveDevice.copyBuffer(stagingBuffer, buffer, bufferSize);

    vkDestroyBuffer(lveDevice.device(), stagingBuffer, nullptr);
    vkFreeMemory(lveDevice.device(), stagingBufferMemory, nullptr);
}

void LveModel::drawForFace(VkCommandBuffer commandBuffer) {
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(this->indices_face.size()), 1, 0, 0, 0);
}

void LveModel::drawForEdge(VkCommandBuffer commandBuffer) {
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(this->indices_edge.size()), 1, 0, 0, 0);
}

void LveModel::bindVertexBuffer(VkCommandBuffer commandBuffer) {
    VkBuffer buffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

void LveModel::bindDrawVertexIndexBufferForFace_layer(VkCommandBuffer commandBuffer, const float layer) {
    VkBuffer buffers[] = { layerbyVertexBuffers[layer] };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, layerbyFaceIndexBuffers[layer], 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(this->layerby_face[layer].size()), 1, 0, 0, 0);
}

void LveModel::bindDrawIndexBufferForEdge_layer(VkCommandBuffer commandBuffer, const float layer) {
    vkCmdBindIndexBuffer(commandBuffer, layerbyEdgeIndexBuffers[layer], 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(this->layerby_edge[layer].size()), 1, 0, 0, 0);
}



void LveModel::bindIndexBufferForFace(VkCommandBuffer commandBuffer) {
    vkCmdBindIndexBuffer(commandBuffer, this->indexBufferForFace, 0, VK_INDEX_TYPE_UINT32);
}

void LveModel::bindIndexBufferForEdge(VkCommandBuffer commandBuffer) {
    vkCmdBindIndexBuffer(commandBuffer, this->indexBufferForEdge, 0, VK_INDEX_TYPE_UINT32);
}


std::vector<VkVertexInputBindingDescription> LveModel::Vertex::getBindingDescriptions() {
    std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(Vertex);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> LveModel::Vertex::getAttributeDescriptions() {
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);
    return attributeDescriptions;
}

void LveModel::makeAxisData(const float axis_length) {
    this->vertices.clear();
    this->vertices = {
        { { -axis_length, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } },
        { {  axis_length, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
        { { 0.0f, -axis_length, 0.0f }, { 1.0f, 1.0f, 1.0f } },
        { { 0.0f,  axis_length, 0.0f }, { 0.0f, 1.0f, 0.0f } },
        { { 0.0f, 0.0f, -axis_length }, { 1.0f, 1.0f, 1.0f } },
        { { 0.0f, 0.0f,  axis_length }, { 0.0f, 0.0f, 1.0f } }
    };

    vector<uint32_t>& indices = this->indices_edge;
    indices.clear();
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(4);
    indices.push_back(5);
}

void LveModel::updateOpacity(float amount) {
    this->opacity += amount;
    if (this->opacity > 1.0f) this->opacity = 1.0f;
    if (this->opacity < 0.0f) this->opacity = 0.0f;
}

std::map<float, LveModel::LayerProperty> LveModel::getLayer() {
    //if(layerList.empty()){
    //    vector<glm::vec3> it = { glm::vec3{0.1,0.5,0.8},glm::vec3{0.8,0.4,0.2},glm::vec3{0.5,0.9,0.9} };
    //    int a = 0;
    //    for (auto ordered_key : drawing_order_layerby) {
    //
    //        layerList.insert(pair(ordered_key, LayerProperty{true,it.at(a%3), 0.5}) );
    //        a++;
    //    }
    //}
    return layerList;
}

void LveModel::changeLayerColor(float layernumber, glm::vec3 rgb){
    if(layerList.count(layernumber) != 0 ){
        layerList[layernumber].color = rgb;
    }

}

void LveModel::changeLayerOpacity(float layernumber, float opacity){
    if(layerList.count(layernumber) != 0 ){
        layerList[layernumber].opacity = opacity;
    }

}

void LveModel::changeLayerVisiblity(float layernumber, bool visiblity){
    if(layerList.count(layernumber) != 0 ){
        layerList[layernumber].visiblity = visiblity;
    }

}

void LveModel::demoVisiblityOff(){
    for(auto const& [key, val] : layerList){
        layerList[key].visiblity = false;
    }
    layerList[4].visiblity = true;

}

void LveModel::demoVisiblityOn(std::vector<float> layernumbers){
    for(auto layer : layernumbers){
        layerList[layer].visiblity = true;
    }
}

