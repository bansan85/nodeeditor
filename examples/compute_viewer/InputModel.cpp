#include "InputModel.hpp"
#include "InputData.hpp"
#include <QtNodes/NodeDelegateModel>

using QtNodes::NodeStyle;

unsigned int InputModel::nPorts(PortType portType) const
{
    unsigned int result;

    if (portType == PortType::In)
        result = 0;
    else
        result = 1;

    return result;
}

NodeDataType InputModel::dataType(PortType, PortIndex) const
{
    return InputData().type();
}

std::shared_ptr<NodeData> InputModel::outData(PortIndex)
{
    return nullptr;
}

void InputModel::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex)
{
    auto numberData = std::dynamic_pointer_cast<InputModel>(data);

    if (!data) {
        Q_EMIT dataInvalidated(0);
    }
}

QWidget *InputModel::embeddedWidget()
{
    return nullptr;
}
