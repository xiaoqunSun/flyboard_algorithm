
cmake_minimum_required(VERSION 3.18)

set(APP_NAME algorithm)
project (${APP_NAME})

#add_executable(${APP_NAME} test.cc Bezier.cc Bezier.h Vec2.h)
add_executable(${APP_NAME} main.cc Bezier.cc Bezier.h  Vec2.h)


if(EMSCRIPTEN)
    set(USE_FLAGS " -Oz ")
    #set(USE_FLAGS " -Oz -s MAIN_MODULE=1  ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
endif()

if(EMSCRIPTEN)
    set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS "-s EXPORT_NAME=${APP_NAME}  -s MODULARIZE=1 -s EXPORTED_FUNCTIONS=[_get_version,_bezier_intersect,_free_pointer,_malloc,_free]")

    #set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS "-s EXPORT_NAME=${APP_NAME} -s MODULARIZE=1")
    #set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS "-s EXPORT_NAME=${APP_NAME} -s EXPORTED_FUNCTIONS=[_get_version,_bezier_intersect,_free_pointer,_malloc,_free]") 
endif()


