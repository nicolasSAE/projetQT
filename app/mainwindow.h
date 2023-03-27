#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QDirIterator>
#include <QMenuBar>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void selectImage();
    void selectFolder();
    void displayImage(QString imagePath);
    void nextImage();
    void previousImage();
private:
    QPushButton *selectButton;
    QPushButton *selectFolderButton;
    QPushButton *nextImageButton;
    QPushButton *previousImageButton;
    QLabel *imageLabel;
    QHBoxLayout *layout;
    int currentIndex = 0;
};

#endif // MAINWINDOW_H
