#ifndef TYPESELECTOR_H
#define TYPESELECTOR_H

#include <QDialog>
#include <QToolButton>

// 18 属性选择弹窗（3x6 网格，图标+名称布局）
class TypeSelector : public QDialog {
    Q_OBJECT
public:
    explicit TypeSelector(QWidget *parent = nullptr);

    QString selectedType() const { return m_selected; }

signals:
    void typeSelected(const QString &typeName);

private:
    QString m_selected;

    QToolButton* createTypeButton(const QString &typeName);
};

#endif // TYPESELECTOR_H
