#ifndef SPIRITROW_H
#define SPIRITROW_H

#include <QWidget>
#include <QStringList>
#include "TypeButton.h"

// 单只精灵输入行：标签 + 属性1按钮 + 属性2按钮 + 删除按钮
class SpiritRow : public QWidget {
    Q_OBJECT
public:
    explicit SpiritRow(int index, QWidget *parent = nullptr);

    QStringList getTypes() const;
    bool isValid() const; // 至少选了属性1

    void setIndex(int idx);

signals:
    void removeRequested(int index);
    void typesChanged();

private:
    int m_index;
    TypeButton *m_typeBtn1;
    TypeButton *m_typeBtn2;
};

#endif // SPIRITROW_H
