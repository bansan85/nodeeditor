file(GLOB_RECURSE CPPS  ./*.cpp )

add_executable(lock_nodes_and_connections ${CPPS})

target_link_libraries(lock_nodes_and_connections QtNodes)

if(WIN32)
  add_custom_command(
    TARGET lock_nodes_and_connections
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E env VCINSTALLDIR=${CMAKE_GENERATOR_INSTANCE}/VC
      ${WINDEPLOYQT_EXECUTABLE} --pdb
      "$<TARGET_FILE:lock_nodes_and_connections>")
endif()
