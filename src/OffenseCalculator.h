#ifndef OFFENSECALCULATOR_H
#define OFFENSECALCULATOR_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <vector>
#include "TypeChart.h"

// 打击盲点计算器
// 输入：所有精灵的攻击技能属性
// 输出：18 种防守属性各自的最高倍率
class OffenseCalculator {
public:
    explicit OffenseCalculator(const TypeChart &chart);

    // teamSkillTypes: 每只精灵的技能属性列表
    // 返回: map<防守属性名, 最高倍率>
    QMap<QString, double> calcTeamOffense(
        const std::vector<QStringList> &teamSkillTypes) const;

private:
    const TypeChart &m_chart;
};

#endif // OFFENSECALCULATOR_H
