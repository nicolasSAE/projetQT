#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QHBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void selectImage();
    void selectFolder();
private:
    QPushButton *selectButton;
    QPushButton *selectButtonFolder;
    QLabel *imageLabel;
    QHBoxLayout *layout;
};

#endif // MAINWINDOW_H
