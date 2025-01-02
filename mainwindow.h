#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPalette>
#include <QPixmap>
#include <QGraphicsDropShadowEffect>
#include <QLineEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QStack>
#include <QListWidget>
#include <QTextEdit>
#include <QTimer>
#include <QMessageBox>
#include <QInputDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addComponent();  // 添加零件到栈
    void repairComponent();  // 开始维修零件
    void updateDisassembly();  // 更新拆解动画
    void updateAssembly();
    void showContextMenu(QListWidgetItem *item);
    QString generateDisassemblyPath();
    void startAssemblyAfterRepair();
    void showDisassemblyDialog();
    void exitApplication();
    QString generateAssemblyPath();
    void showAssemblyDialog();
    void editComponentName(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    QStack<QString> componentStack;  // 组件栈
    QListWidget *stackDisplay;  // 显示栈内容
    QTextEdit *consoleInput;  // 控制台输入区域
    QPushButton *addButton;
    QPushButton *repairButton;
    QString repairPart;
    bool isDisassembling;
    QTimer *animationTimer;
    QStringList disassemblyPath;
};

#endif // MAINWINDOW_H
