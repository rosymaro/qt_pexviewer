#ifndef TOYCAD_OBJECT_H
#define TOYCAD_OBJECT_H

#include "toycadmodel.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>


struct ToyCADTransformComponent {
    glm::vec3 translation{};
    glm::vec3 scale{ 1.f, 1.f, 1.f };
    glm::vec3 rotation{};


    glm::mat4 mat4() {
        glm::mat4 glm_mat4 = glm::mat4{ 1 };
        //glm_mat4 = glm::rotate(glm_mat4,  rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        //glm_mat4 = glm::rotate(glm_mat4, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));

        glm_mat4 = glm::translate(glm_mat4, translation);
        glm_mat4 = glm::rotate(glm_mat4,
                               glm::radians(rotation.z),
                               glm::vec3(0.0f, 0.0f, 1.0f));
        glm_mat4 = glm::scale(glm_mat4, scale);


        return glm_mat4;
    }
};

class ToyCADObject {
public:
    static ToyCADObject createToyCADObject() {
        static unsigned int currentId = 0;
        return ToyCADObject{ currentId++ };
    }

    ToyCADObject(unsigned int objId) : id{ objId } {}

    ToyCADObject(const ToyCADObject&) = delete;
    ToyCADObject& operator=(const ToyCADObject&) = delete;
    ToyCADObject(ToyCADObject&&) = default;
    ToyCADObject& operator=(ToyCADObject&&) = default;

public:
    std::shared_ptr<ToyCADModel> model{};
    glm::vec3 color{};
    float opacity{1.0f};
    bool visibility{true};

    ToyCADTransformComponent transform{};

private:
    unsigned int id;

public:
    unsigned int getId() { return this->id; }
    bool getVisibility(){return this->visibility;}
    void setVisibility(bool visibility){this->visibility = visibility;}
};




#endif // TOYCAD_OBJECT_H
