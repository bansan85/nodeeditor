find_package(TBB REQUIRED)

set(CALC_SOURCE_FILES
  main.cpp
  MathOperationDataModel.cpp
  NumberDisplayDataModel.cpp
  NumberSourceDataModel.cpp
  TbbDataModel.cpp
)

set(CALC_HEADER_FILES
  AdditionModel.hpp
  DecimalData.hpp
  DivisionModel.hpp
  MathOperationDataModel.hpp
  MultiplicationModel.hpp
  NumberDisplayDataModel.hpp
  NumberSourceDataModel.hpp
  SubtractionModel.hpp
  TbbDataModel.hpp
  Helpers.hpp
)

add_executable(calculator_tbb
  ${CALC_SOURCE_FILES}
  ${CALC_HEADER_FILES}
)

target_link_libraries(calculator_tbb QtNodes TBB::tbb)
target_compile_definitions(calculator_tbb PRIVATE QT_NO_KEYWORDS)

add_definitions(/MP)

add_custom_command(
  TARGET calculator_tbb
  POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E env VCINSTALLDIR=${CMAKE_GENERATOR_INSTANCE}/VC
    ${WINDEPLOYQT_EXECUTABLE} --pdb
    "$<TARGET_FILE:calculator_tbb>")
