

set(target test)


file(GLOB source 
${PROJECT_ROOT_DIR}/test/*.cpp
${PROJECT_ROOT_DIR}/proto/*.cc
)

add_executable( ${target} ${source} )


target_include_directories(${target} PUBLIC
${PROJECT_ROOT_DIR}/proto
${PROJECT_ROOT_DIR}/server
)

target_link_libraries(${target} 
    ${Protobuf_LIBRARIES}
)

