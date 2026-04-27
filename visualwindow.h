// visualizationwindow.h
#ifndef VISUALIZATIONWINDOW_H
#define VISUALIZATIONWINDOW_H

#include <QWidget>
#include "datamanager.h"
#include "visual.h"

class VisualizationWindow : public QWidget
{
    Q_OBJECT
public:
    explicit VisualizationWindow(DataManager *manager, QWidget *parent = nullptr);

    Q_INVOKABLE void showWindow();
    Q_INVOKABLE void setStructureType(const QString &type);
    Q_INVOKABLE void insertElement(const QString &value);
    Q_INVOKABLE void removeLastElement();
    Q_INVOKABLE void replaceElement(int index, const QString &newValue);
    Q_INVOKABLE void clearAll();
    Q_INVOKABLE double getMedian() const;
    Q_INVOKABLE void cyclicShift(int positions);

private:
    StructureVisualizer *m_visualizer;
};

#endif
