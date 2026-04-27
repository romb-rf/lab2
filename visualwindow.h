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

public slots:
    void showWindow();
    void setStructureType(const QString &type);
    void insertElement(const QString &value);
    void removeCorrectElement();
    void replaceElement(int index, const QString &newValue);
    void clearAll();
    double getMedian() const;
    void cyclicShift(int positions);

private:
    StructureVisualizer *m_visualizer;
};

#endif
