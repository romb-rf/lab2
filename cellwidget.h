#ifndef CELLWIDGET_H
#define CELLWIDGET_H

#include <QWidget>
#include <QLabel>

class CellWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
public:
    // теперь 4 аргумента: адрес, значение, размер в байтах, родитель
    explicit CellWidget(const QString &address, const QString &value,
                        int byteSize = 0, QWidget *parent = nullptr);

    void setAddress(const QString &addr);
    void setValue(const QString &val);
    void setSize(int byteSize);
    QString address() const;
    QString value() const;
    int byteSize() const;
    void setOpacity(qreal opacity);
    qreal opacity() const;

private:
    QLabel *m_addressLabel;
    QLabel *m_valueLabel;
    QLabel *m_sizeLabel;
    int m_byteSize;
    qreal m_opacity = 1.0;
};

#endif
