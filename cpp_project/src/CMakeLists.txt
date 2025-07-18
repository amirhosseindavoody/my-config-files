add_subdirectory(utils)
add_subdirectory(testing)
add_subdirectory(optimizer)

add_library(main_lib STATIC main_lib.cc)
target_link_libraries(main_lib PUBLIC ${THIRDPARTY_LIBS} logging nelder_mead)

# Main executable
add_executable(main main.cc)
target_link_libraries(main PRIVATE main_lib)

