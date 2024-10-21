#pragma once

#include <QtNodes/NodeDelegateModel>

#include <tbb/tbb.h>
#include <QtCore/QObject>

#include <iostream>

class DecimalData;

#include "TbbDataModel.hpp"
#include <QtNodes/NodeData>

using oneapi::tbb::flow::join_node;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;
using tbb::flow::function_node;

class QLabel;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class NumberDisplayDataModel : public TbbDataModel<std::tuple<float>, std::tuple<>>
{
    Q_OBJECT

public:
    NumberDisplayDataModel();

    ~NumberDisplayDataModel() = default;

public:
    QString caption() const override { return QStringLiteral("Result"); }

    bool captionVisible() const override { return false; }

    QString name() const override { return QStringLiteral("Result"); }

public:
    unsigned int nPorts(PortType portType) const override;

    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

    std::shared_ptr<NodeData> outData(PortIndex port) override;

    void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

    QWidget *embeddedWidget() override;

    float number() const;

private:
    void constructTbbGraph(const std::vector<TbbDataModelVoid *> &range,
                           const std::unordered_set<ConnectionId> &connectIds,
                           tbb::flow::graph &g,
                           DataFlowGraphModel &graphModel,
                           NodeId nodeId) override
    {
        buf1_node1 = std::make_shared<broadcast_node<float>>(g);
        input_data = std::make_shared<join_node<std::tuple<float>>>(g);

        for (const ConnectionId &conId : connectIds) {
            if (conId.inNodeId == nodeId) {
                oneapi::tbb::flow::sender<float> &sender
                    = *graphModel.delegateModel<TbbDataModelVoid>(conId.outNodeId)
                           ->getTbbSender<float>(conId.outPortIndex);
                make_edge(sender, *buf1_node1);

                make_edge(sender, input_port<0>(*input_data));
                func_node = std::make_shared<
                    tbb::flow::function_node<std::tuple<float>, tbb::flow::continue_msg>>(
                        g, tbb::flow::unlimited, [](const std::tuple<float> &input) {
                            float a = std::get<0>(input);
                            std::cout << "Result " << a << "\n";
                        });

                make_edge(*input_data, *func_node);
            }
        }
    }

    void tryPutTbb() override {}

    void free() override
    {
        buf1_node1.reset();
        input_data.reset();
        func_node.reset();
    }

    void *getTbbSenderSub(PortIndex idx) override { return nullptr; }

    std::shared_ptr<DecimalData> _numberData;

    QLabel *_label;

    std::shared_ptr<broadcast_node<float>> buf1_node1;

    std::shared_ptr<join_node<std::tuple<float>>> input_data;
    std::shared_ptr<tbb::flow::function_node<std::tuple<float>, tbb::flow::continue_msg>>
        func_node;
};
