#pragma once

#include <QKeyEvent>
#include <QMouseEvent>
#include "lve_game_object.hpp"
#include "lve_window.hpp"
#include "lve_camera.hpp"
#include "PEXResistorModel.h"
//#include "defineParam.h"

#include "ToyCAD/Src/toycad_object.h"



class KeyboardMovementController {
public:
    struct ModelVisibleSet {
        int resistor_key = Qt::Key_R;
        int capacitor_key = Qt::Key_C;
        int resistor_up_key = Qt::Key_M;
        int resistor_down_key = Qt::Key_N;
        int resistor_horizontal_key = Qt::Key_Comma;
        int resistor_vertical_key = Qt::Key_Period;
        bool resistor_up_key_pressed = false;
        bool resistor_down_key_pressed = false;
        bool resistor_horizontal_key_pressed = false;
        bool resistor_vertical_key_pressed = false;
    };

    struct CameraMoveKeyMappings {
        int rotateLeft = Qt::Key_A;
        int rotateRight = Qt::Key_D;
        int moveForward = Qt::Key_W;
        int moveBackward = Qt::Key_S;
        int rotateForward = Qt::Key_E;
        int rotateBackward = Qt::Key_Q;
        int moveUp = Qt::Key_Up;
        int moveDown= Qt::Key_Down;
        int moveLeft= Qt::Key_Left;
        int moveRight= Qt::Key_Right;
        int toggleDemension= Qt::Key_T;
        Qt::MouseButtons mouseLeft = Qt::LeftButton;
        Qt::MouseButtons mouseRight = Qt::RightButton;
        Qt::MouseButtons mouseMiddle = Qt::MiddleButton;
    };

    struct CameraMovingContinousFlags {
        bool rotateLeft = false;
        bool rotateRight = false;
        bool moveForward = false;
        bool moveBackward = false;
        bool rotateForward = false;
        bool rotateBackward = false;
        bool moveUp = false;
        bool moveDown= false;
        bool moveLeft= false;
        bool moveRight= false;
        bool toggleDemension = false;
        bool toggleDemensionXY = false;
        bool mouseLeft= false;
        bool mouseRight= false;
        bool mouseMiddle= false;
        bool demoTrigger= false;
        bool demoInitLayer= false;
        bool tiltOn=false;
        bool layerOn=false;
        bool wave1 = false;
        bool wave2 = false;
        bool wave3 = false;
    };

    struct MousePosition {
        QPoint pressPosition;
        QPoint releasePosition;
        QPoint currentPosition;
        QPoint scroll;
        QPoint scrollClicked;
    };

    void visibleSetting(std::vector<LveGameObject>& gameObjects, Qt::Key key);
    ModelVisibleSet model_visible_set;
    void moveCamera(Qt::Key key, float dt, LveCamera& camera, float render_scale);
    CameraMoveKeyMappings camera_move_key;
    MousePosition mouse_position;

    void moveCamera(float dt, LveCamera& camera, float render_scale,
                    std::vector<LveGameObject>& gameObjects, LveWindow* lvewindow,
                    std::vector<ToyCADObject>* toycad_objects = nullptr);
    void moveCameraMouse(LveCamera& camera, float render_scale,
                         std::vector<LveGameObject>& gameObjects,LveWindow* lvewindow,
                         std::vector<ToyCADObject>* toycad_objects = nullptr);
    void setPerspective(LveCamera& camera, float aspect);
    void moveKeyPressed(Qt::Key key);
    void moveKeyReleased(Qt::Key key);
    void moveButtonPressed(Qt::MouseButtons buttons);
    void moveButtonReleased(Qt::MouseButtons buttons);
    void setAllMoveFlagOff();
    void setMouseFlagOff();
    void setMousePosition(QPoint position, int mode);
    void setMouseScroll(QPoint position);
    void PEXSetting(std::shared_ptr< PEXResistorModel>& pexResistorModel, Qt::Key key);
    void triggerDemo();
    //void translateLayerPosition(std::vector<LveGameObject>& gameObjects);
    void onDemo(float dt, LveCamera& camera, float render_scale,std::vector<LveGameObject>& gameObjects, LveWindow* lvewindow);
    CameraMovingContinousFlags camera_moving_flag;
    POS_MONITORING monitor;

    glm::vec3 translationOnPressed;


    float moveSpeed{ 0.05f };
    float lookSpeed{ 1.5f };
    int xpos_prev;
    int ypos_prev;
    float perspectiveNear{ 0.1f };
    int demoTime{0};
    int demoCheckPoint{0};


};

