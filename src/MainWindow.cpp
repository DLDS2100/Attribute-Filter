#include "MainWindow.h"
#include "OffenseCalculator.h"
#include <QMessageBox>
#include <QScrollBar>
#include <QApplication>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_inputLayout(nullptr)
    , m_addBtn(nullptr)
    , m_calcBtn(nullptr)
    , m_resultPanel(nullptr)
    , m_resultScroll(nullptr)
    , m_tabDefense(nullptr)
    , m_tabOffense(nullptr)
    , m_modeSubLabel(nullptr)
{
    setWindowTitle(QStringLiteral("洛克王国：世界 - 属性盲点计算器 v2.1"));
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
    QLabel *titleLabel = new QLabel(QStringLiteral("属性盲点计算器"), this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(QStringLiteral(
        "font-size: 22px; font-weight: bold; color: #333333;"
        "padding: 8px 0 4px 0;"));
    mainLayout->addWidget(titleLabel);

    // Tab 栏
    QWidget *tabBar = new QWidget(this);
    QHBoxLayout *tabLayout = new QHBoxLayout(tabBar);
    tabLayout->setContentsMargins(0, 0, 0, 0);
    tabLayout->setSpacing(0);
    tabLayout->setAlignment(Qt::AlignCenter);

    m_tabDefense = new QPushButton(QStringLiteral("联防盲点"), this);
    m_tabDefense->setFixedSize(140, 40);
    m_tabDefense->setCursor(Qt::PointingHandCursor);
    m_tabDefense->setCheckable(true);
    m_tabDefense->setChecked(true);

    m_tabOffense = new QPushButton(QStringLiteral("打击盲点"), this);
    m_tabOffense->setFixedSize(140, 40);
    m_tabOffense->setCursor(Qt::PointingHandCursor);
    m_tabOffense->setCheckable(true);

    tabLayout->addWidget(m_tabDefense);
    tabLayout->addWidget(m_tabOffense);
    mainLayout->addWidget(tabBar);

    // 模式说明
    m_modeSubLabel = new QLabel(
        QStringLiteral("输入队伍精灵属性，分析联防覆盖情况"), this);
    m_modeSubLabel->setAlignment(Qt::AlignCenter);
    m_modeSubLabel->setStyleSheet(QStringLiteral(
        "font-size: 12px; color: #999999; padding-bottom: 8px;"));
    mainLayout->addWidget(m_modeSubLabel);

    // 初始化 Tab 样式
    updateModeUI();

    // 分隔线
    QFrame *line1 = new QFrame(this);
    line1->setFrameShape(QFrame::HLine);
    line1->setStyleSheet(QStringLiteral("background-color: #E0E0E0; max-height: 1px;"));
    mainLayout->addWidget(line1);

    // 输入区标签
    QLabel *inputTitle = new QLabel(
        (m_currentMode == CalcMode::Defense)
            ? QStringLiteral("队伍配置")
            : QStringLiteral("技能配置"),
        this);
    inputTitle->setObjectName("inputTitleLabel");
    inputTitle->setStyleSheet(QStringLiteral(
        "font-size: 14px; font-weight: bold; color: #555555;"));
    mainLayout->addWidget(inputTitle);

    // 精灵输入列表
    QWidget *inputContainer = new QWidget(this);
    m_inputLayout = new QVBoxLayout(inputContainer);
    m_inputLayout->setContentsMargins(0, 0, 0, 0);
    m_inputLayout->setSpacing(6);
    mainLayout->addWidget(inputContainer);

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

    // 结果面板
    QScrollArea *resultScroll = new QScrollArea(this);
    m_resultScroll = resultScroll;
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
    connect(m_tabDefense, &QPushButton::clicked, this, &MainWindow::onModeChanged);
    connect(m_tabOffense, &QPushButton::clicked, this, &MainWindow::onModeChanged);

    updateAddButton();
}

bool MainWindow::loadTypeChart()
{
    QString jsonPath = QApplication::applicationDirPath() + "/TypeChart.json";
    return m_chart.load(jsonPath);
}

void MainWindow::onModeChanged()
{
    if (m_currentMode == CalcMode::Defense) {
        m_currentMode = CalcMode::Offense;
    } else {
        m_currentMode = CalcMode::Defense;
    }

    updateModeUI();
    rebuildInputArea();
    m_resultPanel->setMode(m_currentMode);
    m_resultPanel->clearResults();

    // 更新输入区标签
    QLabel *inputTitle = findChild<QLabel*>("inputTitleLabel");
    if (inputTitle) {
        inputTitle->setText((m_currentMode == CalcMode::Defense)
            ? QStringLiteral("队伍配置")
            : QStringLiteral("技能配置"));
    }
}

void MainWindow::updateModeUI()
{
    if (m_currentMode == CalcMode::Defense) {
        m_tabDefense->setChecked(true);
        m_tabOffense->setChecked(false);
        m_tabDefense->setStyleSheet(QStringLiteral(
            "QPushButton {"
            "   border: none;"
            "   border-radius: 10px 10px 0px 0px;"
            "   background-color: #FFFFFF;"
            "   color: #333333;"
            "   font-size: 14px;"
            "   font-weight: bold;"
            "}"
        ));
        m_tabOffense->setStyleSheet(QStringLiteral(
            "QPushButton {"
            "   border: none;"
            "   border-radius: 10px 10px 0px 0px;"
            "   background-color: #F0F0F0;"
            "   color: #999999;"
            "   font-size: 14px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #E8E8E8;"
            "   color: #666666;"
            "}"
        ));
        m_modeSubLabel->setText(
            QStringLiteral("输入队伍精灵属性，分析联防覆盖情况"));
    } else {
        m_tabOffense->setChecked(true);
        m_tabDefense->setChecked(false);
        m_tabOffense->setStyleSheet(QStringLiteral(
            "QPushButton {"
            "   border: none;"
            "   border-radius: 10px 10px 0px 0px;"
            "   background-color: #FFFFFF;"
            "   color: #333333;"
            "   font-size: 14px;"
            "   font-weight: bold;"
            "}"
        ));
        m_tabDefense->setStyleSheet(QStringLiteral(
            "QPushButton {"
            "   border: none;"
            "   border-radius: 10px 10px 0px 0px;"
            "   background-color: #F0F0F0;"
            "   color: #999999;"
            "   font-size: 14px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #E8E8E8;"
            "   color: #666666;"
            "}"
        ));
        m_modeSubLabel->setText(
            QStringLiteral("输入精灵攻击技能属性，分析打击覆盖情况"));
    }
}

void MainWindow::rebuildInputArea()
{
    // 清除旧行
    for (InputRow *row : m_inputRows) {
        m_inputLayout->removeWidget(row);
        delete row;
    }
    m_inputRows.clear();

    // 创建 1 行新行
    InputRow *row = new InputRow(1, m_currentMode, this);
    connect(row, &InputRow::removeRequested, this, &MainWindow::onRemoveSpirit);
    m_inputRows.append(row);
    m_inputLayout->addWidget(row);

    updateAddButton();
}

void MainWindow::onAddSpirit()
{
    if (m_inputRows.size() >= MAX_SPIRITS) return;

    int index = m_inputRows.size() + 1;
    InputRow *row = new InputRow(index, m_currentMode, this);
    connect(row, &InputRow::removeRequested, this, &MainWindow::onRemoveSpirit);

    m_inputRows.append(row);
    m_inputLayout->addWidget(row);
    updateAddButton();
}

void MainWindow::onRemoveSpirit(int index)
{
    for (int i = 0; i < m_inputRows.size(); ++i) {
        if (m_inputRows[i]->property("rowIndex").toInt() == index ||
            i + 1 == index) {
            InputRow *row = m_inputRows.takeAt(i);
            m_inputLayout->removeWidget(row);
            delete row;
            rebuildSpiritLabels();
            updateAddButton();
            return;
        }
    }
}

void MainWindow::rebuildSpiritLabels()
{
    for (int i = 0; i < m_inputRows.size(); ++i) {
        m_inputRows[i]->setIndex(i + 1);
    }
}

void MainWindow::onCalculate()
{
    if (!m_chart.isLoaded()) {
        QMessageBox::critical(this, QStringLiteral("错误"),
            QStringLiteral("属性克制表数据未加载，无法计算。"));
        return;
    }

    // 联防模式：每只精灵至少1个属性
    if (m_currentMode == CalcMode::Defense) {
        for (int i = 0; i < m_inputRows.size(); ++i) {
            if (!m_inputRows[i]->isValid()) {
                QMessageBox::warning(this, QStringLiteral("提示"),
                    QStringLiteral("精灵 %1 未选择属性，请先选择至少一个属性。")
                        .arg(i + 1));
                return;
            }
        }
    }

    // 收集数据
    std::vector<QStringList> team;
    for (InputRow *row : m_inputRows) {
        team.push_back(row->getTypes());
    }

    if (m_currentMode == CalcMode::Defense) {
        // 联防计算
        QMap<QString, double> result = m_chart.calcTeamDefense(team);
        m_resultPanel->updateResults(result);
    } else {
        // 打击计算
        OffenseCalculator calc(m_chart);
        QMap<QString, double> result = calc.calcTeamOffense(team);
        m_resultPanel->updateResults(result);
    }

    // 每次计算后将结果面板滚回顶部，防止图标行下移
    if (m_resultScroll) {
        m_resultScroll->verticalScrollBar()->setValue(0);
    }
}

void MainWindow::updateAddButton()
{
    if (!m_addBtn) return;
    m_addBtn->setEnabled(m_inputRows.size() < MAX_SPIRITS);
    if (m_inputRows.size() >= MAX_SPIRITS) {
        m_addBtn->setText(QStringLiteral("已满 6 只"));
    } else {
        m_addBtn->setText(QStringLiteral("+ 添加精灵"));
    }
}
