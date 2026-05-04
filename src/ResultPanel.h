#ifndef RESULTPANEL_H
#define RESULTPANEL_H

#include <QWidget>
#include <QMap>
#include "TypeChart.h"

// 结果展示面板
// Defense 模式：按 5 档倍率分组（0.25x / 0.5x / 1x / 2x / 3x + 0x）
// Offense 模式：按 3 档分组（2x 可克制 / 1x 中性 / 0.5x 被抵抗含0x）
class ResultPanel : public QWidget {
    Q_OBJECT
public:
    explicit ResultPanel(QWidget *parent = nullptr);

    void setMode(CalcMode mode);
    void updateResults(const QMap<QString, double> &resultMap);
    void clearResults();

private:
    CalcMode m_mode = CalcMode::Defense;

    QWidget* createMultiplierRow(double multiplier, const QStringList &types);
};

#endif // RESULTPANEL_H
