#ifndef TOYCADMODEL_H
#define TOYCADMODEL_H



#include <iostream>
#include <vector>
#include <string>

#include "Rendering/Src/lve_device.hpp"
#include "Rendering/Src/lve_model.hpp"
#include "toycaddata.h"


class ToyCADModel
{
public:
    struct ToyCADVertex {
        glm::vec3 position{};
        glm::vec3 color{};

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };

public:
    ToyCADModel(LveDevice& device, TOYCAD_DATA& toycad_data);
    ~ToyCADModel();

    ToyCADModel() = delete;
    ToyCADModel(const ToyCADModel&) = delete;
    ToyCADModel& operator=(const ToyCADModel&) = delete;


    void bindVertexBuffer(VkCommandBuffer commandBuffer);
    void bindIndexBufferForFace(VkCommandBuffer commandBuffer);
    void bindIndexBufferForEdge(VkCommandBuffer commandBuffer);
    void drawForFace(VkCommandBuffer commandBuffer);
    void drawForEdge(VkCommandBuffer commandBuffer);

    void createBuffers();
    void destroyBuffers();
    void createVertexBuffer(const std::vector<ToyCADVertex>& vertices, VkBuffer& buffer, VkDeviceMemory& memory);
    void createIndexBuffer(const std::vector<uint32_t>& indices, VkBuffer & buffer, VkDeviceMemory & memory);

public:
    void makeRenderingData(TOYCAD_DATA& toycad_data);
    void setNumPointsForCircle(unsigned int num_){this->num_points_for_circle=num_;}
    MODEL_TYPE getModelType() { return this->model_type; }

private:
    void makeVertices(TOYCAD_DATA& toycad_data);
    void makeIndices();

    void makeRectVertices(TOYCAD_DATA& toycad_data);
    void makeCircleVertices(TOYCAD_DATA& toycad_data);
    void makeFanVertices(TOYCAD_DATA& toycad_data);
    void makeHexagonVertices(TOYCAD_DATA& toycad_data);
    void makeOctagonVertices(TOYCAD_DATA& toycad_data);

    void makeRectIndices();
    void makeCircleIndices();
    void makeFanIndices();
    void makeHexagonIndices();
    void makeOctagonIndices();

    //void makeIndicesForEdge();
    int getCurDataType(std::string& str_type);

public:
    LveDevice& lveDevice;
    VkBuffer vertexBuffer{};
    VkDeviceMemory vertexBufferMemory{};
    VkBuffer indexBufferForFace{};
    VkDeviceMemory indexBufferMemoryForFace{};
    VkBuffer indexBufferForEdge{};
    VkDeviceMemory indexBufferMemoryForEdge{};

    std::vector<ToyCADVertex> vertices{};
    std::vector<uint32_t> indices_face{};
    std::vector<uint32_t> indices_edge{};



private:
    unsigned int num_points_for_circle;
    int toycad_datatype;
    MODEL_TYPE model_type{};

public:
    void test();
};

#endif // TOYCADMODEL_H
