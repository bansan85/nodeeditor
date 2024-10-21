#pragma once

#include <QLineEdit>
#include <QObject>
#include <QString>
#include <QWidget>
#include <QtNodes/Definitions>

using QtNodes::NodeId;
using QtNodes::PortIndex;
using QtNodes::PortType;

class DecimationWidget : public QWidget
{
    Q_OBJECT
public:
    DecimationWidget(QWidget *parent = nullptr);

    ~DecimationWidget() = default;

    float getM() const { return _m; }
    void setM(float m)
    {
        _m = m;
        _m_line->setText(QString::number(m));
    }

private:
    float _m;
    QLineEdit *_m_line;
};
