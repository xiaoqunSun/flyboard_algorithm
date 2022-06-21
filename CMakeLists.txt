
set(APP_NAME algorithm)
project (${APP_NAME})

add_executable(${APP_NAME} Bezier.cc Bezier.h test.cc vector.h)


if(EMSCRIPTEN)
    set(USE_FLAGS " -Oz  -std=c++11 -s MAIN_MODULE=1 -s EXPORT_NAME=${APP_NAME} -s MODULARIZE=1 -s WASM=1  -s INITIAL_MEMORY=512MB  -s \"EXTRA_EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']\" ")
    
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
endif()


if(EMSCRIPTEN)

    set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS "-s DYNAMIC_EXECUTION=1 -s WASM=1")
endif()
