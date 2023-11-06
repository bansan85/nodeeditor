add_executable(
  dynamic_ports
  "CMakeLists.txt"
  "DynamicPortsModel.cpp"
  "DynamicPortsModel.hpp"
  "main.cpp"
  "PortAddRemoveWidget.cpp"
  "PortAddRemoveWidget.hpp")

target_link_libraries(dynamic_ports QtNodes)
