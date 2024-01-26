#pragma once

#include <QtNodes/NodeData>

using QtNodes::NodeData;
using QtNodes::NodeDataType;

/// The class can potentially incapsulate any user data which
/// need to be transferred within the Node Editor graph
class DecimalData : public NodeData
{
public:
    DecimalData()
        : _number(0.0)
    {}

    DecimalData(float const number)
        : _number(number)
    {}

    NodeDataType type() const override { return NodeDataType{"decimal", "Decimal"}; }

    float number() const { return _number; }

    QString numberAsText() const { return QString::number(_number, 'f'); }

private:
    float _number;
};