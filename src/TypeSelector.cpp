#include "TypeSelector.h"
#include "TypeChart.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>
#include <QToolButton>
#include <QGuiApplication>
#include <QScreen>

TypeSelector::TypeSelector(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(QStringLiteral("选择属性"));
    setFixedSize(860, 600);
    setModal(true);

    // 窗口居中
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect sg = screen->geometry();
        move((sg.width()  - width())  / 2,
             (sg.height() - height()) / 2);
    }

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 16, 20, 20);

    QLabel *titleLabel = new QLabel(QStringLiteral("选择属性类型"), this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(QStringLiteral(
        "font-size: 18px; font-weight: bold; padding: 8px; color: #333333;"));
    mainLayout->addWidget(titleLabel);

    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(12);

    for (int i = 0; i < TYPE_NAMES.size(); ++i) {
        const QString &name = TYPE_NAMES[i];
        QToolButton *btn = createTypeButton(name);
        grid->addWidget(btn, i / 6, i % 6);
    }

    mainLayout->addLayout(grid);
}

QToolButton* TypeSelector::createTypeButton(const QString &typeName)
{
    QToolButton *btn = new QToolButton(this);
    // 增大按钮尺寸，容纳图标+下方文字
    btn->setFixedSize(120, 140);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn->setText(typeName);
    btn->setToolTip(typeName);

    // 加载属性图标并放大
    QString iconPath = TYPE_ICON_MAP.value(typeName, "");
    if (!iconPath.isEmpty()) {
        QPixmap pixmap(iconPath);
        if (!pixmap.isNull()) {
            // 图标尺寸从 48 增大到 80
            QPixmap scaled = pixmap.scaled(80, 80, Qt::KeepAspectRatio,
                                            Qt::SmoothTransformation);
            btn->setIcon(QIcon(scaled));
            btn->setIconSize(scaled.size());
        }
    }

    QString bgColor = TYPE_COLOR_MAP.value(typeName, "#CCCCCC");
    btn->setStyleSheet(QStringLiteral(
        "QToolButton {"
        "   border: 2px solid #DDDDDD;"
        "   border-radius: 10px;"
        "   background-color: %1;"
        "   color: #FFFFFF;"
        "   font-size: 13px;"
        "   font-weight: bold;"
        "   padding-bottom: 6px;"
        "}"
        "QToolButton:hover {"
        "   border: 2px solid #333333;"
        "   font-weight: bold;"
        "}"
    ).arg(bgColor));

    connect(btn, &QToolButton::clicked, this, [this, typeName]() {
        m_selected = typeName;
        emit typeSelected(typeName);
        accept();
    });

    return btn;
}
