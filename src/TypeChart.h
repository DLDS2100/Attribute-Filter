#ifndef TYPECHART_H
#define TYPECHART_H

#include <QString>
#include <QMap>
#include <vector>

// 18 种属性标准名称（与 TypeChart.json _types 数组对应）
// 注意：文件名中"恶"属性对应 PNG 名为"恶魔.png"
inline const QStringList TYPE_NAMES = {
    QStringLiteral("普通"), QStringLiteral("草"), QStringLiteral("火"),
    QStringLiteral("水"),   QStringLiteral("光"), QStringLiteral("地"),
    QStringLiteral("冰"),   QStringLiteral("龙"), QStringLiteral("电"),
    QStringLiteral("毒"),   QStringLiteral("虫"), QStringLiteral("武"),
    QStringLiteral("翼"),   QStringLiteral("萌"), QStringLiteral("幽"),
    QStringLiteral("恶"),   QStringLiteral("机械"), QStringLiteral("幻")
};

// 属性名 → PNG 资源路径的映射
inline const QMap<QString, QString> TYPE_ICON_MAP = {
    {QStringLiteral("普通"), QStringLiteral(":/icons/type_icons/普通.png")},
    {QStringLiteral("草"),   QStringLiteral(":/icons/type_icons/草.png")},
    {QStringLiteral("火"),   QStringLiteral(":/icons/type_icons/火.png")},
    {QStringLiteral("水"),   QStringLiteral(":/icons/type_icons/水.png")},
    {QStringLiteral("光"),   QStringLiteral(":/icons/type_icons/光.png")},
    {QStringLiteral("地"),   QStringLiteral(":/icons/type_icons/地.png")},
    {QStringLiteral("冰"),   QStringLiteral(":/icons/type_icons/冰.png")},
    {QStringLiteral("龙"),   QStringLiteral(":/icons/type_icons/龙.png")},
    {QStringLiteral("电"),   QStringLiteral(":/icons/type_icons/电.png")},
    {QStringLiteral("毒"),   QStringLiteral(":/icons/type_icons/毒.png")},
    {QStringLiteral("虫"),   QStringLiteral(":/icons/type_icons/虫.png")},
    {QStringLiteral("武"),   QStringLiteral(":/icons/type_icons/武.png")},
    {QStringLiteral("翼"),   QStringLiteral(":/icons/type_icons/翼.png")},
    {QStringLiteral("萌"),   QStringLiteral(":/icons/type_icons/萌.png")},
    {QStringLiteral("幽"),   QStringLiteral(":/icons/type_icons/幽.png")},
    {QStringLiteral("恶"),   QStringLiteral(":/icons/type_icons/恶.png")},  // 新图标
    {QStringLiteral("机械"), QStringLiteral(":/icons/type_icons/机械.png")},
    {QStringLiteral("幻"),   QStringLiteral(":/icons/type_icons/幻.png")},
};

// 每种属性对应的主题色（用于UI着色）
inline const QMap<QString, QString> TYPE_COLOR_MAP = {
    {QStringLiteral("普通"), QStringLiteral("#A8A878")},
    {QStringLiteral("草"),   QStringLiteral("#78C850")},
    {QStringLiteral("火"),   QStringLiteral("#F08030")},
    {QStringLiteral("水"),   QStringLiteral("#6890F0")},
    {QStringLiteral("光"),   QStringLiteral("#F8D030")},
    {QStringLiteral("地"),   QStringLiteral("#E0C068")},
    {QStringLiteral("冰"),   QStringLiteral("#98D8D8")},
    {QStringLiteral("龙"),   QStringLiteral("#7038F8")},
    {QStringLiteral("电"),   QStringLiteral("#F8D030")},
    {QStringLiteral("毒"),   QStringLiteral("#A040A0")},
    {QStringLiteral("虫"),   QStringLiteral("#A8B820")},
    {QStringLiteral("武"),   QStringLiteral("#C03028")},
    {QStringLiteral("翼"),   QStringLiteral("#A890F0")},
    {QStringLiteral("萌"),   QStringLiteral("#EE99AC")},
    {QStringLiteral("幽"),   QStringLiteral("#705898")},
    {QStringLiteral("恶"),   QStringLiteral("#705848")},
    {QStringLiteral("机械"), QStringLiteral("#B8B8D0")},
    {QStringLiteral("幻"),   QStringLiteral("#F85888")},
};

class TypeChart {
public:
    // 加载 JSON 文件，返回是否成功
    bool load(const QString &jsonPath);

    // 计算单只精灵对某种攻击属性的防守倍率
    // defenderTypes: 精灵的属性列表（1或2个元素）
    // attackType: 攻击方属性名
    // 返回: 倍率值（0 = 无效, 0.25, 0.5, 1.0, 2.0, 3.0）
    double getDefenseMultiplier(const QStringList &defenderTypes,
                                const QString &attackType) const;

    // 计算整个队伍对 18 种攻击属性的防守倍率
    // team: 队伍中每只精灵的属性列表
    // 返回: map<攻击属性名, 倍率>，队伍取所有精灵中的最低倍率
    QMap<QString, double> calcTeamDefense(
        const std::vector<QStringList> &team) const;

    bool isLoaded() const { return m_loaded; }

private:
    bool m_loaded = false;
    // chart[攻击属性][被攻击属性] = "0"/"1"/"2"/""
    QMap<QString, QMap<QString, QString>> m_chart;
};

#endif // TYPECHART_H
