#include "DecimationModel.hpp"
#include "DecimationData.hpp"
#include "DecimationWidget.hpp"
#include "InputData.hpp"
#include <QJsonValue>
#include <QJsonValueRef>
#include <QtNodes/NodeDelegateModel>

using QtNodes::NodeStyle;

unsigned int DecimationModel::nPorts(PortType portType) const
{
    unsigned int result;

    if (portType == PortType::In)
        result = 1;
    else
        result = 1;

    return result;
}

NodeDataType DecimationModel::dataType(PortType portType, PortIndex) const
{
    if (portType == PortType::In) {
        return InputData().type();
    } else {
        return DecimationData().type();
    }
}

std::shared_ptr<NodeData> DecimationModel::outData(PortIndex)
{
    return nullptr;
}

void DecimationModel::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex)
{
    auto numberData = std::dynamic_pointer_cast<DecimationModel>(data);

    if (!data) {
        Q_EMIT dataInvalidated(0);
    }
}

QWidget *DecimationModel::embeddedWidget()
{
    if (_widget) {
        return _widget;
    }

    _widget = new DecimationWidget(nullptr);

    return _widget;
}

QJsonObject DecimationModel::save() const
{
    QJsonObject retval = NodeDelegateModel::save();
    retval["M"] = _widget->getM();
    return retval;
}

void DecimationModel::load(QJsonObject const &object)
{
    NodeDelegateModel::load(object);
    _widget->setM(object["M"].toDouble());
}
