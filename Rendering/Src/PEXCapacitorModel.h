#pragma once
#include "lve_model.hpp"
#include "LayoutPEXData.h"

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <limits>
#include <thread>
#include <queue>
#include <mutex>
#include <cstdio>


typedef unsigned int uint;

class PEXCapacitorModel : public LveModel
{
public:
    struct cap_node {
        double x;
        double y;
        uint layer_number;
        uint layer_datatype;
        std::string name;
        double value;
        uint connected_count;
    };

    struct pattern_cap {
        cube_info pattern;
        uint layer_number;
        uint layer_datatype;
        double cap_value;
        double normalized_cap_value;
        uint cap_count;
    };

public:
    PEXCapacitorModel(
            LveDevice& device, MODEL_TYPE model_type,
            const QString& data_file = "", T2D * layout_data_ =nullptr);
    ~PEXCapacitorModel();

    PEXCapacitorModel() = delete;
    PEXCapacitorModel(const PEXCapacitorModel&) = delete;
    PEXCapacitorModel& operator=(const PEXCapacitorModel&) = delete;

private:
    PEXCapacitorDataManager pex_data;
    std::map<std::string, size_t> cap_node_name_to_index_map;
    T2D* layout_data =nullptr;

    std::vector<cap_node> cap_nodes;
    std::map<std::string, std::vector<cap_node*>> layer_to_cap_node_map;

    std::vector<pattern_cap> pattern_caps;
    std::map<std::string, std::vector<pattern_cap*>> layer_to_pattern_cap_map;

    std::set<std::pair<uint, uint>> layers;

    double max_cap = std::numeric_limits<double>::min();

    size_t num_threads = { 4 };
    std::vector<std::thread> threads;
    std::queue<std::string> layers_queue;
    std::mutex mutex_layers_queue;

public:
    virtual void makeRenderingData(const QString& file_path = "");
    virtual void loadData(const QString& file_path = "");
    virtual void makeVertices();
    virtual void makeIndices();

    void makeCubeVertices();

private:
    void makeCapNodesFromPEXData();
    void makePatternCapsFromLayoutData();

    void attachCapToPattern();
    void makeLayerToCapNodeMap();
    void makeLayerToPatternCapMap();

    void matchCapWithPatternThread(std::queue<std::string>* layers, std::mutex* mutex_);
    void matchCapWithPattern(std::vector<cap_node*>& caps, std::vector<pattern_cap*>& patterns);
    void matchCapWithPattern(
            std::map<std::string, std::vector<cap_node*>>& cap_layer_map_,
            std::map<std::string, std::vector<pattern_cap*>>& pattern_layer_map_,
            uint target_layer_number, uint target_layer_datatype);
    bool isPatternIncludeCap(const pattern_cap& pattern, const cap_node& cap);

    void setLayoutDataManager(T2D* layout_data_) { this->layout_data = layout_data_; }
    void addNewCapNode(pex_node& new_node, double value);

    std::string getLayerString(uint layer_number, uint layer_datatype);

    void normalizePatternCap();

    void makeLayersQueueForThreadJob();
    double percentileOfCapacitor(const pattern_cap& in_cap);



    void printCapNodes();
    void printPatternCaps(FILE* stream = stdout);
    void printLayerToCapNodeMap(FILE* stream = stdout);
    void printLayerToPatternCapMap(const char* msg = "");
    void printLayerList();
};


