#include "MainWindow.h"
#include <QMessageBox>
#include <QScrollArea>
#include <QApplication>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_spiritLayout(nullptr)
    , m_addBtn(nullptr)
    , m_calcBtn(nullptr)
    , m_resultPanel(nullptr)
{
    setWindowTitle(QStringLiteral("洛克王国：世界 - 联防盲点计算器 v1.1"));
    setMinimumSize(720, 950);
    resize(720, 1000);

    // 加载属性克制表
    loadTypeChart();

    // 中央容器
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    // 标题
    QLabel *titleLabel = new QLabel(QStringLiteral("联防盲点计算器"), this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(QStringLiteral(
        "font-size: 22px; font-weight: bold; color: #333333;"
        "padding: 8px 0 4px 0;"));
    mainLayout->addWidget(titleLabel);

    QLabel *subLabel = new QLabel(QStringLiteral("输入队伍精灵属性，分析联防覆盖情况"), this);
    subLabel->setAlignment(Qt::AlignCenter);
    subLabel->setStyleSheet(QStringLiteral(
        "font-size: 12px; color: #999999; padding-bottom: 8px;"));
    mainLayout->addWidget(subLabel);

    // 分隔线
    QFrame *line1 = new QFrame(this);
    line1->setFrameShape(QFrame::HLine);
    line1->setStyleSheet(QStringLiteral("background-color: #E0E0E0; max-height: 1px;"));
    mainLayout->addWidget(line1);

    // 精灵输入区标签
    QLabel *inputTitle = new QLabel(QStringLiteral("队伍配置"), this);
    inputTitle->setStyleSheet(QStringLiteral(
        "font-size: 14px; font-weight: bold; color: #555555;"));
    mainLayout->addWidget(inputTitle);

    // 精灵输入列表
    QWidget *spiritContainer = new QWidget(this);
    m_spiritLayout = new QVBoxLayout(spiritContainer);
    m_spiritLayout->setContentsMargins(0, 0, 0, 0);
    m_spiritLayout->setSpacing(6);
    mainLayout->addWidget(spiritContainer);

    // 默认添加 1 只精灵
    onAddSpirit();

    // 添加精灵按钮
    m_addBtn = new QPushButton(QStringLiteral("+ 添加精灵"), this);
    m_addBtn->setCursor(Qt::PointingHandCursor);
    m_addBtn->setFixedHeight(36);
    m_addBtn->setStyleSheet(QStringLiteral(
        "QPushButton {"
        "   border: 2px dashed #CCCCCC;"
        "   border-radius: 8px;"
        "   background-color: transparent;"
        "   color: #888888;"
        "   font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "   border-color: #999999;"
        "   color: #555555;"
        "   background-color: #F9F9F9;"
        "}"
        "QPushButton:disabled {"
        "   border-color: #EEEEEE;"
        "   color: #DDDDDD;"
        "}"
    ));
    mainLayout->addWidget(m_addBtn);

    // 分隔线
    QFrame *line2 = new QFrame(this);
    line2->setFrameShape(QFrame::HLine);
    line2->setStyleSheet(QStringLiteral("background-color: #E0E0E0; max-height: 1px;"));
    mainLayout->addWidget(line2);

    // 计算按钮
    m_calcBtn = new QPushButton(QStringLiteral("开始计算"), this);
    m_calcBtn->setCursor(Qt::PointingHandCursor);
    m_calcBtn->setFixedHeight(42);
    m_calcBtn->setStyleSheet(QStringLiteral(
        "QPushButton {"
        "   border: none;"
        "   border-radius: 10px;"
        "   background-color: #4A90D9;"
        "   color: #FFFFFF;"
        "   font-size: 15px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #3A7BC8;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #2A6BB8;"
        "}"
    ));
    mainLayout->addWidget(m_calcBtn);

    // 分隔线
    QFrame *line3 = new QFrame(this);
    line3->setFrameShape(QFrame::HLine);
    line3->setStyleSheet(QStringLiteral("background-color: #E0E0E0; max-height: 1px;"));
    mainLayout->addWidget(line3);

    // 结果面板（放在滚动区域内）
    QScrollArea *resultScroll = new QScrollArea(this);
    resultScroll->setWidgetResizable(true);
    resultScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    resultScroll->setStyleSheet(QStringLiteral(
        "QScrollArea { border: none; background-color: transparent; }"
        "QScrollBar:vertical {"
        "   width: 6px;"
        "   background: transparent;"
        "}"
        "QScrollBar::handle:vertical {"
        "   background: #CCCCCC;"
        "   border-radius: 3px;"
        "}"
    ));

    m_resultPanel = new ResultPanel(resultScroll);
    resultScroll->setWidget(m_resultPanel);
    mainLayout->addWidget(resultScroll, 1);

    // 信号连接
    connect(m_addBtn, &QPushButton::clicked, this, &MainWindow::onAddSpirit);
    connect(m_calcBtn, &QPushButton::clicked, this, &MainWindow::onCalculate);

    updateAddButton();
}

bool MainWindow::loadTypeChart()
{
    QString jsonPath = QApplication::applicationDirPath() + "/TypeChart.json";
    return m_chart.load(jsonPath);
}

void MainWindow::onAddSpirit()
{
    if (m_spiritRows.size() >= MAX_SPIRITS) return;

    int index = m_spiritRows.size() + 1;
    SpiritRow *row = new SpiritRow(index, this);

    connect(row, &SpiritRow::removeRequested, this, &MainWindow::onRemoveSpirit);

    m_spiritRows.append(row);
    m_spiritLayout->addWidget(row);
    updateAddButton();
}

void MainWindow::onRemoveSpirit(int index)
{
    for (int i = 0; i < m_spiritRows.size(); ++i) {
        if (m_spiritRows[i]->property("rowIndex").toInt() == index ||
            i + 1 == index) {
            SpiritRow *row = m_spiritRows.takeAt(i);
            m_spiritLayout->removeWidget(row);
            delete row;
            rebuildSpiritLabels();
            updateAddButton();
            return;
        }
    }
}

void MainWindow::rebuildSpiritLabels()
{
    for (int i = 0; i < m_spiritRows.size(); ++i) {
        m_spiritRows[i]->setIndex(i + 1);
    }
}

void MainWindow::onCalculate()
{
    for (int i = 0; i < m_spiritRows.size(); ++i) {
        if (!m_spiritRows[i]->isValid()) {
            QMessageBox::warning(this, QStringLiteral("提示"),
                QStringLiteral("精灵 %1 未选择属性，请先选择至少一个属性。")
                    .arg(i + 1));
            return;
        }
    }

    if (!m_chart.isLoaded()) {
        QMessageBox::critical(this, QStringLiteral("错误"),
            QStringLiteral("属性克制表数据未加载，无法计算。"));
        return;
    }

    std::vector<QStringList> team;
    for (SpiritRow *row : m_spiritRows) {
        team.push_back(row->getTypes());
    }

    QMap<QString, double> result = m_chart.calcTeamDefense(team);
    m_resultPanel->updateResults(result);
}

void MainWindow::updateAddButton()
{
    if (!m_addBtn) return;
    m_addBtn->setEnabled(m_spiritRows.size() < MAX_SPIRITS);
    if (m_spiritRows.size() >= MAX_SPIRITS) {
        m_addBtn->setText(QStringLiteral("已满 6 只"));
    } else {
        m_addBtn->setText(QStringLiteral("+ 添加精灵"));
    }
}
