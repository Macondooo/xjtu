
cmake_minimum_required(VERSION 3.16)

project(autotest2 VERSION 1.0
                  DESCRIPTION "Auto test for lab 2 (Loop Subdivision)"
                  LANGUAGES CXX
)

set(SOURCES_SCOTTY3D_LIB
    "../../src/lib/bbox.h"
    "../../src/lib/line.h"
    "../../src/lib/log.h"
    "../../src/lib/mat4.h"
    "../../src/lib/mathlib.h"
    "../../src/lib/plane.h"
    "../../src/lib/quat.h"
    "../../src/lib/ray.h"
    "../../src/lib/vec2.h"
    "../../src/lib/vec3.h"
    "../../src/lib/vec4.h"
)
set(MODIFIED_SOURCE
    "halfedge.h"
    "halfedge.cpp"
    "meshedit.cpp"
    "utils.h"
    "utils.cpp"
    "test_main.cpp"
)
set(AUTOTEST2_SOURCE
    ${SOURCES_SCOTTY3D_LIB}
    ${MODIFIED_SOURCE}
)

add_executable(autotest2 ${AUTOTEST2_SOURCE})
set_target_properties(autotest2 PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)
file(COPY ${PROJECT_SOURCE_DIR}/std DESTINATION ${PROJECT_BINARY_DIR})

if(MSVC)
    target_compile_options(autotest2 PRIVATE /MP /W4 /WX /wd4996 /wd4201 /wd4840 /wd4100 /wd4505 /fp:fast)
else()
    target_compile_options(autotest2 PRIVATE -Wall -Wextra -Werror -Wno-reorder -Wno-unused-function -Wno-unused-parameter)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(autotest2 PRIVATE -fno-omit-frame-pointer -Wno-bitwise-instead-of-logical)
endif()
