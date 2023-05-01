#include <vulkan/vulkan.h>

#include "toycadmodel.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cassert>
#include <cmath>

#include <QFile>

using namespace std;

const double PI = 3.141591;
const double RANDIAN_FACTOR = PI / 180.0;

ToyCADModel::ToyCADModel(LveDevice& device, TOYCAD_DATA& toycad_data)
    : lveDevice{device}{
    this->model_type = MODEL_TYPE_TOY_CAD;
    num_points_for_circle = 20;
    this->makeRenderingData(toycad_data);
    this->createBuffers();
}
ToyCADModel::~ToyCADModel(){
    this->destroyBuffers();
    this->vertices.clear();
    this->indices_face.clear();
    this->indices_edge.clear();
}

void ToyCADModel::makeRenderingData(TOYCAD_DATA& toycad_data){
       //this->loadData();
    this->toycad_datatype = getCurDataType(toycad_data.type);
    this->makeVertices(toycad_data);
    this->makeIndices();
}

void ToyCADModel::makeVertices(TOYCAD_DATA& toycad_data){
    if(this->toycad_datatype == TOYCAD_TYPE_RECT) makeRectVertices(toycad_data);
    else if(this->toycad_datatype == TOYCAD_TYPE_CIRCLE) makeCircleVertices(toycad_data);
    else if(this->toycad_datatype == TOYCAD_TYPE_FAN) makeFanVertices(toycad_data);
    else if(this->toycad_datatype == TOYCAD_TYPE_HEXAGON) makeHexagonVertices(toycad_data);
    else if(this->toycad_datatype == TOYCAD_TYPE_OCTAGON) makeOctagonVertices(toycad_data);
    else{}
}

void ToyCADModel::makeRectVertices(TOYCAD_DATA& toycad_data){
    int num_of_vertex = 8;
    ToyCADVertex vertex[num_of_vertex];

    double half_x_length = (toycad_data.maxx - toycad_data.minx)*0.5;
    double half_y_length = (toycad_data.maxy - toycad_data.miny)*0.5;
    double height = toycad_data.maxz - toycad_data.minz;
    double delta_x = height*tan(RANDIAN_FACTOR*toycad_data.slopex);
    double delta_y = height*tan(RANDIAN_FACTOR*toycad_data.slopey);
    if( delta_x < -half_x_length) delta_x = -half_x_length;
    if( delta_y < -half_y_length) delta_y = -half_y_length;

    vertex[0].position = {toycad_data.minx, toycad_data.maxy, toycad_data.maxz};
    vertex[1].position = {toycad_data.maxx, toycad_data.maxy, toycad_data.maxz};
    vertex[2].position = {toycad_data.maxx, toycad_data.miny, toycad_data.maxz};
    vertex[3].position = {toycad_data.minx, toycad_data.miny, toycad_data.maxz};

    vertex[4].position = {toycad_data.minx - delta_x, toycad_data.maxy + delta_y, toycad_data.minz};
    vertex[5].position = {toycad_data.maxx + delta_x, toycad_data.maxy + delta_y, toycad_data.minz};
    vertex[6].position = {toycad_data.maxx + delta_x, toycad_data.miny - delta_y, toycad_data.minz};
    vertex[7].position = {toycad_data.minx - delta_x, toycad_data.miny - delta_y, toycad_data.minz};

    for(int i = 0; i < num_of_vertex; ++i)
        this->vertices.push_back(vertex[i]);
}

void ToyCADModel::makeCircleVertices(TOYCAD_DATA& toycad_data){(void)toycad_data;}
void ToyCADModel::makeFanVertices(TOYCAD_DATA& toycad_data){(void)toycad_data;}
void ToyCADModel::makeHexagonVertices(TOYCAD_DATA& toycad_data){(void)toycad_data;}
void ToyCADModel::makeOctagonVertices(TOYCAD_DATA& toycad_data){(void)toycad_data;}


void ToyCADModel::makeIndices(){
    if(this->toycad_datatype == TOYCAD_TYPE_RECT) makeRectIndices();
    else if(this->toycad_datatype == TOYCAD_TYPE_CIRCLE) makeCircleIndices();
    else if(this->toycad_datatype == TOYCAD_TYPE_FAN) makeFanIndices();
    else if(this->toycad_datatype == TOYCAD_TYPE_HEXAGON) makeHexagonIndices();
    else if(this->toycad_datatype == TOYCAD_TYPE_OCTAGON) makeOctagonIndices();
    else{
    }
}


void ToyCADModel::makeRectIndices(){
    //face
    std::vector<uint32_t>& face = this->indices_face;

    face.push_back(0); face.push_back(1); face.push_back(2);
    face.push_back(2); face.push_back(3); face.push_back(0);//top

    face.push_back(3); face.push_back(2); face.push_back(6);
    face.push_back(6); face.push_back(7); face.push_back(3);//front

    face.push_back(0); face.push_back(3); face.push_back(7);
    face.push_back(7); face.push_back(4); face.push_back(0);//left

    face.push_back(1); face.push_back(5); face.push_back(6);
    face.push_back(6); face.push_back(2); face.push_back(1);//right

    face.push_back(0); face.push_back(4); face.push_back(5);
    face.push_back(5); face.push_back(1); face.push_back(0);//back

    face.push_back(4); face.push_back(7); face.push_back(6);
    face.push_back(6); face.push_back(5); face.push_back(4);//bottom

    //edge
    std::vector<uint32_t>& edge = this->indices_edge;
    edge.push_back(0); edge.push_back(1);
    edge.push_back(1); edge.push_back(2);
    edge.push_back(2); edge.push_back(3);
    edge.push_back(3); edge.push_back(0);//top

    edge.push_back(4); edge.push_back(5);
    edge.push_back(5); edge.push_back(6);
    edge.push_back(6); edge.push_back(7);
    edge.push_back(7); edge.push_back(4);//bottom

    edge.push_back(0); edge.push_back(4);
    edge.push_back(1); edge.push_back(5);
    edge.push_back(2); edge.push_back(6);
    edge.push_back(3); edge.push_back(7);//side


}

void ToyCADModel::makeCircleIndices(){}
void ToyCADModel::makeFanIndices(){}
void ToyCADModel::makeHexagonIndices(){}
void ToyCADModel::makeOctagonIndices(){}


int ToyCADModel::getCurDataType(std::string& str_type){
    TOYCAD_TYPE_LIST type;
    if(str_type == "rect") type = TOYCAD_TYPE_RECT;
    else if(str_type == "circle") type = TOYCAD_TYPE_CIRCLE;
    else if(str_type == "fan") type = TOYCAD_TYPE_FAN;
    else if(str_type == "hexagon") type = TOYCAD_TYPE_HEXAGON;
    else if(str_type == "octagon") type = TOYCAD_TYPE_OCTAGON;
    else type = TOYCAD_TYPE_DEFAULT;

    return type;
}

void ToyCADModel::createBuffers(){
    createVertexBuffer(this->vertices, this->vertexBuffer, this->vertexBufferMemory);
    createIndexBuffer(this->indices_face, this->indexBufferForFace, this->indexBufferMemoryForFace);
    createIndexBuffer(this->indices_edge, this->indexBufferForEdge, this->indexBufferMemoryForEdge);
}

void ToyCADModel::destroyBuffers(){
    vkDestroyBuffer(lveDevice.device(), vertexBuffer, nullptr);
    vkFreeMemory(lveDevice.device(), vertexBufferMemory, nullptr);
    vkDestroyBuffer(lveDevice.device(), indexBufferForFace, nullptr);
    vkFreeMemory(lveDevice.device(), indexBufferMemoryForFace, nullptr);
    vkDestroyBuffer(lveDevice.device(), indexBufferForEdge, nullptr);
    vkFreeMemory(lveDevice.device(), indexBufferMemoryForEdge, nullptr);
}

void ToyCADModel::createVertexBuffer(
        const std::vector<ToyCADVertex>& vertices,
        VkBuffer& buffer,
        VkDeviceMemory& memory){
    uint32_t vertexCount = static_cast<uint32_t>(vertices.size());
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
void ToyCADModel::createIndexBuffer(
        const std::vector<uint32_t>& indices,
        VkBuffer & buffer,
        VkDeviceMemory & memory){
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

void ToyCADModel::bindVertexBuffer(VkCommandBuffer commandBuffer){
    VkBuffer buffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

void ToyCADModel::bindIndexBufferForFace(VkCommandBuffer commandBuffer){
    vkCmdBindIndexBuffer(commandBuffer, this->indexBufferForFace, 0, VK_INDEX_TYPE_UINT32);
}

void ToyCADModel::bindIndexBufferForEdge(VkCommandBuffer commandBuffer){
    vkCmdBindIndexBuffer(commandBuffer, this->indexBufferForEdge, 0, VK_INDEX_TYPE_UINT32);
}

void ToyCADModel::drawForFace(VkCommandBuffer commandBuffer){
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(this->indices_face.size()), 1, 0, 0, 0);
}

void ToyCADModel::drawForEdge(VkCommandBuffer commandBuffer){
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(this->indices_edge.size()), 1, 0, 0, 0);
}

std::vector<VkVertexInputBindingDescription> ToyCADModel::ToyCADVertex::getBindingDescriptions() {
    std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(ToyCADVertex);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> ToyCADModel::ToyCADVertex::getAttributeDescriptions() {
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(ToyCADVertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(ToyCADVertex, color);
    return attributeDescriptions;
}

void ToyCADModel::test(){
    qDebug() << "test ToyCAD model";
}
