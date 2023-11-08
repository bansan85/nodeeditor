file(GLOB_RECURSE CPPS  ./*.cpp )
file(GLOB_RECURSE HPPS  ./*.hpp )

add_executable(simple_graph_model ${CPPS} ${HPPS})

target_link_libraries(simple_graph_model QtNodes)

if(WIN32)
  add_custom_command(
    TARGET simple_graph_model
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E env VCINSTALLDIR=${CMAKE_GENERATOR_INSTANCE}/VC
            ${WINDEPLOYQT_EXECUTABLE} --pdb "$<TARGET_FILE:simple_graph_model>")
endif()
