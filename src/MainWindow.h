#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QVector>
#include "TypeChart.h"
#include "SpiritRow.h"
#include "ResultPanel.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onAddSpirit();
    void onRemoveSpirit(int index);
    void onCalculate();

private:
    static constexpr int MAX_SPIRITS = 6;

    TypeChart m_chart;
    QVector<SpiritRow*> m_spiritRows;
    QVBoxLayout *m_spiritLayout = nullptr;
    QPushButton *m_addBtn = nullptr;
    QPushButton *m_calcBtn = nullptr;
    ResultPanel *m_resultPanel = nullptr;

    void rebuildSpiritLabels();
    void updateAddButton();
    bool loadTypeChart();
};

#endif // MAINWINDOW_H
