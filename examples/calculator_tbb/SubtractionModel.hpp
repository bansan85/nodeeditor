#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QLabel>

#include <QtNodes/NodeDelegateModel>

#include "MathOperationDataModel.hpp"

#include "DecimalData.hpp"

using oneapi::tbb::flow::function_node;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class SubtractionModel : public MathOperationDataModel<std::tuple<float, float>, std::tuple<float>>
{
public:
    virtual ~SubtractionModel() {}

public:
    QString caption() const override { return QStringLiteral("Subtraction"); }

    virtual bool portCaptionVisible(PortType portType, PortIndex portIndex) const override
    {
        Q_UNUSED(portType);
        Q_UNUSED(portIndex);
        return true;
    }

    virtual QString portCaption(PortType portType, PortIndex portIndex) const override
    {
        switch (portType) {
        case PortType::In:
            if (portIndex == 0)
                return QStringLiteral("Minuend");
            else if (portIndex == 1)
                return QStringLiteral("Subtrahend");

            break;

        case PortType::Out:
            return QStringLiteral("Result");

        default:
            break;
        }
        return QString();
    }

    QString name() const override { return QStringLiteral("Subtraction"); }

private:
    void constructTbbGraph(const std::vector<TbbDataModelVoid *> &range,
                           const std::unordered_set<ConnectionId> &connectIds,
                           tbb::flow::graph &g,
                           DataFlowGraphModel &graphModel,
                           NodeId nodeId) override
    {
        input_data = std::make_shared<join_node<std::tuple<float, float>>>(g);
        for (const ConnectionId &conId : connectIds) {
            if (conId.inNodeId == nodeId) {
                oneapi::tbb::flow::sender<float> &sender
                    = *graphModel.delegateModel<TbbDataModelVoid>(conId.outNodeId)
                           ->getTbbSender<float>(conId.outPortIndex);
                if (conId.inPortIndex == 0)
                    make_edge(sender, input_port<0>(*input_data));
                else if (conId.inPortIndex == 1)
                    make_edge(sender, input_port<1>(*input_data));
            }
        }

        func_node = std::make_shared<tbb::flow::function_node<std::tuple<float, float>, float>>(
            g, tbb::flow::unlimited, [this](const std::tuple<float, float> &input) {
                float a = std::get<0>(input);
                float b = std::get<1>(input);
                std::cout << "substract_node\n";
                _result = std::make_shared<DecimalData>(a - b);
                QMetaObject::invokeMethod(qApp, [this] { Q_EMIT dataUpdated(0); });
                return a - b;
            });

        make_edge(*input_data, *func_node);
    }

    void tryPutTbb() override {}

    void free() override
    {
        input_data.reset();
        func_node.reset();
    }

    void *getTbbSenderSub(PortIndex idx) override
    {
        if (idx == 0) {
            return static_cast<oneapi::tbb::flow::sender<float>*> (func_node.get());
        } else {
            return nullptr;
        }
    }

    std::shared_ptr<join_node<std::tuple<float, float>>> input_data;
    std::shared_ptr<tbb::flow::function_node<std::tuple<float, float>, float>> func_node;
};
