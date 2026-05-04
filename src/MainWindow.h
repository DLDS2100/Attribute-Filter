#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include "TypeChart.h"
#include "InputRow.h"
#include "ResultPanel.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onAddSpirit();
    void onRemoveSpirit(int index);
    void onCalculate();
    void onModeChanged();

private:
    static constexpr int MAX_SPIRITS = 6;

    CalcMode m_currentMode = CalcMode::Defense;

    TypeChart m_chart;
    QVector<InputRow*> m_inputRows;
    QVBoxLayout *m_inputLayout = nullptr;
    QPushButton *m_addBtn = nullptr;
    QPushButton *m_calcBtn = nullptr;
    ResultPanel *m_resultPanel = nullptr;

    // Tab 栏
    QPushButton *m_tabDefense = nullptr;
    QPushButton *m_tabOffense = nullptr;
    QLabel *m_modeSubLabel = nullptr;

    void rebuildInputArea();
    void rebuildSpiritLabels();
    void updateAddButton();
    void updateModeUI();
    bool loadTypeChart();
};

#endif // MAINWINDOW_H
