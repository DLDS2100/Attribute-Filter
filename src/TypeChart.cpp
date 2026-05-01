#include "TypeChart.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <cmath>

bool TypeChart::load(const QString &jsonPath)
{
    QFile file(jsonPath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray data = file.readAll();
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (doc.isNull() || !doc.isObject()) {
        return false;
    }

    QJsonObject root = doc.object();
    m_chart.clear();

    // 解析每种属性的克制/抵抗数据
    for (const QString &atkType : TYPE_NAMES) {
        if (!root.contains(atkType)) continue;
        QJsonObject inner = root[atkType].toObject();
        for (const QString &defType : TYPE_NAMES) {
            m_chart[atkType][defType] = inner.value(defType).toString("");
        }
    }

    m_loaded = true;
    return true;
}

double TypeChart::getDefenseMultiplier(const QStringList &defenderTypes,
                                       const QString &attackType) const
{
    if (!m_loaded || defenderTypes.isEmpty()) return 1.0;

    int resistCount = 0;   // "1" 出现次数（抵抗 → 伤害 -1）
    int superCount  = 0;   // "2" 出现次数（克制 → 伤害 +1）

    for (const QString &defType : defenderTypes) {
        QString val = m_chart.value(attackType).value(defType, "");
        if (val == "0") return 0.0;  // 无效，直接返回 0
        if (val == "1") resistCount++;
        if (val == "2") superCount++;
    }

    int net = superCount - resistCount;
    if (net >= 0) {
        return 1.0 + net;           // 1, 2, 3
    } else {
        return std::pow(0.5, -net); // 0.5, 0.25
    }
}

QMap<QString, double> TypeChart::calcTeamDefense(
    const std::vector<QStringList> &team) const
{
    QMap<QString, double> result;

    for (const QString &atkType : TYPE_NAMES) {
        double minMult = 999.0;
        for (const auto &spiritTypes : team) {
            double m = getDefenseMultiplier(spiritTypes, atkType);
            if (m < minMult) minMult = m;
        }
        result[atkType] = minMult;
    }

    return result;
}
