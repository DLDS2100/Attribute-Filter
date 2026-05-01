#include "SpiritRow.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

SpiritRow::SpiritRow(int index, QWidget *parent)
    : QWidget(parent), m_index(index)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(8);

    // 精灵编号标签
    QLabel *label = new QLabel(QStringLiteral("精灵 %1").arg(index), this);
    label->setFixedWidth(60);
    label->setStyleSheet(QStringLiteral(
        "font-size: 14px; font-weight: bold; color: #333333;"));
    layout->addWidget(label);

    // 属性选择按钮
    m_typeBtn1 = new TypeButton(false, this); // 必选
    m_typeBtn2 = new TypeButton(true, this);  // 可选（第二属性）

    layout->addWidget(m_typeBtn1);
    layout->addWidget(m_typeBtn2);

    // 右侧弹性空间
    layout->addStretch();

    // 删除按钮
    QPushButton *delBtn = new QPushButton(QStringLiteral("×"), this);
    delBtn->setFixedSize(32, 32);
    delBtn->setCursor(Qt::PointingHandCursor);
    delBtn->setToolTip(QStringLiteral("移除此精灵"));
    delBtn->setStyleSheet(QStringLiteral(
        "QPushButton {"
        "   border: 1px solid #E0E0E0;"
        "   border-radius: 6px;"
        "   background-color: #FAFAFA;"
        "   color: #CC0000;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #FFE0E0;"
        "   border-color: #CC0000;"
        "}"
    ));

    layout->addWidget(delBtn);

    // 信号连接
    connect(delBtn, &QPushButton::clicked, this, [this]() {
        emit removeRequested(m_index);
    });
    connect(m_typeBtn1, &TypeButton::typeChanged, this, &SpiritRow::typesChanged);
    connect(m_typeBtn2, &TypeButton::typeChanged, this, &SpiritRow::typesChanged);

    // 设置行样式
    setStyleSheet(QStringLiteral(
        "QWidget { background-color: #FFFFFF; border-radius: 8px; }"
    ));
}

QStringList SpiritRow::getTypes() const
{
    QStringList types;
    if (m_typeBtn1->hasType()) {
        types << m_typeBtn1->typeName();
    }
    if (m_typeBtn2->hasType()) {
        types << m_typeBtn2->typeName();
    }
    return types;
}

bool SpiritRow::isValid() const
{
    return m_typeBtn1->hasType();
}

void SpiritRow::setIndex(int idx)
{
    m_index = idx;
}
