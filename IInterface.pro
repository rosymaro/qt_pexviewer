QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Rendering/Src/LayoutData.cpp \
    Rendering/Src/LayoutModel.cpp \
    Rendering/Src/PEXCapacitorModel.cpp \
    Rendering/Src/PEXData.cpp \
    Rendering/Src/PEXResistorModel.cpp \
    Rendering/Src/dtaorendersystem.cpp \
    Rendering/Src/keyboard_movement_controller.cpp \
    Rendering/Src/lve_camera.cpp \
    Rendering/Src/lve_device.cpp \
    Rendering/Src/lve_model.cpp \
    Rendering/Src/lve_pipeline.cpp \
    Rendering/Src/lve_window.cpp \
    Rendering/Src/simple_render_system.cpp \
    all_data.cpp \
    filedb.cpp \
    formhier.cpp \
    forminfo.cpp \
    formlayer.cpp \
    formmap.cpp \
    formtop.cpp \
    main.cpp \
    mainwindow.cpp \
    trianglerenderer.cpp

HEADERS += \
    Rendering/Src/LayoutModel.h \
    Rendering/Src/LayoutPEXData.h \
    Rendering/Src/PEXCapacitorModel.h \
    Rendering/Src/PEXResistorModel.h \
    Rendering/Src/dtaorendersystem.h \
    Rendering/Src/keyboard_movement_controller.hpp \
    Rendering/Src/lve_camera.hpp \
    Rendering/Src/lve_device.hpp \
    Rendering/Src/lve_game_object.hpp \
    Rendering/Src/lve_model.hpp \
    Rendering/Src/lve_pipeline.hpp \
    Rendering/Src/lve_window.hpp \
    Rendering/Src/rapidcsv.h \
    Rendering/Src/simple_render_system.hpp \
    all_data.h \
    filedb.h \
    formhier.h \
    forminfo.h \
    formlayer.h \
    formmap.h \
    formtop.h \
    mainwindow.h \
    mainwindow.h \
    trianglerenderer.h

FORMS += \
    formhier.ui \
    forminfo.ui \
    formlayer.ui \
    formmap.ui \
    formtop.ui \
    mainwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    Resources.qrc

DISTFILES += \
    Rendering/Data/MVP_PEX_RC_r1_0130.csv \
    Rendering/Data/MVP_PEX_RC_r1_0130_0208version.csv \
    Rendering/Data/big_test_espin.csv \
    Rendering/Data/layout_input_data.csv \
    Rendering/Data/layout_input_data_old_format.csv \
    Rendering/Data/pex_cap_test.csv \
    Rendering/Data/pex_res_test.csv \
    Rendering/Data/pex_res_test_beol_only.csv \
    Rendering/Data/pex_res_test_simple.csv \
    Rendering/Data/test_espin.csv \
    Rendering/shaders/compile.bat \
    Rendering/shaders/simple_shader.frag \
    Rendering/shaders/simple_shader.frag.spv \
    Rendering/shaders/simple_shader.frag_edge.spv \
    Rendering/shaders/simple_shader.frag_pex.spv \
    Rendering/shaders/simple_shader.vert \
    Rendering/shaders/simple_shader.vert.spv \
    Rendering/shaders/simple_shader.vert_edge.spv \
    Rendering/shaders/simple_shader.vert_pex.spv \
    Rendering/shaders/simple_shader_edge.frag \
    Rendering/shaders/simple_shader_edge.vert \
    Rendering/shaders/simple_shader_pex.frag \
    Rendering/shaders/simple_shader_pex.vert

unix:!macx|win32: LIBS += -L$$PWD/../../Library/VulkanSDK/1.3.239.0/Lib/ -lvulkan-1

INCLUDEPATH += $$PWD/../../Library/VulkanSDK/1.3.239.0/Lib
DEPENDPATH += $$PWD/../../Library/VulkanSDK/1.3.239.0/Lib

INCLUDEPATH += $$PWD/../../Library/glm-0.9.9.8
