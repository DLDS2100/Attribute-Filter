#include "ResultPanel.h"
#include "TypeChart.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>

ResultPanel::ResultPanel(QWidget *parent)
    : QWidget(parent), m_mode(CalcMode::Defense)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    QLabel *title = new QLabel(QString::fromUtf8(u8"联防结果"), this);
    title->setStyleSheet(QStringLiteral(
        "font-size: 16px; font-weight: bold; color: #333333; padding: 8px 0;"));
    layout->addWidget(title);

    QLabel *hint = new QLabel(QString::fromUtf8(u8"选择精灵属性后点击「开始计算」"), this);
    hint->setAlignment(Qt::AlignCenter);
    hint->setStyleSheet(QStringLiteral(
        "color: #999999; font-size: 13px; padding: 20px;"));
    layout->addWidget(hint);
}

void ResultPanel::setMode(CalcMode mode)
{
    m_mode = mode;
}

void ResultPanel::clearResults()
{
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(this->layout());
    while (QLayoutItem *item = layout->takeAt(0)) {
        if (QWidget *w = item->widget()) {
            delete w;
        }
        delete item;
    }

    QString titleText = (m_mode == CalcMode::Defense)
        ? QString::fromUtf8(u8"联防结果")
        : QString::fromUtf8(u8"打击覆盖");
    QString hintText = (m_mode == CalcMode::Defense)
        ? QString::fromUtf8(u8"选择精灵属性后点击「开始计算」")
        : QString::fromUtf8(u8"选择攻击技能属性后点击「开始计算」");

    QLabel *title = new QLabel(titleText, this);
    title->setStyleSheet(QStringLiteral(
        "font-size: 16px; font-weight: bold; color: #333333; padding: 8px 0;"));
    layout->addWidget(title);

    QLabel *hint = new QLabel(hintText, this);
    hint->setAlignment(Qt::AlignCenter);
    hint->setStyleSheet(QStringLiteral(
        "color: #999999; font-size: 13px; padding: 20px;"));
    layout->addWidget(hint);
}

void ResultPanel::updateResults(const QMap<QString, double> &resultMap)
{
    // 清除旧内容（包括 Widget 子项和 Spacer 条目）
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(this->layout());
    while (QLayoutItem *item = layout->takeAt(0)) {
        if (QWidget *w = item->widget()) {
            delete w;
        }
        delete item;
    }

    // 标题
    QString titleText = (m_mode == CalcMode::Defense)
        ? QString::fromUtf8(u8"联防结果")
        : QString::fromUtf8(u8"打击覆盖");
    QLabel *title = new QLabel(titleText, this);
    title->setStyleSheet(QStringLiteral(
        "font-size: 16px; font-weight: bold; color: #333333; padding: 8px 0;"));
    layout->addWidget(title);

    if (m_mode == CalcMode::Defense) {
        // === 联防模式：5 档分组 ===
        QMap<double, QStringList> groups;
        groups[0.25]; groups[0.5]; groups[1.0]; groups[2.0]; groups[3.0];
        QStringList immuneTypes;

        for (auto it = resultMap.constBegin(); it != resultMap.constEnd(); ++it) {
            if (it.value() == 0.0) {
                immuneTypes << it.key();
            } else {
                groups[it.value()] << it.key();
            }
        }

        QList<double> order = {0.25, 0.5, 1.0, 2.0, 3.0};
        for (double mult : order) {
            layout->addWidget(createMultiplierRow(mult, groups[mult]));
        }
        if (!immuneTypes.isEmpty()) {
            layout->addWidget(createMultiplierRow(0.0, immuneTypes));
        }

    } else {
        // === 打击模式：3 档分组 ===
        QStringList twoX, oneX, halfX;

        for (auto it = resultMap.constBegin(); it != resultMap.constEnd(); ++it) {
            double mult = it.value();
            if (mult >= 2.0) {
                twoX << it.key();
            } else if (mult == 1.0) {
                oneX << it.key();
            } else {
                // 0.5 或 0.0（被抵抗或免疫）→ 归入 0.5x 档
                halfX << it.key();
            }
        }

        layout->addWidget(createMultiplierRow(2.0, twoX));
        layout->addWidget(createMultiplierRow(1.0, oneX));
        layout->addWidget(createMultiplierRow(0.5, halfX));
    }

    layout->addStretch();
}

QWidget* ResultPanel::createMultiplierRow(double multiplier, const QStringList &types)
{
    QWidget *row = new QWidget(this);

    QString bgColor, labelColor, labelText, descText;

    // 根据模式决定描述文字
    if (m_mode == CalcMode::Defense) {
        // 联防模式：从防守方视角描述
        if (multiplier == 0.0) {
            bgColor = "#E8E8E8"; labelColor = "#666666";
            labelText = "0x"; descText = QString::fromUtf8(u8"无效");
        } else if (multiplier == 0.25) {
            bgColor = "#C8E6C9"; labelColor = "#2E7D32";
            labelText = "0.25x"; descText = QString::fromUtf8(u8"强力抵抗");
        } else if (multiplier == 0.5) {
            bgColor = "#E8F5E9"; labelColor = "#388E3C";
            labelText = "0.5x"; descText = QString::fromUtf8(u8"抵抗");
        } else if (multiplier == 1.0) {
            bgColor = "#FAFAFA"; labelColor = "#757575";
            labelText = "1.0x"; descText = QString::fromUtf8(u8"正常");
        } else if (multiplier == 2.0) {
            bgColor = "#FFF3E0"; labelColor = "#E65100";
            labelText = "2.0x"; descText = QString::fromUtf8(u8"被克制");
        } else if (multiplier == 3.0) {
            bgColor = "#FFEBEE"; labelColor = "#C62828";
            labelText = "3.0x"; descText = QString::fromUtf8(u8"严重克制");
        } else {
            bgColor = "#FFFFFF"; labelColor = "#333333";
            labelText = QString("%1x").arg(multiplier);
            descText = "";
        }
    } else {
        // 打击模式：从攻击方视角描述
        if (multiplier == 0.0) {
            bgColor = "#E8E8E8"; labelColor = "#666666";
            labelText = "0x"; descText = QString::fromUtf8(u8"无效（免疫）");
        } else if (multiplier == 0.5) {
            bgColor = "#E8F5E9"; labelColor = "#388E3C";
            labelText = "0.5x"; descText = QString::fromUtf8(u8"被抵抗");
        } else if (multiplier == 1.0) {
            bgColor = "#FAFAFA"; labelColor = "#757575";
            labelText = "1.0x"; descText = QString::fromUtf8(u8"正常");
        } else if (multiplier == 2.0) {
            bgColor = "#FFF3E0"; labelColor = "#E65100";
            labelText = "2.0x"; descText = QString::fromUtf8(u8"克制");
        } else {
            bgColor = "#FFFFFF"; labelColor = "#333333";
            labelText = QString("%1x").arg(multiplier);
            descText = "";
        }
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

    QLabel *multLabel = new QLabel(labelText, row);
    multLabel->setFixedWidth(70);
    multLabel->setAlignment(Qt::AlignCenter);
    multLabel->setStyleSheet(QStringLiteral(
        "font-size: 22px; font-weight: bold; color: %1;").arg(labelColor));
    hLayout->addWidget(multLabel);

    QLabel *descLabel = new QLabel(descText, row);
    descLabel->setFixedWidth(72);
    descLabel->setStyleSheet(QStringLiteral(
        "font-size: 12px; color: %1;").arg(labelColor));
    hLayout->addWidget(descLabel);

    for (const QString &typeName : types) {
        QLabel *iconLabel = new QLabel(row);
        iconLabel->setFixedSize(72, 72);
        iconLabel->setAlignment(Qt::AlignCenter);
        iconLabel->setToolTip(typeName);

        QString iconPath = TYPE_ICON_MAP.value(typeName, "");
        if (!iconPath.isEmpty()) {
            QPixmap pixmap(iconPath);
            if (!pixmap.isNull()) {
                iconLabel->setPixmap(pixmap.scaled(64, 64, Qt::KeepAspectRatio,
                                                    Qt::SmoothTransformation));
            }
        }
        iconLabel->setCursor(Qt::WhatsThisCursor);
        hLayout->addWidget(iconLabel);
    }

    if (types.isEmpty()) {
        QLabel *empty = new QLabel(QString::fromUtf8(u8"—"), row);
        empty->setStyleSheet(QStringLiteral("color: #CCCCCC; font-size: 13px;"));
        hLayout->addWidget(empty);
    }

    hLayout->addStretch();

    return row;
}
