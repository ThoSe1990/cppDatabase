

set(target server)


file(GLOB source ${PROJECT_ROOT_DIR}/server/*.cpp ${PROJECT_ROOT_DIR}/proto/*.cc)

add_executable( ${target} ${source} )

target_include_directories(${target} PUBLIC
    ${PROJECT_ROOT_DIR}/server
    ${PROJECT_ROOT_DIR}/proto
)


target_link_libraries(${target} PUBLIC 
    ${Protobuf_LIBRARIES}
    Boost::program_options
    pthread
)
