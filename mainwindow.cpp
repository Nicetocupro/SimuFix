#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    isDisassembling(true),
    animationTimer(new QTimer(this))
{
    ui->setupUi(this);

    // 设置窗口标题和大小
    setWindowTitle("产品维修教学系统 - 主界面");
    setFixedSize(1000, 800);

    // 设置整体背景为黑色
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(0, 0, 0)); // 纯黑色背景
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    // 设置左侧栈显示区为纯白色背景
    stackDisplay = new QListWidget(this);
    stackDisplay->setFixedSize(450, 700);
    stackDisplay->setLayoutDirection(Qt::RightToLeft); // 倒序排列
    stackDisplay->setVerticalScrollMode(QListWidget::ScrollPerPixel);
    stackDisplay->scrollToBottom(); // 一开始滚动到最底端
    stackDisplay->setStyleSheet("background-color: white; color: black; border: none;"); // 设置背景为白色，文本为黑色

    // 设置右侧控制台区域为黑色背景
    QFrame *consoleFrame = new QFrame(this);
    consoleFrame->setFixedSize(400, 700);
    consoleFrame->setStyleSheet("background-color: black; border-radius: 15px; padding: 15px; color: white;"); // 黑色背景，白色文本

    // 设置控制台输入部分为白色背景，黑色文本
    consoleInput = new QTextEdit(this);
    consoleInput->setFixedSize(350, 150);
    consoleInput->setStyleSheet("font-size: 18px; border: 2px solid #000000; border-radius: 10px; padding: 10px; background-color: white; color: black;"); // 白色背景，黑色文本

    addButton = new QPushButton("添加组件", this);
    addButton->setFixedSize(250, 50);
    addButton->setStyleSheet("font-size: 20px; color: white; background-color: #007bff; border-radius: 10px;");

    // 创建安全退出按钮
    QPushButton *exitButton = new QPushButton("安全退出", this);
    exitButton->setFixedSize(250, 50);
    exitButton->setStyleSheet("font-size: 20px; color: white; background-color: #dc3545; border-radius: 10px;");

    // 布局管理器：左右分布
    QHBoxLayout *mainLayout = new QHBoxLayout();
    QVBoxLayout *leftLayout = new QVBoxLayout();
    QVBoxLayout *rightLayout = new QVBoxLayout();

    // 左侧栈布局
    QLabel *stackLabel = new QLabel("组件栈", this);
    stackLabel->setAlignment(Qt::AlignCenter);
    stackLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: white;"); // 标题字体为白色
    leftLayout->addWidget(stackLabel);
    leftLayout->addWidget(stackDisplay);

    // 右侧控制台布局
    QLabel *consoleLabel = new QLabel("控制台", this);
    consoleLabel->setAlignment(Qt::AlignCenter);
    consoleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: white;"); // 标题字体为白色

    QVBoxLayout *consoleLayout = new QVBoxLayout(consoleFrame);
    consoleLayout->addWidget(consoleLabel);
    consoleLayout->addSpacing(20);  // 控件之间的间距
    consoleLayout->addWidget(consoleInput, 0, Qt::AlignCenter);
    consoleLayout->addSpacing(30);
    consoleLayout->addWidget(addButton, 0, Qt::AlignCenter);
    consoleLayout->addSpacing(20);  // 控件之间的间距
    consoleLayout->addWidget(exitButton, 0, Qt::AlignCenter);  // 添加退出按钮
    consoleLayout->addStretch();

    rightLayout->addWidget(consoleFrame, 0, Qt::AlignCenter);

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    centralWidget()->setLayout(mainLayout);

    // 连接按钮点击事件
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addComponent);
    connect(animationTimer, &QTimer::timeout, this, &MainWindow::updateDisassembly);

    // 连接退出按钮点击事件
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::exitApplication);

    // 连接左侧栈显示区的点击事件
    connect(stackDisplay, &QListWidget::itemClicked, this, &MainWindow::showContextMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addComponent()
{
    QString component = consoleInput->toPlainText().trimmed();
    if (!component.isEmpty()) {
        if (!componentStack.contains(component)) {
            componentStack.push(component);  // 将组件添加到栈

            // 创建新的列表项，并使其可编辑
            QListWidgetItem *item = new QListWidgetItem(component);
            item->setFlags(item->flags() | Qt::ItemIsEditable); // 允许编辑

            // 自动调整字体大小使其适应宽度
            QFont font("Arial", 18, QFont::Bold);
            QFontMetrics fm(font);
            int maxWidth = stackDisplay->width() - 20;  // 考虑到内边距和滚动条的宽度
            while (fm.horizontalAdvance(component) > maxWidth && font.pointSize() > 1) {
                font.setPointSize(font.pointSize() - 1);
                fm = QFontMetrics(font);
            }
            item->setFont(font);

            item->setTextAlignment(Qt::AlignCenter);  // 字居中
            item->setBackground(QBrush(QColor(200, 200, 200)));  // 设置背景颜色为浅灰色

            // 插入到顶部而不是底部
            stackDisplay->insertItem(0, item);

            // 滚动到顶部，确保新添加的组件显示在顶部
            stackDisplay->scrollToTop();

            consoleInput->clear();  // 清空输入框
        } else {
            // 弹出警告对话框，提示组件已存在
            QMessageBox::warning(this, "重复组件", "该组件已经存在，请输入不同的组件。");
        }
    }
}


void MainWindow::repairComponent()
{
    if (!componentStack.isEmpty()) {
        // 清除上一次的拆解记录
        disassemblyPath.clear();

        // 开始动画定时器，并且将isDisassembling设置为true，表示当前正在进行拆解
        isDisassembling = true;
        animationTimer->start(1000);  // 开始动画，每秒钟更新一次
    }
}

void MainWindow::updateDisassembly()
{
    if (isDisassembling && !componentStack.isEmpty()) {
        QString topComponent = componentStack.pop();  // 从栈中移除顶部组件
        disassemblyPath.append(topComponent);  // 将组件添加到拆解路径中
        delete stackDisplay->takeItem(0);  // 从显示中移除最顶部的组件

        // 检查移除的组件是否是要维修的组件
        if (topComponent == repairPart) {
            // 停止拆解
            isDisassembling = false;
            animationTimer->stop();

            // 弹出拆解路径对话框
            showDisassemblyDialog();
        } else if (componentStack.isEmpty()) {
            // 如果栈空了，也停止动画
            animationTimer->stop();
        }
    }
}

// MainWindow.cpp
void MainWindow::updateAssembly()
{
    if (!isDisassembling) {
        QString component = repairPart;

        // 创建新的列表项
        QListWidgetItem *item = new QListWidgetItem(component);

        // 自动调整字体大小使其适应宽度
        QFont font("Arial", 18, QFont::Bold);
        QFontMetrics fm(font);
        int maxWidth = stackDisplay->width() - 20;  // 考虑到内边距和滚动条的宽度
        while (fm.horizontalAdvance(component) > maxWidth && font.pointSize() > 1) {
            font.setPointSize(font.pointSize() - 1);
            fm = QFontMetrics(font);
        }
        item->setFont(font);

        item->setTextAlignment(Qt::AlignCenter);  // 字居中
        item->setBackground(QBrush(QColor(200, 200, 200)));  // 设置背景颜色为浅灰色

        // 插入到顶部而不是底部
        stackDisplay->insertItem(0, item);

        // 滚动到顶部，确保新添加的组件显示在顶部
        stackDisplay->scrollToTop();

        componentStack.push(component);  // 将零件重新推入栈
        animationTimer->stop();  // 停止动画
    }
}

QString MainWindow::generateDisassemblyPath()
{
    // 生成拆解路径字符串
    return disassemblyPath.join(" -> ");
}

void MainWindow::showDisassemblyDialog()
{
    // 使用QDialog来代替QMessageBox
    QDialog disassemblyDialog(this);
    disassemblyDialog.setWindowTitle("拆解路径");

    QVBoxLayout *dialogLayout = new QVBoxLayout(&disassemblyDialog);

    // 使用HTML格式化文本，调整字体和样式
    QString formattedText = "<h3 style='color:#007bff;'>拆解已经完成</h3>";
    formattedText += "<p style='font-size:14px;'>以下是拆解路径：</p>";
    formattedText += "<p style='font-size:14px; color:#555;'>" + generateDisassemblyPath() + "</p>";

    QLabel *textLabel = new QLabel(formattedText, &disassemblyDialog);
    textLabel->setTextFormat(Qt::RichText);
    textLabel->setAlignment(Qt::AlignCenter);
    dialogLayout->addWidget(textLabel);

    // 创建按钮并设置其样式
    QPushButton *installButton = new QPushButton("进行维修后安装", &disassemblyDialog);
    installButton->setStyleSheet(
        "font-size: 14px;"
        "color: white;"
        "background-color: #007bff;"
        "border: none;"
        "padding: 8px 16px;"
        "border-radius: 8px;"
        );

    // 将按钮添加到布局中，并设置按钮居中
    dialogLayout->addWidget(installButton, 0, Qt::AlignCenter);

    connect(installButton, &QPushButton::clicked, &disassemblyDialog, &QDialog::accept);

    disassemblyDialog.exec();

    startAssemblyAfterRepair();
}

void MainWindow::startAssemblyAfterRepair()
{

    // 开始安装动画，并倒序安装
    isDisassembling = false;
    animationTimer->stop();

    // 反向遍历拆解路径进行安装
    for (int i = disassemblyPath.size() - 1; i >= 0; --i) {
        QString component = disassemblyPath[i];

        // 创建新的列表项
        QListWidgetItem *item = new QListWidgetItem(component);

        // 自动调整字体大小使其适应宽度
        QFont font("Arial", 18, QFont::Bold);
        QFontMetrics fm(font);
        int maxWidth = stackDisplay->width() - 20;  // 考虑到内边距和滚动条的宽度
        while (fm.horizontalAdvance(component) > maxWidth && font.pointSize() > 1) {
            font.setPointSize(font.pointSize() - 1);
            fm = QFontMetrics(font);
        }
        item->setFont(font);

        item->setTextAlignment(Qt::AlignCenter);  // 字居中
        item->setBackground(QBrush(QColor(200, 200, 200)));  // 设置背景颜色为浅灰色

        // 插入到顶部而不是底部
        stackDisplay->insertItem(0, item);

        // 滚动到顶部，确保新添加的组件显示在顶部
        stackDisplay->scrollToTop();

        componentStack.push(component);  // 将组件重新推入栈

        // 模拟安装过程中的动画
        QEventLoop loop;
        QTimer::singleShot(1000, &loop, &QEventLoop::quit);  // 延迟1秒
        loop.exec();
    }

    // 安装完成后显示安装路径
    showAssemblyDialog();
}

void MainWindow::showAssemblyDialog()
{
    // 使用QDialog来代替QMessageBox
    QDialog assemblyDialog(this);
    assemblyDialog.setWindowTitle("安装路径");

    QVBoxLayout *dialogLayout = new QVBoxLayout(&assemblyDialog);

    // 使用HTML格式化文本，调整字体和样式
    QString formattedText = "<h3 style='color:#007bff;'>安装已经完成</h3>";
    formattedText += "<p style='font-size:14px;'>以下是安装路径：</p>";
    formattedText += "<p style='font-size:14px; color:#555;'>" + generateAssemblyPath() + "</p>";

    QLabel *textLabel = new QLabel(formattedText, &assemblyDialog);
    textLabel->setTextFormat(Qt::RichText);
    textLabel->setAlignment(Qt::AlignCenter);
    dialogLayout->addWidget(textLabel);

    // 创建按钮并设置其样式
    QPushButton *closeButton = new QPushButton("完成", &assemblyDialog);
    closeButton->setStyleSheet(
        "font-size: 14px;"
        "color: white;"
        "background-color: #007bff;"
        "border: none;"
        "padding: 8px 16px;"
        "border-radius: 8px;"
        );

    // 将按钮添加到布局中，并设置按钮居中
    dialogLayout->addWidget(closeButton, 0, Qt::AlignCenter);

    connect(closeButton, &QPushButton::clicked, &assemblyDialog, &QDialog::accept);

    assemblyDialog.exec();

    // 清除拆解路径，准备下一个任务
    disassemblyPath.clear();
}


QString MainWindow::generateAssemblyPath()
{
    // 创建一个反向的安装路径列表
    QStringList reversedPath = disassemblyPath;
    std::reverse(reversedPath.begin(), reversedPath.end());

    // 生成安装路径字符串
    return reversedPath.join(" -> ");
}


void MainWindow::showContextMenu(QListWidgetItem *item)
{
    QMenu contextMenu(this);

    QAction *renameAction = contextMenu.addAction("重命名");
    QAction *deleteAction = contextMenu.addAction("删除组件");
    QAction *repairAction = contextMenu.addAction("维修此组件");
    QAction *addUpAction = contextMenu.addAction("在此组件上方添加组件");
    QAction *addDownAction = contextMenu.addAction("在此组件下方添加组件");

    // 连接重命名事件
    connect(renameAction, &QAction::triggered, this, [this]() {
        QListWidgetItem *item = stackDisplay->currentItem();
        if (item) {
            editComponentName(item);
        }
    });

    // 连接删除事件
    connect(deleteAction, &QAction::triggered, this, [this, item]() {
        int actualIndex = componentStack.size() - 1 - stackDisplay->row(item);
        componentStack.removeAt(actualIndex);  // 从栈中移除该组件
        delete stackDisplay->takeItem(stackDisplay->row(item));  // 从显示区移除该项
    });

    // 连接维修事件
    connect(repairAction, &QAction::triggered, this, [this, item]() {
        repairPart = item->text();  // 获取需要维修的零件
        repairComponent();  // 直接调用维修功能，从栈顶开始移除
    });

    // 添加组件到此项上方
    connect(addUpAction, &QAction::triggered, this, [this, item]() {
        bool ok;
        QString newComponent = QInputDialog::getText(this, "添加组件",
                                                     "请输入新的组件名称:",
                                                     QLineEdit::Normal, "", &ok);
        if (ok && !newComponent.isEmpty()) {
            // 检查重名
            if (componentStack.contains(newComponent)) {
                QMessageBox::warning(this, "重复名称", "该名称已存在，请输入不同的名称。");
                return;
            }

            int row = stackDisplay->row(item);
            int actualIndex = componentStack.size() - row;  // 倒序插入上方元素
            componentStack.insert(actualIndex, newComponent);  // 在组件栈中插入
            QListWidgetItem *newItem = new QListWidgetItem(*item);  // 复制当前项的属性
            newItem->setText(newComponent);  // 设置新名称
            stackDisplay->insertItem(row, newItem);  // 在界面上插入
        }
    });

    // 添加组件到此项下方
    connect(addDownAction, &QAction::triggered, this, [this, item]() {
        bool ok;
        QString newComponent = QInputDialog::getText(this, "添加组件",
                                                     "请输入新的组件名称:",
                                                     QLineEdit::Normal, "", &ok);
        if (ok && !newComponent.isEmpty()) {
            // 检查重名
            if (componentStack.contains(newComponent)) {
                QMessageBox::warning(this, "重复名称", "该名称已存在，请输入不同的名称。");
                return;
            }

            int row = stackDisplay->row(item);
            int actualIndex = componentStack.size() - row - 1;  // 倒序插入下方元素
            componentStack.insert(actualIndex, newComponent);  // 在组件栈中插入
            QListWidgetItem *newItem = new QListWidgetItem(*item);  // 复制当前项的属性
            newItem->setText(newComponent);  // 设置新名称
            stackDisplay->insertItem(row + 1, newItem);  // 在界面上插入
        }
    });

    contextMenu.exec(QCursor::pos());
}


// 新增的退出函数
void MainWindow::exitApplication()
{
    // 关闭应用程序
    QApplication::quit();
}

void MainWindow::editComponentName(QListWidgetItem *item)
{
    // 启动编辑模式
    stackDisplay->editItem(item);

    // 断开之前的连接，防止重复连接
    disconnect(stackDisplay, &QListWidget::itemChanged, nullptr, nullptr);

    // 获取编辑前的原始名称
    QString originalName = item->text();

    // 连接 itemChanged 信号，更新组件栈中的名称并检查重名
    connect(stackDisplay, &QListWidget::itemChanged, this, [this, item, originalName]() {
        QString newName = item->text().trimmed();

        // 检查是否有相同名称的组件
        bool nameExists = std::any_of(componentStack.begin(), componentStack.end(),
                                      [newName, originalName](const QString &existingName) {
                                          return existingName == newName && existingName != originalName;
                                      });

        if (nameExists) {
            // 弹出警告对话框，提示名称已存在
            QMessageBox::warning(this, "重复名称", "该名称已存在，请输入不同的名称。");

            // 还原到原来的名称
            int index = stackDisplay->row(item);
            if (index >= 0 && index < componentStack.size()) {
                item->setText(originalName);
            }
        } else {
            // 更新组件栈中的名称
            int index = stackDisplay->row(item);
            int actualIndex = componentStack.size() - 1 - index;  // 计算倒序后的实际索引
            if (actualIndex >= 0 && actualIndex < componentStack.size()) {
                componentStack[actualIndex] = newName;
            }
        }
    });
}

