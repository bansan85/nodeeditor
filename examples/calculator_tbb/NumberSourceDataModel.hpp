#pragma once

#include <QtNodes/NodeDelegateModel>

#include <QtCore/QObject>

#include "DecimalData.hpp"
#include <tbb/tbb.h>

#include <iostream>

class DecimalData;

#include "TbbDataModel.hpp"

using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;

using oneapi::tbb::flow::broadcast_node;

class QLineEdit;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class NumberSourceDataModel : public TbbDataModel<std::tuple<>, std::tuple<float>>
{
    Q_OBJECT

public:
    NumberSourceDataModel();

    virtual ~NumberSourceDataModel() {}

public:
    QString caption() const override { return QStringLiteral("Number Source"); }

    bool captionVisible() const override { return false; }

    QString name() const override { return QStringLiteral("NumberSource"); }

public:
    QJsonObject save() const override;

    void load(QJsonObject const &p) override;

public:
    unsigned int nPorts(PortType portType) const override;

    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

    std::shared_ptr<NodeData> outData(PortIndex port) override;

    void setInData(std::shared_ptr<NodeData>, PortIndex) override {}

    QWidget *embeddedWidget() override;

public:
    void setNumber(float number);

private Q_SLOTS:

    void onTextEdited(QString const &string);

private:
    void constructTbbGraph(const std::vector<TbbDataModelVoid *> &range,
                           const std::unordered_set<ConnectionId> &connectIds,
                           tbb::flow::graph &g,
                           DataFlowGraphModel &graphModel,
                           NodeId nodeId) override
    {
        buf1_node1 = std::make_shared<broadcast_node<float>>(g);
    }

    void tryPutTbb() override { 
        buf1_node1->try_put(_number->number());
        Q_EMIT dataUpdated(0);
    }

    void free() override
    {
        buf1_node1.reset();
    }

    void *getTbbSenderSub(PortIndex idx) override
    {
        if (idx == 0) {
            return static_cast<oneapi::tbb::flow::sender<float> *>(buf1_node1.get());
        } else {
            return nullptr;
        }
    }

    std::shared_ptr<DecimalData> _number;

    QLineEdit *_lineEdit;

    std::shared_ptr<broadcast_node<float>> buf1_node1;
};
