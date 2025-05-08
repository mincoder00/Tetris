file(GLOB_RECURSE APP_SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(app ${APP_SRC_FILES})

target_link_libraries(app PRIVATE Tetris_cpp ${CURSES_LIBRARIES})
target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_compile_options(app PRIVATE -Wall -Werror)
target_compile_features(app PRIVATE cxx_std_17)