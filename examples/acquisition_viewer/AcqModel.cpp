#include "AcqModel.hpp"
#include "AcqData.hpp"
#include "QtNodes/NodeDelegateModel"
#include <cstdlib>
#include <QColor>
#include <QInputDialog>
#include <QLineEdit>

using QtNodes::NodeStyle;

unsigned int AcqModel::nPorts(PortType portType) const
{
    unsigned int result;

    if (portType == PortType::In)
        result = 1;
    else
        result = 1;

    return result;
}

NodeDataType AcqModel::dataType(PortType, PortIndex) const
{
    return AcqData().type();
}

std::shared_ptr<NodeData> AcqModel::outData(PortIndex)
{
    return std::static_pointer_cast<NodeData>(_result);
}

void AcqModel::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex)
{
    auto numberData = std::dynamic_pointer_cast<AcqData>(data);

    if (!data) {
        Q_EMIT dataInvalidated(0);
    }
}

QWidget *AcqModel::embeddedWidget()
{
    if (_result) {
        return nullptr;
    }

    _result = std::make_shared<AcqData>();

    bool ok;

    QString text
        = QInputDialog::getText(nullptr, "Acquisition node", "Title:", QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty())
        _title = text;

    NodeStyle style;
    style.GradientColor0 = {rand() % 256, rand() % 256, rand() % 256};
    style.GradientColor1 = {rand() % 256, rand() % 256, rand() % 256};
    style.GradientColor2 = {rand() % 256, rand() % 256, rand() % 256};
    style.GradientColor3 = {rand() % 256, rand() % 256, rand() % 256};
    setNodeStyle(style);

    return nullptr;
}
