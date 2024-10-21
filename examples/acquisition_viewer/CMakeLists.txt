set(ACQ_SOURCE_FILES main.cpp AcqModel.cpp)

set(ACQ_HEADER_FILES AcqData.hpp AcqModel.hpp)

add_executable(acquisition_viewer ${ACQ_SOURCE_FILES} ${ACQ_HEADER_FILES}
                                  CMakeLists.txt)

target_link_libraries(acquisition_viewer QtNodes)
