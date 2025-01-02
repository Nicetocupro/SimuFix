#ifndef ENTRANCEWINDOW_H
#define ENTRANCEWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPalette>
#include <QPixmap>
#include <QGraphicsDropShadowEffect>

namespace Ui {
class entrancewindow;
}

class entrancewindow : public QWidget
{
    Q_OBJECT

public:
    explicit entrancewindow(QWidget *parent = nullptr);
    ~entrancewindow();

private slots:
    void on_enterButton_clicked();

private:
    Ui::entrancewindow *ui;
};

#endif // ENTRANCEWINDOW_H
