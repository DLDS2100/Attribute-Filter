#include "TypeButton.h"
#include "TypeSelector.h"
#include "TypeChart.h"
#include <QMouseEvent>
#include <QPixmap>

TypeButton::TypeButton(bool isOptional, QWidget *parent)
    : QPushButton(parent)
    , m_isOptional(isOptional)
{
    setFixedSize(80, 44);
    setCursor(Qt::PointingHandCursor);
    updateAppearance();
}

void TypeButton::updateAppearance()
{
    if (m_typeName.isEmpty()) {
        setText(m_isOptional ? QStringLiteral("+ 第二属性")
                             : QStringLiteral("选择属性"));
        setIcon(QIcon());
        setStyleSheet(QStringLiteral(
            "QPushButton {"
            "   border: 2px dashed #AAAAAA;"
            "   border-radius: 8px;"
            "   background-color: #F5F5F5;"
            "   color: #888888;"
            "   font-size: 11px;"
            "}"
            "QPushButton:hover {"
            "   border-color: #666666;"
            "   background-color: #EEEEEE;"
            "}"
        ));
    } else {
        setText(m_typeName);
        QString iconPath = TYPE_ICON_MAP.value(m_typeName, "");
        if (!iconPath.isEmpty()) {
            QPixmap pixmap(iconPath);
            if (!pixmap.isNull()) {
                setIcon(QIcon(pixmap));
                setIconSize(QSize(24, 24));
            }
        }
        QString bgColor = TYPE_COLOR_MAP.value(m_typeName, "#CCCCCC");
        setStyleSheet(QStringLiteral(
            "QPushButton {"
            "   border: 2px solid #CCCCCC;"
            "   border-radius: 8px;"
            "   background-color: %1;"
            "   color: #FFFFFF;"
            "   font-size: 12px;"
            "   font-weight: bold;"
            "   text-shadow: 1px 1px 2px rgba(0,0,0,0.5);"
            "}"
            "QPushButton:hover {"
            "   border-color: #333333;"
            "}"
        ).arg(bgColor));
    }
}

void TypeButton::setType(const QString &name)
{
    if (m_typeName != name) {
        m_typeName = name;
        updateAppearance();
        emit typeChanged(name);
    }
}

void TypeButton::clearType()
{
    if (!m_typeName.isEmpty()) {
        m_typeName.clear();
        updateAppearance();
        emit typeChanged("");
    }
}

void TypeButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton && m_isOptional) {
        clearType();
        return;
    }

    if (event->button() == Qt::LeftButton) {
        TypeSelector dialog(this);
        // 正确居中：以父窗口中心为参考
        QWidget *top = this;
        while (top->parentWidget()) top = top->parentWidget();
        QPoint winCenter = top->mapToGlobal(
            QPoint(top->width() / 2, top->height() / 2));
        dialog.move(winCenter.x() - dialog.width() / 2,
                    winCenter.y() - dialog.height() / 2);

        if (dialog.exec() == QDialog::Accepted) {
            setType(dialog.selectedType());
        }
        return;
    }

    QPushButton::mousePressEvent(event);
}
