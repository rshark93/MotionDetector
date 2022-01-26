set(MAIN_TARGET_NAME ${PROJECT_NAME}Main)
set(srcPath ../src/)
set(incPath ../include/)

# path to OpenCV libs
if(WIN32)
    set(OpenCV_DIR ../../opencv/build)
endif()

find_package(OpenCV REQUIRED)

add_executable( ${MAIN_TARGET_NAME} 
    "main.cpp" "main.h"
    ${srcPath}MotionDetector.cpp  ${incPath}MotionDetector.h
)

target_include_directories(${MAIN_TARGET_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS} "../src" "../include")
target_link_libraries(${MAIN_TARGET_NAME} ${OpenCV_LIBS})