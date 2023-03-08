#pragma once

#include <QKeyEvent>

#include "lve_game_object.hpp"
#include "lve_window.hpp"
#include "lve_camera.hpp"


class KeyboardMovementController {
public:
    struct CameraMoveKeyMappings {
        int moveLeft = Qt::Key_A;
        int moveRight = Qt::Key_D;
        int moveForward = Qt::Key_W;
        int moveBackward = Qt::Key_S;
        int moveUp = Qt::Key_E;
        int moveDown = Qt::Key_Q;
        //int lookLeft = GLFW_KEY_LEFT;
        //int lookRight = GLFW_KEY_RIGHT;
        //int lookUp = GLFW_KEY_UP;
        //int lookDown = GLFW_KEY_DOWN;
        //int mouseLeft = GLFW_MOUSE_BUTTON_LEFT;
        //int mouseRight = GLFW_MOUSE_BUTTON_RIGHT;
    };

    struct CameraMovingContinousFlags {
        bool moveLeft = false;
        bool moveRight = false;
        bool moveForward = false;
        bool moveBackward = false;
        bool moveUp = false;
        bool moveDown = false;
        //int lookLeft = GLFW_KEY_LEFT;
        //int lookRight = GLFW_KEY_RIGHT;
        //int lookUp = GLFW_KEY_UP;
        //int lookDown = GLFW_KEY_DOWN;
        //int mouseLeft = GLFW_MOUSE_BUTTON_LEFT;
        //int mouseRight = GLFW_MOUSE_BUTTON_RIGHT;
    };

    void moveCamera(Qt::Key key, float dt, LveCamera& camera, float render_scale);
    CameraMoveKeyMappings camera_move_key;

    void moveCamera(float dt, LveCamera& camera, float render_scale);
    void moveKeyPressed(Qt::Key key);
    void moveKeyReleased(Qt::Key key);
    void setAllMoveFlagOff();
    CameraMovingContinousFlags camera_moving_flag;


    /*
    struct KeyMappings {
        int moveLeft = Qt::Key_A;//'GLFW_KEY_A;
        int moveRight = Qt::Key_D;
        int moveForward = Qt::Key_W;
        int moveBackward = Qt::Key_S;
        int moveUp = Qt::Key_E;
        int moveDown = Qt::Key_Q;
        int lookLeft = Qt::Key_Left;
        int lookRight = Qt::Key_Right;
        int lookUp = Qt::Key_Up;
        int lookDown = Qt::Key_Down;
        //int mouseLeft = GLFW_MOUSE_BUTTON_LEFT;
        //int mouseRight = GLFW_MOUSE_BUTTON_RIGHT;
    };

    struct ModelVisibleSet {

        int edge_key = GLFW_KEY_E;
        int layout_key = GLFW_KEY_L;
        int resistor_key = GLFW_KEY_R;
        int capacitor_key = GLFW_KEY_C;
        int axis_key = GLFW_KEY_X;
        int vertical_resistor_key = GLFW_KEY_V;
        int horizontal_resistor_key = GLFW_KEY_H;
        int opacity_plus_key = GLFW_KEY_O;
        int opacity_minus_key = GLFW_KEY_P;

        bool edge_key_pressed = false;
        bool layout_key_pressed = false;
        bool resistor_key_pressed = false;
        bool capacitor_key_pressed = false;
        bool axis_key_pressed = false;
        bool vertical_resistor_key_pressed = false;
        bool horizontal_resistor_key_pressed = false;
        bool opacity_plus_key_pressed = false;
        bool opacity_minus_key_pressed = false;

    };

    void moveInPlaneXZ(GLFWwindow* window, float dt, std::vector<LveGameObject>& gameObjects);

    void moveCamera(GLFWwindow* window, float dt, LveCamera& camera, std::vector<LveGameObject>& gameObjects);
    CameraMoveKeyMappings camera_move_key;

    void visibleSetting(GLFWwindow* window, std::vector<LveGameObject>& gameObjects);
    ModelVisibleSet model_visible_set;

    void initMouse(GLFWwindow* window);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    KeyMappings keys{};
    double mouseX;
    double mouseY;
    double oldXPos;
    double oldYPos;
    float moveSpeed{ 3.f };
    float lookSpeed{ 1.5f };

    //KeyMappings keys{};
*/
    float moveSpeed{ 3.f };
    float lookSpeed{ 1.5f };
};

