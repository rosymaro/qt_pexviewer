#include "LayoutPEXData.h"
#include <iostream>
#include <string>
#include <vector>

#include <limits>
#include <cstdio>

#include "rapidcsv.h"

#include <QFile>
#include <QString>
#include <QDebug>
using namespace std;


/// <summary>
/// PEXItem
/// ///////////////////////////////////////////////////////////////
/// </summary>
PEXItem::PEXItem() : node1{}, node2{}, name{} {
    this->value = 0.0;
}

PEXItem::PEXItem(const node& node1_, const node& node2_, const double& value_, const string& name_) :
    node1(node1_), node2(node2_), value(value_), name(name_) {
}

PEXItem::~PEXItem() {
}

void PEXItem::print() {
    printf("%s\n", this->name.c_str());
    printf("Node1 name/layer#/x1,y1/Z-s,Z-e = %s / %d.%d / %.5f,%.5f / %.5f,%.5f\n",
        this->node1.name.c_str(), this->node1.layer_number, this->node1.layer_datatype,
        this->node1.x, this->node1.y, this->node1.z_start, this->node1.z_end);
    printf("Node2 name/layer#/x1,y1/Z-s,Z-e = %s / %d.%d / %.5f,%.5f / %.5f,%.5f\n",
        this->node1.name.c_str(), this->node1.layer_number, this->node1.layer_datatype,
        this->node2.x, this->node2.y, this->node2.z_start, this->node2.z_end);
}


/// <summary>
/// PEXResistor
/// ///////////////////////////////////////////////////////////////
/// </summary>
PEXResistor::PEXResistor() : PEXItem(), direction(RES_DIRECTION_NONE) {
}
PEXResistor::PEXResistor(const string& name_, const node& node1_, const node& node2_, const double& value_, const PEXResDirection& direction_) :
    PEXItem(node1_, node2_, value_, name_), direction(direction_) {
}

void PEXResistor::print() {
    PEXItem::print();
    printf("Direction = ");
    if (this->direction == RES_DIRECTION_VERTICAL) printf("Vertical\n");
    else printf("Horizontal\n");
    printf("Resistor Value = %10.5f\n\n", this->getValue());
}

PEXResistor::~PEXResistor() {
}

PEXResDirection PEXResistor::checkDirectionFromDescription(const std::string& description) {
    PEXResDirection direction;
    if (description.find("$a") == std::string::npos) {
        direction = RES_DIRECTION_HORIZONTAL;
    }
    else {
        direction = RES_DIRECTION_VERTICAL;
    }
    //printf("#####Description = %s, ", description.c_str());
    //if (direction == RES_DIRECTION_VERTICAL) printf("Vertical\n");
    //else printf("Horizontal\n");

    return direction;
}

/// <summary>
/// PEXCapacitor
/// ///////////////////////////////////////////////////////////////
/// </summary>
PEXCapacitor::PEXCapacitor() : PEXItem() {
}
PEXCapacitor::PEXCapacitor(const std::string& name_, const node& node1_, const node& node2_, const double& value_) :
    PEXItem(node1_, node2_, value_, name_) {
}
PEXCapacitor::~PEXCapacitor() {
}

void PEXCapacitor::print() {
    PEXItem::print();
    printf("Capacitor Value = %10.5e\n\n", this->getValue());
}

/// <summary>
/// PEXResistorDataManager
/// ///////////////////////////////////////////////////////////////
/// </summary>
PEXResistorDataManager::PEXResistorDataManager() {
    this->resistors = {};
    this->file_path = "";
    this->max_resistor_vertical = std::numeric_limits<double>::min();
    this->min_resistor_vertical = std::numeric_limits<double>::max();
    this->max_resistor_horizontal = std::numeric_limits<double>::min();
    this->min_resistor_horizontal = std::numeric_limits<double>::max();
}

PEXResistorDataManager::~PEXResistorDataManager() {
    printf("\n\n\t~PEXDataManager\n");
    this->resistors.clear();
    this->resistors_ref.clear();
    this->resistors_of_node.clear();
}

void PEXResistorDataManager::clear() {
    this->resistors.clear();
}

void PEXResistorDataManager::loadData(const QString file_path) {
    if (!QFile::exists(file_path)) {
        qDebug() << "Error! " << file_path << " is not exist!!\n";
        return;
    }
    qDebug() << "resistor parsing start";


    this->file_path = file_path;
    rapidcsv::Document infile(file_path.toStdString(), rapidcsv::LabelParams(-1, -1));

    string item_name{}, node1_lvl{}, node2_lvl{};
    node node1{}, node2{};
    string description{};
    double value = 0.0;

    size_t line_count = infile.GetRowCount();
    for (size_t i = 1; i < line_count; ++i) {
        vector<string> line = infile.GetRow<string>(i);

        item_name = line[PEXINFO_INDEX::PEXINFO_INDEX_NAME];
        if (item_name[0] != 'R') continue;

        node1.name = line[PEXINFO_INDEX::PEXINFO_INDEX_NODE1_NAME];
        node1.x = stof(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE1_X]);
        node1.y = stof(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE1_Y]);
        node1.z_start = stof(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE1_ZSTART]);
        node1.z_end = stof(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE1_ZEND]);
        node1.layer_number = stoi(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE1_LAYER_NUMBER]);
        node1.layer_datatype = stoi(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE1_LAYER_DATATYPE]);

        node2.name = line[PEXINFO_INDEX::PEXINFO_INDEX_NODE2_NAME];
        node2.x = stof(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE2_X]);
        node2.y = stof(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE2_Y]);
        node2.z_start = stof(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE2_ZSTART]);
        node2.z_end = stof(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE2_ZEND]);
        node2.layer_number = stoi(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE2_LAYER_NUMBER]);
        node2.layer_datatype = stoi(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE2_LAYER_DATATYPE]);

        value = stof(line[PEXINFO_INDEX::PEXINFO_INDEX_VALUE]);

        node1_lvl = line[PEXINFO_INDEX_NODE1_LVL];
        node2_lvl = line[PEXINFO_INDEX_NODE2_LVL];


        if (node1_lvl == "RX") {
            node1.x = node2.x;
            node1.y = node2.y;
        }

        description = line[PEXINFO_INDEX::PEXINFO_INDEX_DESCRIPTION];
        PEXResDirection resistor_direction = PEXResistor::checkDirectionFromDescription(description);
        if (value > 0.005) {
            updateMinMaxRes(value, resistor_direction); //임의의 코드, 0보다 작은 저항은 무시
            this->resistors_ref.push_back(PEXResistor(item_name, node1, node2, value, resistor_direction));
        }
    }
    this->resistors = this->resistors_ref;
}

void PEXResistorDataManager::groupResistorOfNode() {
    for (const auto& resistor : this->resistors_ref) {
        bool found = false;
        for (auto& resistors_v : this->resistors_of_node) {
            if (!resistors_v.empty()) {
                for (auto& resistor1 : resistors_v) {
                    if (resistor1.node1.name == resistor.node1.name
                        || resistor1.node1.name == resistor.node2.name
                        || resistor1.node2.name == resistor.node1.name
                        || resistor1.node2.name == resistor.node2.name) {
                        resistors_v.push_back(resistor);
                        found = true;
                        break;
                    }
                }
            }
        }
        if (!found) {
            this->resistors_of_node.emplace_back(std::vector<PEXResistor>{resistor});
        }
    }



    int i = 0;
    while (i < this->resistors_of_node.size()) {
        int j = 0;
        while (j < this->resistors_of_node.size()) {
            bool merge = false;
            for (auto& resistor1 : resistors_of_node[i]) {
                if (merge) {
                    break;
                }
                if (i == j) {
                }
                else {
                    for (auto& resistor2 : resistors_of_node[j]) {
                        if (resistor1.node1.name == resistor2.node1.name
                            || resistor1.node1.name == resistor2.node2.name
                            || resistor1.node2.name == resistor2.node1.name
                            || resistor1.node2.name == resistor2.node2.name) {
                            resistors_of_node[i].insert(resistors_of_node[i].end(), resistors_of_node[j].begin(), resistors_of_node[j].end());
                            resistors_of_node.erase(resistors_of_node.begin() + j);
                            merge = true;
                            break;
                        }
                    }
                }
            }
            if (!merge) {
                j++;
            }
        }
        i++;
    }
    qDebug() << "resistors_of_node size confirm &&&&&&&&&&&&&&&&&&&&&&&" << resistors_of_node.size();

    for (auto& loop : resistors_of_node[0]){
        qDebug() << "loop test XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    }


    for (const auto& nodes : this->resistors_of_node) {

            qDebug() << "------------";
        for (const auto& node : nodes) {
            qDebug() << QString::fromStdString(node.name);
                //<< ", " << node.node1.name << ", " << node.node1.layer_number << ", " << node.node2.name << ", " << node.node2.layer_number << ", ";
        }
    }
};

void PEXResistorDataManager::setDriectionOfResistor(PEXResDirection _direction) {
    std::vector<PEXResistor> _resistors_buf;
    if (this->number_of_resistor_group > -1) {
        for (auto& resistor_ : this->resistors_of_node[number_of_resistor_group % resistors_of_node.size()]) {
            if (resistor_.getDirection() == _direction) {
                _resistors_buf.push_back(resistor_);
            }
        }
        this->resistors = _resistors_buf;
    }
    else if (this->number_of_resistor_group == -1) {
        for (auto& resistor_ : this->resistors_ref) {
            if (resistor_.getDirection() == _direction) {
                _resistors_buf.push_back(resistor_);
            }
        }
        this->resistors = _resistors_buf;
    }
    for (const auto& node : this->resistors) {
        std::cout << node.name << ", " << node.value << ", " << node.node1.name << ", " << node.node1.x << ", " << node.node1.y << ", " << node.node1.z_start << ", " << node.node1.z_end << ", " << node.node1.layer_number << ", " << node.node2.name << ", " << node.node2.x << ", " << node.node2.y << ", " << node.node2.z_start << ", " << node.node2.z_end << ", " << node.node2.layer_number << ", " << std::endl;
    }

}

inline void PEXResistorDataManager::updateMinMaxRes(const double& new_value, const PEXResDirection direction) {
    if (direction == PEXResDirection::RES_DIRECTION_VERTICAL) {
        if (this->min_resistor_vertical > new_value) this->min_resistor_vertical = new_value;
        if (this->max_resistor_vertical < new_value) this->max_resistor_vertical = new_value;
    }
    else {
        if (this->min_resistor_horizontal > new_value) this->min_resistor_horizontal = new_value;
        if (this->max_resistor_horizontal < new_value) this->max_resistor_horizontal = new_value;
    }

}

void PEXResistorDataManager::printData() {
    for (auto item : this->resistors) {
        item.print();
    }
    printf("##Resistor Count : %lld, Vertical Res Max = %10.5f, Min = %10.5f, Horizontal Res Max = %10.5f, Min = %10.5f\n",
        this->resistors.size(),
        this->getMaxResistorVerticalValue(), this->getMinResistorVerticalValue(),
        this->getMaxResistorHorizontalValue(), this->getMinResistorHorizontalValue());
}


/// <summary>
/// PEXCapacitorDataManager
/// ///////////////////////////////////////////////////////////////
/// </summary>
PEXCapacitorDataManager::PEXCapacitorDataManager() {
    this->capasitors = {};
    this->file_path = "";
    this->max_capacitor = std::numeric_limits<double>::min();
    this->min_capacitor = std::numeric_limits<double>::max();
}

PEXCapacitorDataManager::~PEXCapacitorDataManager() {
    this->capasitors.clear();
}

void PEXCapacitorDataManager::clear() {
    this->capasitors.clear();
}

void PEXCapacitorDataManager::loadData(const QString file_path) {
    if (!QFile::exists(file_path)) {
        qDebug() << "Error! " << file_path << " is not exist!!\n";
        return;
    }
    qDebug() << "capacitor parsing start";

    this->file_path = file_path;
    rapidcsv::Document infile(file_path.toStdString(), rapidcsv::LabelParams(-1, -1));

    string item_name{}, node1_lvl{}, node2_lvl{};
    node node1{}, node2{};
    string description{};
    double value = 0.0;

    size_t line_count = infile.GetRowCount();
    for (size_t i = 1; i < line_count; ++i) {
        vector<string> line = infile.GetRow<string>(i);

        item_name = line[PEXINFO_INDEX::PEXINFO_INDEX_NAME];
        if (item_name[0] != 'C') continue;

        node1.name = line[PEXINFO_INDEX::PEXINFO_INDEX_NODE1_NAME];
        node1.x = stof(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE1_X]);
        node1.y = stof(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE1_Y]);
        node1.z_start = stof(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE1_ZSTART]);
        node1.z_end = stof(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE1_ZEND]);
        node1.layer_number = stoi(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE1_LAYER_NUMBER]);
        node1.layer_datatype = stoi(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE1_LAYER_DATATYPE]);

        node2.name = line[PEXINFO_INDEX::PEXINFO_INDEX_NODE2_NAME];
        node2.x = stof(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE2_X]);
        node2.y = stof(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE2_Y]);
        node2.z_start = stof(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE2_ZSTART]);
        node2.z_end = stof(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE2_ZEND]);
        node2.layer_number = stoi(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE2_LAYER_NUMBER]);
        node2.layer_datatype = stoi(line[PEXINFO_INDEX::PEXINFO_INDEX_NODE2_LAYER_DATATYPE]);

        value = stof(line[PEXINFO_INDEX::PEXINFO_INDEX_VALUE]);

        node1_lvl = line[PEXINFO_INDEX_NODE1_LVL];
        node2_lvl = line[PEXINFO_INDEX_NODE2_LVL];

        updateMinMaxCap(value);
        this->capasitors.push_back(PEXCapacitor(item_name, node1, node2, value));
    }
}


inline void PEXCapacitorDataManager::updateMinMaxCap(const double& new_value) {
    if (this->min_capacitor > new_value) this->min_capacitor = new_value;
    if (this->max_capacitor < new_value) this->max_capacitor = new_value;
}


void PEXCapacitorDataManager::printData() {

    std::cout << "\n\nPEX Capacitors\n";
    for (auto item : this->capasitors) {
        item.print();
    }
    printf("##Capacitor Count : %lld, Max cap = %10.5e, Min cap = %10.5e\n",
        this->capasitors.size(), this->getMaxCapacitorValue(), this->getMinCapacitorValue());
}


void testPEXData() {
    PEXResistorDataManager resistor_data;
    resistor_data.loadData("Data/MVP_PEX_RC_r1_0130_0208version.csv");
    resistor_data.printData();

    PEXCapacitorDataManager capacitor_data;
    capacitor_data.loadData("Data/MVP_PEX_RC_r1_0130_0208version.csv");
    capacitor_data.printData();
}
