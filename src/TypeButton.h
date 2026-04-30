#ifndef TYPEBUTTON_H
#define TYPEBUTTON_H

#include <QPushButton>

// 属性显示/选择按钮，点击打开属性选择器
class TypeButton : public QPushButton {
    Q_OBJECT
public:
    explicit TypeButton(bool isOptional = false, QWidget *parent = nullptr);

    QString typeName() const { return m_typeName; }
    bool hasType() const { return !m_typeName.isEmpty(); }

    void setType(const QString &name);
    void clearType();

signals:
    void typeChanged(const QString &typeName);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QString m_typeName;
    bool m_isOptional;
    void updateAppearance();
};

#endif // TYPEBUTTON_H
