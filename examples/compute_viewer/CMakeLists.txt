set(COMPUTE_SOURCE_FILES main.cpp InputModel.cpp DecimationModel.cpp
                         DecimationWidget.cpp)

set(COMPUTE_HEADER_FILES InputData.hpp InputModel.hpp DecimationModel.hpp
                         DecimationData.hpp DecimationWidget.hpp)

add_executable(compute_viewer ${COMPUTE_SOURCE_FILES} ${COMPUTE_HEADER_FILES}
                              CMakeLists.txt)

target_link_libraries(compute_viewer QtNodes)
