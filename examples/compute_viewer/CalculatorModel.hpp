#pragma once

#include "CalculatorWidget.hpp"
#include <memory>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QStringLiteral>
#include <QWidget>
#include <QtNodes/NodeDelegateModel>

using QtNodes::ConnectionPolicy;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class CalculatorModel : public NodeDelegateModel
{
    Q_OBJECT

public:
    ~CalculatorModel() override = default;

public:
    unsigned int nPorts(PortType portType) const override;

    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;

    std::shared_ptr<NodeData> outData(PortIndex port) override;

    void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex) override;

    QWidget *embeddedWidget() override;

    QString caption() const override { return QStringLiteral("Calculator"); }

    QString name() const override { return QStringLiteral("Calculator"); }

    QJsonObject save() const override;

    void load(QJsonObject const &) override;

private:
    CalculatorWidget *_widget;
    QString _value;
};
