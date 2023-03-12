#include "keyboard_movement_controller.hpp"

#define GLM_ENABLE_EXPERIMENTAL

// std
#include <limits>
#include <iostream>

#include <glm/gtx/string_cast.hpp>
#include "lve_model.hpp"

void KeyboardMovementController::setAllMoveFlagOff() {
    camera_moving_flag.rotateLeft = false;
    camera_moving_flag.rotateRight = false;
    camera_moving_flag.moveForward = false;
    camera_moving_flag.moveBackward = false;
    camera_moving_flag.rotateForward = false;
    camera_moving_flag.rotateBackward = false;
}

void KeyboardMovementController::setMouseFlagOff() {
    camera_moving_flag.mouseLeft = false;
    camera_moving_flag.mouseRight = false;
}

void KeyboardMovementController::moveKeyPressed(Qt::Key key) {
    //setAllMoveFlagOff();
    if (key == camera_move_key.moveForward) camera_moving_flag.moveForward = true;
    if (key == camera_move_key.moveBackward) camera_moving_flag.moveBackward = true;
    if (key == camera_move_key.rotateRight) camera_moving_flag.rotateRight = true;
    if (key == camera_move_key.rotateLeft) camera_moving_flag.rotateLeft = true;
    if (key == camera_move_key.rotateForward) camera_moving_flag.rotateForward = true;
    if (key == camera_move_key.rotateBackward) camera_moving_flag.rotateBackward = true;
    if (key == camera_move_key.moveUp) camera_moving_flag.moveUp = true;
    if (key == camera_move_key.moveDown) camera_moving_flag.moveDown = true;
    if (key == camera_move_key.moveLeft) camera_moving_flag.moveLeft = true;
    if (key == camera_move_key.moveRight) camera_moving_flag.moveRight = true;

}

void KeyboardMovementController::moveKeyReleased(Qt::Key key) {
    //setAllMoveFlagOff();
    if (key == camera_move_key.moveForward) camera_moving_flag.moveForward = false;
    if (key == camera_move_key.moveBackward) camera_moving_flag.moveBackward = false;
    if (key == camera_move_key.rotateRight) camera_moving_flag.rotateRight = false;
    if (key == camera_move_key.rotateLeft) camera_moving_flag.rotateLeft = false;
    if (key == camera_move_key.rotateForward) camera_moving_flag.rotateForward = false;
    if (key == camera_move_key.rotateBackward) camera_moving_flag.rotateBackward = false;
    if (key == camera_move_key.moveUp) camera_moving_flag.moveUp = false;
    if (key == camera_move_key.moveDown) camera_moving_flag.moveDown = false;
    if (key == camera_move_key.moveLeft) camera_moving_flag.moveLeft = false;
    if (key == camera_move_key.moveRight) camera_moving_flag.moveRight = false;
}

void KeyboardMovementController::setMousePosition(QPoint position, int mode) {
    mouse_position.currentPosition = position;
    if (mode == 1) {
        mouse_position.pressPosition = position;
        mouse_position.currentPosition = position;
        xpos_prev = position.x();
        ypos_prev = position.y();
    }
    if (mode == 2) {
        mouse_position.releasePosition = position;
        mouse_position.currentPosition = position;
    }

    //qDebug()<< position.y();
}

void KeyboardMovementController::setMouseScroll(QPoint scroll) {
    mouse_position.scroll = scroll;
}

void KeyboardMovementController::moveButtonPressed(Qt::MouseButtons buttons) {
    setMouseFlagOff();
    if (buttons == camera_move_key.mouseLeft) camera_moving_flag.mouseLeft = true;
    if (buttons == camera_move_key.mouseRight) camera_moving_flag.mouseRight = true;


}

void KeyboardMovementController::moveButtonReleased(Qt::MouseButtons buttons) {
    setMouseFlagOff();
    if (buttons == camera_move_key.mouseLeft) camera_moving_flag.mouseLeft = true;
    if (buttons == camera_move_key.mouseRight) camera_moving_flag.mouseRight = true;
    //qDebug()<<
}

void KeyboardMovementController::moveCamera(
    float dt, LveCamera& camera, float render_scale, std::vector<LveGameObject>& gameObjects) {
    float scale = 0.0f;
    float rotate_lr = 0.0f;
    float rotate_ud = 0.0f;
    float obj_scale = 2.0f / render_scale;

    if (camera_moving_flag.moveForward) scale = 1.0f + 0.01f * obj_scale;
    else if (camera_moving_flag.moveBackward) scale = 1.0f - 0.01f * obj_scale;

    if (camera_moving_flag.rotateRight) rotate_lr = -0.1f * obj_scale;
    else if (camera_moving_flag.rotateLeft) rotate_lr = 0.1f * obj_scale;

    if (camera_moving_flag.rotateForward) rotate_ud = -0.1f * obj_scale;
    else if (camera_moving_flag.rotateBackward) rotate_ud = 0.1f * obj_scale;
    //if (glfwGetMouseButton(window, camera_move_key.mouseLeft) == GLFW_PRESS) scale = 1.0f + 0.0001f * obj_scale;
    //if (glfwGetMouseButton(window, camera_move_key.mouseRight) == GLFW_PRESS) scale = 1.0f - 0.0001f * obj_scale;

    if (glm::dot(rotate_lr, rotate_lr) > std::numeric_limits<float>::epsilon()) {
        camera.viewMatrix = glm::rotate(camera.getView(), glm::radians(rotate_lr), glm::vec3(0.0f, 0.0f, 1.0f));
        //std::cout << glm::to_string(camera.rotation_d) << '\n';
    }

    glm::vec3 direction{ camera.viewMatrix[0][2], camera.viewMatrix[1][2], 0 };
    const glm::vec3 u{ glm::normalize(glm::cross(direction, glm::normalize(glm::cross(direction, glm::vec3{0,1,0})))) };
    const glm::vec3 v{ glm::normalize(glm::cross(u,glm::vec3{0,0,1})) };

    if (glm::dot(rotate_ud, rotate_ud) > std::numeric_limits<float>::epsilon()) {

        camera.viewMatrix = glm::rotate(camera.getView(), glm::radians(rotate_ud), u);
    }

    if (glm::dot(scale, scale) > std::numeric_limits<float>::epsilon()) {
        camera.viewMatrix = glm::scale(camera.getView(), glm::vec3(scale, scale, scale));
    }


    glm::vec3 moveDir{ 0.f };
    camera.decomposeView(camera.getView());

    if (camera.rotation_d.x * camera.rotation_d.y > 0) {
        if (camera_moving_flag.moveRight) moveDir -= u;
        if (camera_moving_flag.moveLeft)  moveDir += u;
        if (camera_moving_flag.moveUp)  moveDir += v;
        if (camera_moving_flag.moveDown)  moveDir -= v;
    }
    else {
        if (camera_moving_flag.moveRight) moveDir += u;
        if (camera_moving_flag.moveLeft)  moveDir -= u;
        if (camera_moving_flag.moveUp)  moveDir -= v;
        if (camera_moving_flag.moveDown)  moveDir += v;
    }


    if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
        //camera.decomposeView(camera.getView());
        //std::cout << glm::to_string(camera.rotation_d) << '\n';
        for (auto& gameobject : gameObjects) {
            if (gameobject.model->getModelType() != MODEL_TYPE::MODEL_TYPE_AXIS) {
                gameobject.transform.translation += obj_scale * moveSpeed * dt * glm::normalize(moveDir);
                //camera.viewMatrix = glm::translate(camera.getView(), moveSpeed * dt * moveDir);
            }
        }

    }
}

void KeyboardMovementController::moveCameraMouse(LveCamera& camera, float render_scale, std::vector<LveGameObject>& gameObjects) {
    if (camera_moving_flag.mouseRight)
    {
        std::cout << "rotation enable" << std::endl;
        float rotate_lr = 0.0f;
        float rotate_ud = 0.0f;

        rotate_lr = mouse_position.currentPosition.x() - xpos_prev;
        rotate_ud = mouse_position.currentPosition.y() - ypos_prev;

        glm::vec3 direction{ camera.viewMatrix[0][2], camera.viewMatrix[1][2], 0 };
        const glm::vec3 u{ glm::normalize(glm::cross(direction,glm::normalize(glm::cross(direction, glm::vec3{0,1,0})))) };
        const glm::vec3 v{ glm::normalize(glm::cross(u,glm::vec3{0,0,1})) };

        if (glm::dot(rotate_ud, rotate_ud) > std::numeric_limits<float>::epsilon()) {
            if (camera.viewMatrix[1][0] > 0) {
                camera.viewMatrix = glm::rotate(camera.getView(), glm::radians(rotate_ud*0.5f), u);
            }
            else {
                camera.viewMatrix = glm::rotate(camera.getView(), glm::radians(-rotate_ud*0.5f), u);
            }

        }

        if (glm::dot(rotate_lr, rotate_lr) > std::numeric_limits<float>::epsilon()) {
            camera.viewMatrix = glm::rotate(camera.getView(), glm::radians(rotate_lr*0.5f), glm::vec3(0.0f, 0.0f, 1.0f));
            //std::cout << glm::to_string(camera.rotation_d) << '\n';
        }

        xpos_prev = mouse_position.currentPosition.x();
        ypos_prev = mouse_position.currentPosition.y();
    }
    else if (camera_moving_flag.mouseLeft)
    {
        float camera_scale = sqrt(pow(camera.viewMatrix[0][2], 2) + pow(camera.viewMatrix[1][2], 2) + pow(camera.viewMatrix[2][2], 2));
        float obj_scale = gameObjects.begin()->transform.scale.x;

        float scale = 2.0f / camera_scale / obj_scale;
        glm::vec3 moveDir{ 0.f };
        float movement_lr = mouse_position.currentPosition.x() - xpos_prev;
        float movement_ud = mouse_position.currentPosition.y() - ypos_prev;

        glm::vec3 direction{ camera.viewMatrix[0][2], camera.viewMatrix[1][2], 0 };
        const glm::vec3 u{ glm::normalize(glm::cross(direction, glm::vec3{0,0,1})) };
        const glm::vec3 v{ glm::normalize(glm::cross(u,glm::vec3{0,0,1})) };


        if (camera.viewMatrix[2][2] > 0 && camera.viewMatrix[2][1] > 0) {

            moveDir -= u * (movement_lr);
            moveDir -= v * (movement_ud);
        }
        else if (camera.viewMatrix[2][2] < 0 && camera.viewMatrix[2][1] > 0) {
            moveDir -= u * (movement_lr);
            moveDir += v * (movement_ud);
        }
        else if (camera.viewMatrix[2][2] < 0 && camera.viewMatrix[2][1] < 0) {
            moveDir += u * (movement_lr);
            moveDir -= v * (movement_ud);
        }
        else {
            moveDir += u * (movement_lr);
            moveDir += v * (movement_ud);
        }


        if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {

            //camera.decomposeView(camera.getView());
            //std::cout << glm::to_string(camera.rotation_d) << '\n';
            for (auto& gameobject : gameObjects) {
                if (gameobject.model->getModelType() != MODEL_TYPE::MODEL_TYPE_AXIS) {
                    gameobject.transform.translation += scale * 0.01f * glm::normalize(moveDir);
                    //camera.viewMatrix = glm::translate(camera.getView(), moveSpeed * dt * moveDir);
                }
            }
        }

        xpos_prev = mouse_position.currentPosition.x();
        ypos_prev = mouse_position.currentPosition.y();
    }

    if (mouse_position.scroll.y() != 0) {
        float scale_mouse = 1.0f + 0.1f * (mouse_position.scroll.y() / 120);
        camera.viewMatrix = glm::scale(camera.getView(), glm::vec3(scale_mouse, scale_mouse, scale_mouse));
        mouse_position.scroll.setY(0);
    }
}









/*
void KeyboardMovementController::moveCamera(
        Qt::Key key, float dt, LveCamera& camera,
        float render_scale) {

    qDebug()<< "Pressed Key @ KeyboardMovementController::moveCamera : " << key;

    float scale = 0.0f;
    float rotate_lr = 0.0f;
    float rotate_ud = 0.0f;
    float obj_scale = 2.0f / render_scale;

    if (key == camera_move_key.moveForward) scale = 1.0f + 0.01f * obj_scale;
    if (key == camera_move_key.moveBackward) scale = 1.0f - 0.01f * obj_scale;
    if (key == camera_move_key.rotateRight) rotate_lr = -0.1f * obj_scale;
    if (key == camera_move_key.rotateLeft) rotate_lr = 0.1f * obj_scale;
    if (key == camera_move_key.rotateForward) rotate_ud = -0.1f * obj_scale;
    if (key == camera_move_key.rotateBackward) rotate_ud = 0.1f * obj_scale;
    //if (glfwGetMouseButton(window, camera_move_key.mouseLeft) == GLFW_PRESS) scale = 1.0f + 0.0001f * obj_scale;
    //if (glfwGetMouseButton(window, camera_move_key.mouseRight) == GLFW_PRESS) scale = 1.0f - 0.0001f * obj_scale;

    if (glm::dot(rotate_lr, rotate_lr) > std::numeric_limits<float>::epsilon()) {
        camera.viewMatrix = glm::rotate(camera.getView(), glm::radians(rotate_lr), glm::vec3(0.0f, 0.0f, 1.0f));
        //std::cout << glm::to_string(camera.rotation_d) << '\n';
    }

    glm::vec3 direction{ camera.viewMatrix[0][2], camera.viewMatrix[1][2], 0 };
    const glm::vec3 u{ glm::normalize(glm::cross(direction, glm::normalize(glm::cross(direction, glm::vec3{0,1,0})))) };
    const glm::vec3 v{ glm::normalize(glm::cross(u,glm::vec3{0,0,1})) };

    if (glm::dot(rotate_ud, rotate_ud) > std::numeric_limits<float>::epsilon()) {

        camera.viewMatrix = glm::rotate(camera.getView(), glm::radians(rotate_ud), u);
    }

    if (glm::dot(scale, scale) > std::numeric_limits<float>::epsilon()) {
        camera.viewMatrix = glm::scale(camera.getView(), glm::vec3(scale, scale, scale));
    }
}


void KeyboardMovementController::visibleSetting(GLFWwindow* window, std::vector<LveGameObject>& gameObjects) {
    MODEL_TYPE target_model_type = MODEL_TYPE::MODEL_TYPE_DEFAULT;
    bool opacity_mode = false;
    double opacity = 0.0;
    if (glfwGetKey(window, model_visible_set.layout_key) == GLFW_PRESS) model_visible_set.layout_key_pressed = true;
    else if (glfwGetKey(window, model_visible_set.resistor_key) == GLFW_PRESS) model_visible_set.resistor_key_pressed = true;
    else if (glfwGetKey(window, model_visible_set.capacitor_key) == GLFW_PRESS) model_visible_set.capacitor_key_pressed = true;
    else if (glfwGetKey(window, model_visible_set.axis_key) == GLFW_PRESS) model_visible_set.axis_key_pressed = true;
    else if (glfwGetKey(window, model_visible_set.opacity_plus_key) == GLFW_PRESS) model_visible_set.opacity_plus_key_pressed = true;
    else if (glfwGetKey(window, model_visible_set.opacity_minus_key) == GLFW_PRESS) model_visible_set.opacity_minus_key_pressed = true;

    if (glfwGetKey(window, model_visible_set.layout_key) == GLFW_RELEASE && model_visible_set.layout_key_pressed) {
        model_visible_set.layout_key_pressed = false;
        target_model_type = MODEL_TYPE::MODEL_TYPE_LAYOUT;
    }
    else if (glfwGetKey(window, model_visible_set.resistor_key) == GLFW_RELEASE && model_visible_set.resistor_key_pressed) {
        model_visible_set.resistor_key_pressed = false;
        target_model_type = MODEL_TYPE::MODEL_TYPE_PEX_RESISTOR;
    }
    else if (glfwGetKey(window, model_visible_set.capacitor_key) == GLFW_RELEASE && model_visible_set.capacitor_key_pressed) {
        model_visible_set.capacitor_key_pressed = false;
        target_model_type = MODEL_TYPE::MODEL_TYPE_PEX_CAPACITOR;
    }
    else if (glfwGetKey(window, model_visible_set.axis_key) == GLFW_RELEASE && model_visible_set.axis_key_pressed) {
        model_visible_set.axis_key_pressed = false;
        target_model_type = MODEL_TYPE::MODEL_TYPE_AXIS;
    }
    else if (glfwGetKey(window, model_visible_set.opacity_plus_key) == GLFW_RELEASE && model_visible_set.opacity_plus_key_pressed) {
        model_visible_set.opacity_plus_key_pressed = false;
        opacity_mode = true;
        opacity = 0.1f;
        target_model_type = MODEL_TYPE::MODEL_TYPE_LAYOUT;
    }
    else if (glfwGetKey(window, model_visible_set.opacity_minus_key) == GLFW_RELEASE && model_visible_set.opacity_minus_key_pressed) {
        model_visible_set.opacity_minus_key_pressed = false;
        opacity_mode = true;
        opacity = -0.1f;
        target_model_type = MODEL_TYPE::MODEL_TYPE_LAYOUT;
    }

    if (target_model_type != MODEL_TYPE::MODEL_TYPE_DEFAULT) {
        for (auto& obj : gameObjects) {
            if (obj.model->getModelType() == target_model_type) {
                if (opacity_mode) obj.model->updateOpacity(opacity);
                else obj.model->toggleVisible();
            }
        }
    }
}

void KeyboardMovementController::moveCamera(GLFWwindow* window, float dt, LveCamera& camera, std::vector<LveGameObject>& gameObjects) {

    float scale = 0.0f;
    float rotate_lr = 0.0f;
    float rotate_ud = 0.0f;
    float obj_scale = 2.0f / gameObjects.begin()->transform.scale.x;

    if (glfwGetKey(window, camera_move_key.moveForward) == GLFW_PRESS) scale = 1.0f + 0.0001f * obj_scale;
    if (glfwGetKey(window, camera_move_key.moveBackward) == GLFW_PRESS) scale = 1.0f - 0.0001f * obj_scale;
    if (glfwGetKey(window, camera_move_key.moveRight) == GLFW_PRESS) rotate_lr = -0.001f * obj_scale;
    if (glfwGetKey(window, camera_move_key.moveLeft) == GLFW_PRESS) rotate_lr = 0.001f * obj_scale;
    if (glfwGetKey(window, camera_move_key.moveUp) == GLFW_PRESS) rotate_ud = -0.001f * obj_scale;
    if (glfwGetKey(window, camera_move_key.moveDown) == GLFW_PRESS) rotate_ud = 0.001f * obj_scale;
    //if (glfwGetMouseButton(window, camera_move_key.mouseLeft) == GLFW_PRESS) scale = 1.0f + 0.0001f * obj_scale;
    //if (glfwGetMouseButton(window, camera_move_key.mouseRight) == GLFW_PRESS) scale = 1.0f - 0.0001f * obj_scale;

    if (glm::dot(rotate_lr, rotate_lr) > std::numeric_limits<float>::epsilon()) {
        camera.viewMatrix = glm::rotate(camera.getView(), glm::radians(rotate_lr), glm::vec3(0.0f, 0.0f, 1.0f));
        //std::cout << glm::to_string(camera.rotation_d) << '\n';
    }

    glm::vec3 direction{ camera.viewMatrix[0][2], camera.viewMatrix[1][2], 0 };
    const glm::vec3 u{ glm::normalize(glm::cross(direction, glm::normalize(glm::cross(direction, glm::vec3{0,1,0})))) };
    const glm::vec3 v{ glm::normalize(glm::cross(u,glm::vec3{0,0,1})) };

    if (glm::dot(rotate_ud, rotate_ud) > std::numeric_limits<float>::epsilon()) {

        camera.viewMatrix = glm::rotate(camera.getView(), glm::radians(rotate_ud), u);
    }

    if (glm::dot(scale, scale) > std::numeric_limits<float>::epsilon()) {
        camera.viewMatrix = glm::scale(camera.getView(), glm::vec3(scale, scale, scale));
    }


    glm::vec3 moveDir{ 0.f };
    camera.decomposeView(camera.getView());
    if (camera.rotation_d.x * camera.rotation_d.y > 0) {
        if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) moveDir -= u;
        if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) moveDir += u;
        if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) moveDir += v;
        if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) moveDir -= v;
    }
    else {
        if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) moveDir += u;
        if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) moveDir -= u;
        if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) moveDir -= v;
        if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) moveDir += v;
    }


    if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
        //camera.decomposeView(camera.getView());
        //std::cout << glm::to_string(camera.rotation_d) << '\n';
        for (auto& gameobject : gameObjects) {
            if (gameobject.model->getModelType() != MODEL_TYPE::MODEL_TYPE_AXIS) {
                gameobject.transform.translation += obj_scale * moveSpeed * dt * glm::normalize(moveDir);
                //camera.viewMatrix = glm::translate(camera.getView(), moveSpeed * dt * moveDir);
            }
        }

    }
}

void KeyboardMovementController::moveInPlaneXZ(
        GLFWwindow* window, float dt, std::vector<LveGameObject>& gameObjects) {
    glm::vec3 rotate{ 0 };

    //std::cout << "moveInPlaneXZ\n";
    //if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1.f;
    //if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.f;
    //if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x += 1.f;
    //if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1.f;
    //if (glfwGetMouseButton(window, keys.mouseLeft) == GLFW_PRESS) {
    //    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //
    //    int width, height;
    //    glfwGetWindowSize(window, &width, &height);
    //
    //
    //    oldXPos = mouseX, oldYPos = mouseY;
    //
    //    glfwGetCursorPos(window, &mouseX, &mouseY);
    //
    //    float xDelta, yDelta;
    //    xDelta = (mouseX - oldXPos) / width;
    //    yDelta = (mouseY - oldYPos) / height;
    //
    //    rotate.y -= xDelta * 1000;
    //    rotate.x += yDelta * 1000 * height / width;
    //
    //    // std::cout <<"old" << oldXPos << " " << oldYPos << "\n";
    //    // std::cout << mouseX << " " << mouseY << "\n";
    //}
    //if (glfwGetMouseButton(window, keys.mouseLeft) == GLFW_RELEASE) {
    //    glfwGetCursorPos(window, &mouseX, &mouseY);
    //    oldXPos = mouseX, oldYPos = mouseY;
    //};

    //if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
    //
    //    gameObject.transform.rotation += lookSpeed * dt * rotate;
    //}

    // limit pitch values between about +/- 85ish degrees
    //gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
    //gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

    float yaw = 20.f;
    const glm::vec3 forwardDir{ 0.f, -1.f, 0.f };
    const glm::vec3 rightDir{ 1.f, 0.f, 0.f };
    const glm::vec3 upDir{ 0.f, -1.f, 0.f };

    glm::vec3 moveDir{ 0.f };
    if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
    if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
    if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) moveDir += rightDir;
    if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) moveDir -= rightDir;
    if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) moveDir += forwardDir;
    if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) moveDir -= forwardDir;

    if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
        for (auto& gameobject : gameObjects) {
            if (gameobject.model->getModelType() == MODEL_TYPE::MODEL_TYPE_AXIS) {
                //gameobject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
            }


        }

    }
}
*/
