 ### GLFW ###
set(GLFW_LIB_NAME "glfw")
set(GLFW_INC_PATH ${GLFW_LIB_NAME}/include)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

add_subdirectory(${GLFW_LIB_NAME})

### GLAD ###
set(GLAD_LIB_NAME "glad")
set(GLAD_SRC_PATH "${GLAD_LIB_NAME}/src")
set(GLAD_INC_PATH "${GLAD_LIB_NAME}/include")

add_library( ${GLAD_LIB_NAME}
    STATIC
        "${GLAD_SRC_PATH}/glad.c"
)

target_include_directories(${GLAD_LIB_NAME}
    PUBLIC
        "${GLAD_INC_PATH}"
)

### GLM ### wq
set(GLM_LIB_NAME "glm")
set(GLM_INC_PATH ${GLM_LIB_NAME}/glm)

add_library(${GLM_LIB_NAME} INTERFACE)
target_include_directories(${GLM_LIB_NAME}
    INTERFACE
        ${GLM_INC_PATH}
)

### imgui ### 
set(IMGUI_LIB_NAME "imgui")
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui")
set(IMGUI_SOURCES 
	"${IMGUI_DIR}/imgui.cpp"
	"${IMGUI_DIR}/imgui_demo.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/imgui_impl_opengl3.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp")
set(IMGUI_HEADERS 
	"${IMGUI_DIR}/imgui.cpp"
	"${IMGUI_DIR}/imconfig.h"
    "${IMGUI_DIR}/imgui_internal.h"
    "${IMGUI_DIR}/imgui_impl_glfw.h"
    "${IMGUI_DIR}/imgui_impl_opengl3.h"
    "${IMGUI_DIR}/imstd_rectpack.h"
    "${IMGUI_DIR}/imstb_textedit.h"
    "${IMGUI_DIR}/imstb_truetype.h")

add_library(${IMGUI_LIB_NAME} ${IMGUI_SOURCES})
target_include_directories(${IMGUI_LIB_NAME}
    PUBLIC
    ${IMGUI_HEADERS}
    ${GLAD_INC_PATH}
)

target_link_libraries(${IMGUI_LIB_NAME} ${GLFW_LIB_NAME})
