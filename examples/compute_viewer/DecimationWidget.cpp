#include "DecimationWidget.hpp"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSizePolicy>

DecimationWidget::DecimationWidget(QWidget *parent)
    : QWidget(nullptr)
{
    setSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);

    QGridLayout *layout = new QGridLayout(this);

    QLabel *m_label = new QLabel("M", this);
    layout->addWidget(m_label, 0, 0, 1, 1);
    _m_line = new QLineEdit(this);
    layout->addWidget(_m_line, 0, 1, 1, 1);
    connect(_m_line, &QLineEdit::textChanged, [this](QString obj) { _m = obj.toFloat(); });
}
