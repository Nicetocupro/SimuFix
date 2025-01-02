#include "entrancewindow.h"
#include "ui_entrancewindow.h"
#include "mainwindow.h"


entrancewindow::entrancewindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::entrancewindow)
{
    ui->setupUi(this);

    // 设置窗口标题和固定大小
    setWindowTitle("产品维修教学系统");
    setFixedSize(1000, 800);  // 窗口更大，1000x800

    // 设置纯黑色背景
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::black);  // 设置背景为黑色
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    // 设置标题标签，扩大字体和标签尺寸
    QLabel *titleLabel = new QLabel("产品维修教学系统", this);
    titleLabel->setAlignment(Qt::AlignCenter);  // 文本居中
    titleLabel->setStyleSheet("font-size: 48px; font-weight: bold; color: white;");  // 更大字体，白色
    titleLabel->setFixedSize(600, 100);  // 设置标签大小

    // 添加阴影效果
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(25);
    shadow->setOffset(0, 7);
    shadow->setColor(QColor(0, 0, 0, 180));
    titleLabel->setGraphicsEffect(shadow);

    // 设置进入按钮
    QPushButton *enterButton = new QPushButton("进入", this);
    enterButton->setFixedSize(200, 70);  // 更大的按钮
    enterButton->setStyleSheet("font-size: 24px; color: white; background-color: #007BFF; border-radius: 35px; padding: 10px;");

    // 鼠标悬停时按钮效果
    enterButton->setStyleSheet(
        "QPushButton {"
        "   font-size: 24px; color: white; background-color: #007BFF; border-radius: 35px; padding: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0056b3;"
        "}"
        );

    // 布局管理器，设置标题和按钮居中
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(50, 50, 50, 50);  // 增加边缘间距
    layout->setSpacing(50);  // 设置组件间距
    layout->addStretch();  // 添加弹性空间
    layout->addWidget(titleLabel, 0, Qt::AlignCenter);  // 居中标题
    layout->addStretch();  // 添加弹性空间
    layout->addWidget(enterButton, 0, Qt::AlignCenter);  // 居中按钮
    layout->addStretch();  // 添加弹性空间
    setLayout(layout);

    // 连接按钮点击事件
    connect(enterButton, &QPushButton::clicked, this, &entrancewindow::on_enterButton_clicked);
}

entrancewindow::~entrancewindow()
{
    delete ui;
}

void entrancewindow::on_enterButton_clicked()
{
    // 创建并显示主窗口
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();

    // 关闭当前窗口
    this->close();
}
