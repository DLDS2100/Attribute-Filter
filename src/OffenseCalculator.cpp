#include "OffenseCalculator.h"

OffenseCalculator::OffenseCalculator(const TypeChart &chart)
    : m_chart(chart)
{
}

QMap<QString, double> OffenseCalculator::calcTeamOffense(
    const std::vector<QStringList> &teamSkillTypes) const
{
    QMap<QString, double> result;

    for (const QString &defType : TYPE_NAMES) {
        double maxMult = 0.0;

        for (const QStringList &skills : teamSkillTypes) {
            for (const QString &skillType : skills) {
                if (skillType.isEmpty()) continue;

                // skillType 是攻击方, defType 是被攻击方
                double mult = m_chart.getDefenseMultiplier(
                    QStringList{defType}, skillType);

                if (mult > maxMult) {
                    maxMult = mult;
                }
            }
        }

        result[defType] = maxMult;
    }

    return result;
}
