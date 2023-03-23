#include "keyboard_movement_controller.hpp"
#include "lve_window.hpp"

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
    camera_moving_flag.mouseMiddle = false;
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
    if (key == camera_move_key.toggleDemension) {
        camera_moving_flag.toggleDemension = true;
        camera_moving_flag.toggleDemensionXY = true;
    }


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
    if (camera_moving_flag.mouseMiddle){
        mouse_position.scrollClicked = scroll;
    } else {
        mouse_position.scroll = scroll;
    }
}

void KeyboardMovementController::moveButtonPressed(Qt::MouseButtons buttons) {
    setMouseFlagOff();
    if (buttons == camera_move_key.mouseLeft) camera_moving_flag.mouseLeft = true;
    if (buttons == camera_move_key.mouseRight) camera_moving_flag.mouseRight = true;
    if (buttons == camera_move_key.mouseMiddle) camera_moving_flag.mouseMiddle = true;


}

void KeyboardMovementController::moveButtonReleased(Qt::MouseButtons buttons) {
    setMouseFlagOff();
    if (buttons == camera_move_key.mouseLeft) camera_moving_flag.mouseLeft = true;
    if (buttons == camera_move_key.mouseRight) camera_moving_flag.mouseRight = true;
    if (buttons == camera_move_key.mouseMiddle) camera_moving_flag.mouseMiddle = true;
    //qDebug()<<
}

void KeyboardMovementController::moveCamera(
    float dt, LveCamera& camera, float render_scale, std::vector<LveGameObject>& gameObjects, LveWindow* lvewindow) {
    float scale = 0.0f;
    float rotate_lr = 0.0f;
    float rotate_ud = 0.0f;
    float rotate_toggle = 0.0f;
    float rotate_toggle_xy = 0.0f;
    float obj_scale = 2.0f / render_scale;

    if (camera_moving_flag.moveForward) scale = 1.0f + 0.01f * obj_scale;
    else if (camera_moving_flag.moveBackward) scale = 1.0f - 0.01f * obj_scale;

    if (camera_moving_flag.rotateRight) rotate_lr = -0.1f * obj_scale;
    else if (camera_moving_flag.rotateLeft) rotate_lr = 0.1f * obj_scale;

    if (camera_moving_flag.rotateForward ) rotate_ud = -0.1f * obj_scale;
    else if (camera_moving_flag.rotateBackward) rotate_ud = 0.1f * obj_scale;

    if (camera_moving_flag.toggleDemension && !camera.isperspective) {
        camera.setPerspectiveProjection(glm::radians(50.f), dt, 0.1f, 100.f);
        camera_moving_flag.toggleDemension = false;
    }
    else if (camera_moving_flag.toggleDemension)rotate_toggle = 0.1f;

    if (camera_moving_flag.toggleDemensionXY) rotate_toggle_xy = -0.1f;


    glm::vec3 right{camera.viewMatrix[0][0], camera.viewMatrix[1][0], camera.viewMatrix[2][0]};
    glm::vec3 up{camera.viewMatrix[0][1], camera.viewMatrix[1][1], camera.viewMatrix[2][1]};
    glm::vec3 forward{camera.viewMatrix[0][2], camera.viewMatrix[1][2], camera.viewMatrix[2][2]};

    const glm::vec3 v{ glm::normalize(glm::cross(right,glm::vec3{0,0,1})) };

    if (glm::dot(rotate_toggle, rotate_toggle) > std::numeric_limits<float>::epsilon()) {

        camera.decomposeView(camera.getView());
        camera.rotation_d = glm::conjugate(camera.rotation_d);

        // yaw / z
        double siny_cosp = 2 * (camera.rotation_d.w * camera.rotation_d.z + camera.rotation_d.x * camera.rotation_d.y);
        double cosy_cosp = 1 - 2 * (camera.rotation_d.y * camera.rotation_d.y + camera.rotation_d.z * camera.rotation_d.z);
        float rotation = (float)atan2(siny_cosp, cosy_cosp);
        float degree = rotation * (180/3.141592653589793238463);
        float tilt = asin(forward.z/(sqrt(forward.x*forward.x +forward.y*forward.y+forward.z*forward.z)));
        tilt = tilt *(180/3.141592653589793238463);

        if(!camera_moving_flag.toggleDemensionXY && camera_moving_flag.toggleDemension){
            if(tilt  > 89.5 ){
                camera.decomposeView(camera.getView());
                camera.setViewTarget(glm::vec3(0.0f,0.0f,1.0f),glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f));
                camera.viewMatrix = glm::scale(camera.getView(), glm::vec3(camera.scale_d.x, camera.scale_d.x, camera.scale_d.x));
                qDebug() << camera.rotation_d.z;
                camera_moving_flag.toggleDemension = false;
                camera.setOrthographicProjection(-1*dt*0.47,1*dt*0.47,-1*0.47,1*0.47,0.1f,100.f);
            } else {
                rotate_toggle=rotate_toggle * ((90 - tilt)/2 +1);
                camera.viewMatrix = glm::rotate(camera.getView(), glm::radians(rotate_toggle), right);
            }
        }


        if (camera_moving_flag.toggleDemensionXY) {
            if(up.z <0){
                qDebug() << degree;
                if( 180 - abs(degree) < 1 ){
                    camera.viewMatrix = glm::rotate(camera.getView(), glm::radians(degree-90), glm::vec3(0.0f, 0.0f, 1.0f));
                    camera_moving_flag.toggleDemensionXY = false;
                } else {
                    rotate_toggle_xy=rotate_toggle_xy * ((180 - abs(degree))/2 +1) *(180 - abs(degree))/(180 - abs(degree));
                    camera.viewMatrix = glm::rotate(camera.getView(), glm::radians(rotate_toggle_xy), glm::vec3(0.0f, 0.0f, 1.0f));
                }
            } else{
                if( abs(degree) < 1 ){
                    camera.viewMatrix = glm::rotate(camera.getView(), glm::radians( 0 - abs(degree)), glm::vec3(0.0f, 0.0f, 1.0f));
                    camera_moving_flag.toggleDemensionXY = false;
                } else {
                    rotate_toggle_xy=rotate_toggle_xy * (abs(degree)/2 +1) *abs(degree)/abs(degree);
                    camera.viewMatrix = glm::rotate(camera.getView(), glm::radians(rotate_toggle_xy), glm::vec3(0.0f, 0.0f, 1.0f));
                }
            }
        }

    }



    if (glm::dot(rotate_lr, rotate_lr) > std::numeric_limits<float>::epsilon()) {
        camera.viewMatrix = glm::rotate(camera.getView(), glm::radians(rotate_lr), glm::vec3(0.0f, 0.0f, 1.0f));
    }



    if (glm::dot(rotate_ud, rotate_ud) > std::numeric_limits<float>::epsilon()) {
        camera.viewMatrix = glm::rotate(camera.getView(), glm::radians(rotate_ud), right);

    }

    if (glm::dot(scale, scale) > std::numeric_limits<float>::epsilon()) {
        camera.viewMatrix = glm::scale(camera.getView(), glm::vec3(scale, scale, scale));

    }


    glm::vec3 moveDir{ 0.f };
    camera.decomposeView(camera.getView());

    if (camera_moving_flag.moveRight) moveDir += right;
    if (camera_moving_flag.moveLeft)  moveDir -= right;
    if (camera_moving_flag.moveUp)  moveDir -= v;
    if (camera_moving_flag.moveDown)  moveDir += v;



    if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
        //camera.decomposeView(camera.getView());
        //std::cout << glm::to_string(camera.rotation_d) << '\n';
        for (auto& gameobject : gameObjects) {
            if (gameobject.model->getModelType() != MODEL_TYPE::MODEL_TYPE_AXIS) {
                gameobject.transform.translation += obj_scale * moveSpeed  * glm::normalize(moveDir);
                //camera.viewMatrix = glm::translate(camera.getView(), moveSpeed * dt * moveDir);
            }
        }

    }
    forward = {camera.viewMatrix[0][2], camera.viewMatrix[1][2], camera.viewMatrix[2][2]};
    double tilt = asin(forward.z/(sqrt(forward.x*forward.x +forward.y*forward.y+forward.z*forward.z)))*(180/3.141592653589793238463);
    camera.decomposeView(camera.getView());
    camera.rotation_d = glm::conjugate(camera.rotation_d);
    // yaw / z
    double siny_cosp = 2 * (camera.rotation_d.w * camera.rotation_d.z + camera.rotation_d.x * camera.rotation_d.y);
    double cosy_cosp = 1 - 2 * (camera.rotation_d.y * camera.rotation_d.y + camera.rotation_d.z * camera.rotation_d.z);
    double rotation = (double)atan2(siny_cosp, cosy_cosp)*(180/3.141592653589793238463);

    monitor.tilt = tilt;
    monitor.rotation = rotation;
    monitor.zoom = camera.scale_d.x;
    QString funcName = "camera_position";
    emit lvewindow->signalInfoText(funcName,monitor);

}

void KeyboardMovementController::moveCameraMouse(LveCamera& camera, float render_scale, std::vector<LveGameObject>& gameObjects, LveWindow* lvewindow) {
    if (camera_moving_flag.mouseRight)
    {

        float rotate_lr = 0.0f;
        float rotate_ud = 0.0f;

        rotate_lr = mouse_position.currentPosition.x() - xpos_prev;
        rotate_ud = mouse_position.currentPosition.y() - ypos_prev;

        //glm::vec3 direction{ camera.viewMatrix[0][2], camera.viewMatrix[1][2], 0 };
        glm::vec3 right{camera.viewMatrix[0][0], camera.viewMatrix[1][0], camera.viewMatrix[2][0]};
        glm::vec3 up{camera.viewMatrix[0][1], camera.viewMatrix[1][1], camera.viewMatrix[2][1]};
        glm::vec3 forward{camera.viewMatrix[0][2], camera.viewMatrix[1][2], camera.viewMatrix[2][2]};


        if (glm::dot(rotate_ud, rotate_ud) > std::numeric_limits<float>::epsilon()) {
            if(glm::rotate(camera.getView(), glm::radians(rotate_ud*0.5f),right)[2][1] >= 0){
                camera.viewMatrix = glm::rotate(camera.getView(), glm::radians(rotate_ud*0.5f),right);
            }
        }

        if (glm::dot(rotate_lr, rotate_lr) > std::numeric_limits<float>::epsilon()) {
            camera.viewMatrix = glm::rotate(camera.getView(), glm::radians(rotate_lr*0.5f), glm::vec3(0.0f, 0.0f, 1.0f));
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
        camera.decomposeView(camera.getView());
        qDebug() << camera.scale_d.x<< camera.scale_d.y<< camera.scale_d.z;
    }

    glm::vec3 forward = {camera.viewMatrix[0][2], camera.viewMatrix[1][2], camera.viewMatrix[2][2]};
    float tilt = asin(forward.z/(sqrt(forward.x*forward.x +forward.y*forward.y+forward.z*forward.z)))*(180/3.141592653589793238463);
    camera.decomposeView(camera.getView());
    camera.rotation_d = glm::conjugate(camera.rotation_d);
    // yaw / z
    double siny_cosp = 2 * (camera.rotation_d.w * camera.rotation_d.z + camera.rotation_d.x * camera.rotation_d.y);
    double cosy_cosp = 1 - 2 * (camera.rotation_d.y * camera.rotation_d.y + camera.rotation_d.z * camera.rotation_d.z);
    float rotation = (float)atan2(siny_cosp, cosy_cosp)*(180/3.141592653589793238463);

    monitor.tilt = tilt;
    monitor.rotation = rotation;
    monitor.zoom = camera.scale_d.x;
    QString funcName = "camera_position";
    emit lvewindow->signalInfoText(funcName,monitor);
}

void KeyboardMovementController::setPerspective(LveCamera& camera, float aspect){
    if (mouse_position.scrollClicked.y() > 0) {
        if (perspectiveNear + 0.01  > 100.f){
            perspectiveNear = 100.f;

        } else {
            perspectiveNear=perspectiveNear +0.01;
        }
    } else if (mouse_position.scrollClicked.y() < 0) {
        if (perspectiveNear - 0.01  < 0.1f){
            perspectiveNear = 0.1f;

        }else {perspectiveNear=perspectiveNear -0.01;}
        }
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, perspectiveNear, 100.f);
        mouse_position.scrollClicked.setY(0);
}



