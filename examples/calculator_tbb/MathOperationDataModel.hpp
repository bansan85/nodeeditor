#pragma once

#include "DecimalData.hpp"
#include "TbbDataModel.hpp"
#include <memory>
#include <QObject>
#include <QWidget>
#include <QtNodes/NodeDelegateModel>

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
template<typename Input, typename Output>
class MathOperationDataModel : public TbbDataModel<Input, Output>
{
public:
    ~MathOperationDataModel() = default;

public:
    unsigned int nPorts(PortType portType) const override
    {
        unsigned int result;

        if (portType == PortType::In) {
            static_assert(std::tuple_size_v<Input> == 2);
            result = 2;
        } else {
            static_assert(std::tuple_size_v<Output> == 1);
            result = 1;
        }

        return result;
    }

    NodeDataType dataType(PortType portType, PortIndex portIndex) const override
    {
        return DecimalData().type();
    }

    std::shared_ptr<NodeData> outData(PortIndex port) override
    {
        return std::static_pointer_cast<NodeData>(_result);
    }

    void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override
    {
        auto numberData = std::dynamic_pointer_cast<DecimalData>(data);

        if (!data) {
            Q_EMIT this->dataInvalidated(0);
        }

        if (portIndex == 0) {
            _number1 = numberData;
        } else {
            _number2 = numberData;
        }
    }

    QWidget *embeddedWidget() override { return nullptr; }

protected:
    std::weak_ptr<DecimalData> _number1;
    std::weak_ptr<DecimalData> _number2;

    std::shared_ptr<DecimalData> _result;
};
