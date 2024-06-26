#include "AdditionModel.hpp"
#include "DecimalData.hpp"
#include "DivisionModel.hpp"
#include "MultiplicationModel.hpp"
#include "NumberDisplayDataModel.hpp"
#include "NumberSourceDataModel.hpp"
#include "SubtractionModel.hpp"
#include "TbbDataModel.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <oneapi/tbb/flow_graph.h>
#include <stdio.h>
#include <tuple>
#include <unordered_set>
#include <vector>
#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QObject>
#include <QPoint>
#include <QRect>
#include <QScreen>
#include <QVBoxLayout>
#include <QVariant>
#include <QWidget>
#include <QtNodes/ConnectionStyle>
#include <QtNodes/DataFlowGraphicsScene>
#include <QtNodes/GraphicsView>

#if defined(_WIN32) && !defined(NDEBUG)
#include <windows.h>
#endif

using QtNodes::ConnectionId;
using QtNodes::ConnectionStyle;
using QtNodes::DataFlowGraphicsScene;
using QtNodes::DataFlowGraphModel;
using QtNodes::GraphicsView;
using QtNodes::NodeDelegateModelRegistry;
using QtNodes::NodeId;

using oneapi::tbb::flow::broadcast_node;
using oneapi::tbb::flow::function_node;
using oneapi::tbb::flow::graph;
using oneapi::tbb::flow::input_port;
using oneapi::tbb::flow::join_node;
using oneapi::tbb::flow::make_edge;
using oneapi::tbb::flow::unlimited;

graph gra;

static std::shared_ptr<NodeDelegateModelRegistry> registerDataModels()
{
    auto ret = std::make_shared<NodeDelegateModelRegistry>();
    ret->registerModel<NumberSourceDataModel>("Sources");

    ret->registerModel<NumberDisplayDataModel>("Displays");

    ret->registerModel<AdditionModel>("Operators");

    ret->registerModel<SubtractionModel>("Operators");

    ret->registerModel<MultiplicationModel>("Operators");

    ret->registerModel<DivisionModel>("Operators");

    return ret;
}

static void setStyle()
{
    ConnectionStyle::setConnectionStyle(
        R"(
  {
    "ConnectionStyle": {
      "ConstructionColor": "gray",
      "NormalColor": "black",
      "SelectedColor": "gray",
      "SelectedHaloColor": "deepskyblue",
      "HoveredColor": "deepskyblue",

      "LineWidth": 3.0,
      "ConstructionLineWidth": 2.0,
      "PointDiameter": 10.0,

      "UseDataDefinedColors": true
    }
  }
  )");
}

int main(int argc, char *argv[])
{
#if defined(_WIN32) && !defined(NDEBUG)
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif

    QApplication app(argc, argv);

    setStyle();

    std::shared_ptr<NodeDelegateModelRegistry> registry = registerDataModels();

    QWidget mainWidget;

    auto menuBar = new QMenuBar();
    QMenu *menu = menuBar->addMenu("File");
    auto saveAction = menu->addAction("Save Scene");
    auto loadAction = menu->addAction("Load Scene");
    auto computeFakeTbbAction = menu->addAction("Compute TBB Fake");
    auto computeTbbAction = menu->addAction("Compute TBB");

    QVBoxLayout *l = new QVBoxLayout(&mainWidget);

    DataFlowGraphModel dataFlowGraphModel(registry);

    l->addWidget(menuBar);
    auto scene = new DataFlowGraphicsScene(dataFlowGraphModel, &mainWidget);

    auto view = new GraphicsView(scene);
    l->addWidget(view);
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);

    QObject::connect(saveAction, &QAction::triggered, scene, &DataFlowGraphicsScene::save);

    QObject::connect(loadAction, &QAction::triggered, scene, &DataFlowGraphicsScene::load);

    QObject::connect(computeFakeTbbAction, &QAction::triggered, [scene]() {
        tbb::flow::graph g;

        broadcast_node<float> buf1_node1(g);

        broadcast_node<float> buf2_node1(g);

        broadcast_node<float> buf1_node2(g);

        join_node<std::tuple<float, float>> input_node1(g);

        make_edge(buf1_node1, input_port<0>(input_node1));
        make_edge(buf2_node1, input_port<1>(input_node1));
        tbb::flow::function_node<std::tuple<float, float>, float>
            add_node(g, tbb::flow::unlimited, [](const std::tuple<float, float> &input) {
                float a = std::get<0>(input);
                float b = std::get<1>(input);
                return a + b;
            });

        make_edge(input_node1, add_node);

        broadcast_node<float> buf_out_for_result_add_node(g);
        make_edge(add_node, buf_out_for_result_add_node);

        join_node<std::tuple<float, float>> input_node2(g);
        make_edge(add_node, input_port<0>(input_node2));
        make_edge(buf1_node2, input_port<1>(input_node2));

        tbb::flow::function_node<std::tuple<float, float>, float>
            subtract_node(g, tbb::flow::unlimited, [](const std::tuple<float, float> &input) {
                float a = std::get<0>(input);
                float b = std::get<1>(input);
                return a - b;
            });

        make_edge(input_node2, subtract_node);

        broadcast_node<float> buf_out_for_result_subtract_node(g);
        make_edge(subtract_node, buf_out_for_result_subtract_node);

        broadcast_node<std::tuple<float, float>> buf_out_for_result_input_node2(g);
        make_edge(input_node2, buf_out_for_result_input_node2);

        buf1_node1.try_put(3.);
        buf2_node1.try_put(4.);
        buf1_node2.try_put(9.);

        g.wait_for_all();

        float add_node_result;
        float subtract_node_result;
        std::tuple<float, float> input_node2_result;

        if (buf_out_for_result_add_node.try_get(add_node_result)) {
            std::cout << "add_node == (" << add_node_result << ")\n";
        } else {
            std::cout << "add_node failure\n";
        }
        if (buf_out_for_result_input_node2.try_get(input_node2_result)) {
            std::cout << "join_node output == (" << std::get<0>(input_node2_result) << ","
                      << std::get<1>(input_node2_result) << ")\n";
        } else {
            std::cout << "join_node output failure\n";
        }
        if (buf_out_for_result_subtract_node.try_get(subtract_node_result)) {
            printf("join_node 2 output == (%f)\n", subtract_node_result);
        } else {
            std::cout << "join_node 2 output failure\n";
        }
    });

    QObject::connect(computeTbbAction, &QAction::triggered, [scene, &dataFlowGraphModel]() {
        tbb::flow::graph g;

        std::unordered_set<NodeId> allNodes = scene->graphModel().allNodeIds();
        std::unordered_set<NodeId> nodesRemaining = allNodes;
        while (!nodesRemaining.empty()) {
            std::unordered_set<QtNodes::NodeId> nodes = nodesRemaining;

            for (QtNodes::NodeId node : nodes) {
                bool skip = false;
                QVariant v = scene->graphModel().nodeData(node, QtNodes::NodeRole::Type);

                auto allConnectionIds = scene->graphModel().allConnectionIds(node);
                for (const QtNodes::ConnectionId &j : allConnectionIds) {
                    // Dependency on an output port. Don't care.
                    if (j.outNodeId == node) {
                        continue;
                    }
                    // Dependency on an input port. Don't insert it if other node has not been inserted.
                    else if (j.inNodeId == node
                             && nodesRemaining.find(j.outNodeId) == nodesRemaining.end()) {
                        continue;
                    }
                    skip = true;
                    break;
                }
                if (!skip) {
                    auto *nodei = dataFlowGraphModel.delegateModel<TbbDataModelVoid>(node);
                    std::vector<TbbDataModelVoid *> t;
                    t.resize(allNodes.size());
                    std::transform(allNodes.cbegin(),
                                   allNodes.cend(),
                                   t.begin(), // write to the same location
                                   [&dataFlowGraphModel](const NodeId &name) {
                                       return dataFlowGraphModel.delegateModel<TbbDataModelVoid>(
                                           name);
                                   });
                    nodei->constructTbbGraph(t, allConnectionIds, g, dataFlowGraphModel, node);

                    nodesRemaining.erase(node);
                }
            }
        }

        for (QtNodes::NodeId node : scene->graphModel().allNodeIds()) {
            auto *nodei = dataFlowGraphModel.delegateModel<TbbDataModelVoid>(node);
            nodei->tryPutTbb();
        }

        g.wait_for_all();

        for (QtNodes::NodeId node : scene->graphModel().allNodeIds()) {
            auto *nodei = dataFlowGraphModel.delegateModel<TbbDataModelVoid>(node);
            nodei->free();
        }
    });

    QObject::connect(scene, &DataFlowGraphicsScene::sceneLoaded, view, &GraphicsView::centerScene);

    mainWidget.setWindowTitle("Data Flow: simplest calculator");
    mainWidget.resize(800, 600);
    // Center window.
    mainWidget.move(QApplication::primaryScreen()->availableGeometry().center()
                    - mainWidget.rect().center());
    mainWidget.showNormal();

    return app.exec();
}
