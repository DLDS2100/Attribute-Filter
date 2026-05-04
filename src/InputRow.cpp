#include "InputRow.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

InputRow::InputRow(int index, CalcMode mode, QWidget *parent)
    : QWidget(parent), m_index(index), m_mode(mode)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(8);

    // 精灵编号标签
    m_label = new QLabel(QString::fromUtf8(u8"精灵 %1").arg(index), this);
    m_label->setFixedWidth(60);
    m_label->setStyleSheet(QStringLiteral(
        "font-size: 14px; font-weight: bold; color: #333333;"));
    layout->addWidget(m_label);

    // 属性按钮组
    buildButtons();
    for (TypeButton *btn : m_typeBtns) {
        layout->addWidget(btn);
    }

    // 右侧弹性空间
    layout->addStretch();

    // 删除按钮
    QPushButton *delBtn = new QPushButton(QString::fromUtf8(u8"×"), this);
    delBtn->setFixedSize(32, 32);
    delBtn->setCursor(Qt::PointingHandCursor);
    delBtn->setToolTip(QString::fromUtf8(u8"移除此精灵"));
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

    setStyleSheet(QStringLiteral(
        "QWidget { background-color: #FFFFFF; border-radius: 8px; }"
    ));
}

void InputRow::buildButtons()
{
    // 清除旧按钮
    for (TypeButton *btn : m_typeBtns) {
        disconnect(btn, nullptr, this, nullptr);
        delete btn;
    }
    m_typeBtns.clear();

    int count = (m_mode == CalcMode::Defense) ? 2 : 5;

    for (int i = 0; i < count; ++i) {
        bool isOptional = (m_mode == CalcMode::Defense) ? (i > 0) : true;
        TypeButton *btn = new TypeButton(isOptional, this);
        connect(btn, &TypeButton::typeChanged, this, &InputRow::typesChanged);
        m_typeBtns.append(btn);
    }
}

QStringList InputRow::getTypes() const
{
    QStringList types;
    for (TypeButton *btn : m_typeBtns) {
        if (btn->hasType()) {
            types << btn->typeName();
        }
    }
    return types;
}

bool InputRow::isValid() const
{
    if (m_mode == CalcMode::Defense) {
        return !m_typeBtns.isEmpty() && m_typeBtns[0]->hasType();
    }
    // Offense: 允许 0 个技能
    return true;
}

void InputRow::setIndex(int idx)
{
    m_index = idx;
    if (m_label) {
        m_label->setText(QString::fromUtf8(u8"精灵 %1").arg(idx));
    }
}

void InputRow::setMode(CalcMode mode)
{
    if (m_mode == mode) return;
    m_mode = mode;
    buildButtons();
}

int InputRow::maxSlots() const
{
    return (m_mode == CalcMode::Defense) ? 2 : 5;
}
