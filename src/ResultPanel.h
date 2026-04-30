#ifndef RESULTPANEL_H
#define RESULTPANEL_H

#include <QWidget>
#include <QMap>

// 结果展示面板：按 5 档倍率分组展示 18 种属性
class ResultPanel : public QWidget {
    Q_OBJECT
public:
    explicit ResultPanel(QWidget *parent = nullptr);

    void updateResults(const QMap<QString, double> &defenseMap);

    void clearResults();

private:
    // 倍率 → 属性列表
    QMap<double, QStringList> m_resultGroups;

    QWidget* createMultiplierRow(double multiplier, const QStringList &types);
};

#endif // RESULTPANEL_H
