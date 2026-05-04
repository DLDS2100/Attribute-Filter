#ifndef INPUTROW_H
#define INPUTROW_H

#include <QWidget>
#include <QLabel>
#include <QStringList>
#include <QVector>
#include "TypeChart.h"
#include "TypeButton.h"

// 通用输入行：通过 CalcMode 控制按钮数量
// Defense 模式：1~2 个属性按钮（精灵自身属性，第1个必选，第2个可选）
// Offense 模式：0~5 个属性按钮（攻击技能属性，全部可选）
class InputRow : public QWidget {
    Q_OBJECT
public:
    explicit InputRow(int index, CalcMode mode, QWidget *parent = nullptr);

    // 获取已选属性列表
    QStringList getTypes() const;

    // 校验：Defense 至少1个属性；Offense 始终 true
    bool isValid() const;

    void setIndex(int idx);
    void setMode(CalcMode mode);

    int maxSlots() const;

signals:
    void removeRequested(int index);
    void typesChanged();

private:
    int m_index;
    CalcMode m_mode;
    QVector<TypeButton*> m_typeBtns;
    QLabel *m_label = nullptr;

    void buildButtons();
};

#endif // INPUTROW_H
