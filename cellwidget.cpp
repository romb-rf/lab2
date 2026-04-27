#include "cellwidget.h"
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>

CellWidget::CellWidget(const QString &address, const QString &value,
                       int byteSize, QWidget *parent)
    : QWidget(parent), m_byteSize(byteSize), m_opacity(1.0)
{
    setFixedSize(110, 100);
    setStyleSheet(
        "background-color: #e8eaf6; border: 1px solid #5c6bc0; border-radius: 8px;");

    auto *layout = new QVBoxLayout(this);

    // Адрес
    m_addressLabel = new QLabel(address, this);
    m_addressLabel->setAlignment(Qt::AlignCenter);
    m_addressLabel->setStyleSheet("font-size: 10px; color: #9e9e9e; border: none;");

    // Значение
    m_valueLabel = new QLabel(value, this);
    m_valueLabel->setAlignment(Qt::AlignCenter);
    m_valueLabel->setStyleSheet(
        "font-size: 18px; font-weight: bold; color: #283593; border: none;");

    // Размер
    m_sizeLabel = new QLabel(QString("%1 bytes").arg(byteSize), this);
    m_sizeLabel->setAlignment(Qt::AlignCenter);
    m_sizeLabel->setStyleSheet("font-size: 9px; color: #9e9e9e; border: none;");

    layout->addWidget(m_addressLabel);
    layout->addWidget(m_valueLabel);
    layout->addWidget(m_sizeLabel);

    auto *effect = new QGraphicsOpacityEffect(this);
    effect->setOpacity(m_opacity);
    setGraphicsEffect(effect);
}

void CellWidget::setAddress(const QString &addr) { m_addressLabel->setText(addr); }
void CellWidget::setValue(const QString &val)   { m_valueLabel->setText(val); }
void CellWidget::setSize(int byteSize) {
    m_byteSize = byteSize;
    m_sizeLabel->setText(QString("%1 bytes").arg(byteSize));
}
QString CellWidget::address() const { return m_addressLabel->text(); }
QString CellWidget::value() const   { return m_valueLabel->text(); }
int CellWidget::byteSize() const    { return m_byteSize; }

void CellWidget::setOpacity(qreal opacity)
{
    m_opacity = opacity;
    if (auto *e = qobject_cast<QGraphicsOpacityEffect *>(graphicsEffect()))
        e->setOpacity(opacity);
}
qreal CellWidget::opacity() const { return m_opacity; }
