file(GLOB_RECURSE CPPS  ./*.cpp )
file(GLOB_RECURSE HPPS  ./*.hpp )

add_executable(text ${CPPS} ${HPPS})

target_link_libraries(text QtNodes)

if(WIN32)
  add_custom_command(
    TARGET text
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E env VCINSTALLDIR=${CMAKE_GENERATOR_INSTANCE}/VC
            ${WINDEPLOYQT_EXECUTABLE} --pdb "$<TARGET_FILE:text>")
endif()
