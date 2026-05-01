#include "ResultPanel.h"
#include "TypeChart.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>

ResultPanel::ResultPanel(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // 标题
    QLabel *title = new QLabel(QStringLiteral("联防结果"), this);
    title->setStyleSheet(QStringLiteral(
        "font-size: 16px; font-weight: bold; color: #333333; padding: 8px 0;"));
    layout->addWidget(title);

    // 提示文字
    QLabel *hint = new QLabel(QStringLiteral("选择精灵属性后点击「开始计算」"), this);
    hint->setAlignment(Qt::AlignCenter);
    hint->setObjectName("hintLabel");
    hint->setStyleSheet(QStringLiteral(
        "color: #999999; font-size: 13px; padding: 20px;"));
    layout->addWidget(hint);
}

void ResultPanel::clearResults()
{
    // 清除所有子控件
    qDeleteAll(findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(this->layout());

    QLabel *title = new QLabel(QStringLiteral("联防结果"), this);
    title->setStyleSheet(QStringLiteral(
        "font-size: 16px; font-weight: bold; color: #333333; padding: 8px 0;"));
    layout->addWidget(title);

    QLabel *hint = new QLabel(QStringLiteral("选择精灵属性后点击「开始计算」"), this);
    hint->setAlignment(Qt::AlignCenter);
    hint->setStyleSheet(QStringLiteral(
        "color: #999999; font-size: 13px; padding: 20px;"));
    layout->addWidget(hint);
}

void ResultPanel::updateResults(const QMap<QString, double> &defenseMap)
{
    // 分组
    m_resultGroups.clear();
    m_resultGroups[0.25];
    m_resultGroups[0.5];
    m_resultGroups[1.0];
    m_resultGroups[2.0];
    m_resultGroups[3.0];

    QStringList immuneTypes;

    for (auto it = defenseMap.constBegin(); it != defenseMap.constEnd(); ++it) {
        if (it.value() == 0.0) {
            immuneTypes << it.key();
        } else {
            m_resultGroups[it.value()] << it.key();
        }
    }

    // 清除旧内容
    qDeleteAll(findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(this->layout());

    // 标题
    QLabel *title = new QLabel(QStringLiteral("联防结果"), this);
    title->setStyleSheet(QStringLiteral(
        "font-size: 16px; font-weight: bold; color: #333333; padding: 8px 0;"));
    layout->addWidget(title);

    // 按 5 档顺序展示
    QList<double> order = {0.25, 0.5, 1.0, 2.0, 3.0};
    for (double mult : order) {
        QStringList types = m_resultGroups[mult];
        if (!immuneTypes.isEmpty()) {
            // 将无效属性合并到 0.25 档标注
        }
        QWidget *row = createMultiplierRow(mult, types);
        layout->addWidget(row);
    }

    // 无效属性单独一行
    if (!immuneTypes.isEmpty()) {
        QWidget *row = createMultiplierRow(0.0, immuneTypes);
        layout->addWidget(row);
    }

    layout->addStretch();
}

QWidget* ResultPanel::createMultiplierRow(double multiplier, const QStringList &types)
{
    QWidget *row = new QWidget(this);

    // 颜色方案
    QString bgColor, labelColor, labelText, descText;
    if (multiplier == 0.0) {
        bgColor = "#E8E8E8"; labelColor = "#666666";
        labelText = "0x"; descText = "无效";
    } else if (multiplier == 0.25) {
        bgColor = "#C8E6C9"; labelColor = "#2E7D32";
        labelText = "0.25x"; descText = "强力抵抗";
    } else if (multiplier == 0.5) {
        bgColor = "#E8F5E9"; labelColor = "#388E3C";
        labelText = "0.5x"; descText = "抵抗";
    } else if (multiplier == 1.0) {
        bgColor = "#FAFAFA"; labelColor = "#757575";
        labelText = "1.0x"; descText = "正常";
    } else if (multiplier == 2.0) {
        bgColor = "#FFF3E0"; labelColor = "#E65100";
        labelText = "2.0x"; descText = "被克制";
    } else if (multiplier == 3.0) {
        bgColor = "#FFEBEE"; labelColor = "#C62828";
        labelText = "3.0x"; descText = "严重克制";
    } else {
        bgColor = "#FFFFFF"; labelColor = "#333333";
        labelText = QString("%1x").arg(multiplier); descText = "";
    }

    row->setStyleSheet(QStringLiteral(
        "QWidget#rowWidget {"
        "   background-color: %1;"
        "   border-radius: 10px;"
        "   margin: 4px 0px;"
        "}"
    ).arg(bgColor));
    row->setObjectName("rowWidget");

    QHBoxLayout *hLayout = new QHBoxLayout(row);
    hLayout->setContentsMargins(16, 12, 16, 12);
    hLayout->setSpacing(12);

    // 倍率标签
    QLabel *multLabel = new QLabel(labelText, row);
    multLabel->setFixedWidth(70);
    multLabel->setAlignment(Qt::AlignCenter);
    multLabel->setStyleSheet(QStringLiteral(
        "font-size: 22px; font-weight: bold; color: %1;").arg(labelColor));
    hLayout->addWidget(multLabel);

    // 描述
    QLabel *descLabel = new QLabel(descText, row);
    descLabel->setFixedWidth(72);
    descLabel->setStyleSheet(QStringLiteral(
        "font-size: 12px; color: %1;").arg(labelColor));
    hLayout->addWidget(descLabel);

    // 属性图标（尺寸从 52x52 增大到 72x72，图标从 48 增大到 64）
    for (const QString &typeName : types) {
        QLabel *iconLabel = new QLabel(row);
        iconLabel->setFixedSize(72, 72);
        iconLabel->setAlignment(Qt::AlignCenter);
        iconLabel->setToolTip(typeName);

        QString iconPath = TYPE_ICON_MAP.value(typeName, "");
        if (!iconPath.isEmpty()) {
            QPixmap pixmap(iconPath);
            if (!pixmap.isNull()) {
                // 增大图标显示尺寸：48x48 → 64x64
                iconLabel->setPixmap(pixmap.scaled(64, 64, Qt::KeepAspectRatio,
                                                    Qt::SmoothTransformation));
            }
        }
        iconLabel->setCursor(Qt::WhatsThisCursor);
        hLayout->addWidget(iconLabel);
    }

    // 如果该档没有属性，显示"—"
    if (types.isEmpty()) {
        QLabel *empty = new QLabel(QStringLiteral("—"), row);
        empty->setStyleSheet(QStringLiteral("color: #CCCCCC; font-size: 13px;"));
        hLayout->addWidget(empty);
    }

    hLayout->addStretch();

    return row;
}
