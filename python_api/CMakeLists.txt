

set(target pythonDatabase)


set(CMAKE_SHARED_MODULE_PREFIX "")

file(GLOB source 
${PROJECT_ROOT_DIR}/python_api/*.cpp
${PROJECT_ROOT_DIR}/client/*.cpp
${PROJECT_ROOT_DIR}/proto/*.cc
)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(Boost COMPONENTS python38 REQUIRED)

add_library( ${target} MODULE ${source} )

set_property(TARGET ${target} PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET ${target} PROPERTY C_VISIBILITY_PRESET hidden)
set_property(TARGET ${target} PROPERTY VISIBILITY_INLINES_HIDDEN ON)

target_include_directories(${target} PUBLIC
${PROJECT_ROOT_DIR}/proto
${PROJECT_ROOT_DIR}/client
${Python3_INCLUDE_DIRS}
)

target_link_libraries(${target} 
    ${Protobuf_LIBRARIES}
    Boost::python38
    ${Python3_LIBRARY}
)

install(TARGETS ${target} DESTINATION /usr/lib/python3.8)