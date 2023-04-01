#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QDirIterator>
#include <QMenuBar>
#include <QHBoxLayout>

class MainWindow : public QMainWindow{

    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void selectImage();
    void selectFolder();
    void displayImage(QString imagePath);
    void previousImage();
    void nextImage();

private:
    QPushButton *selectImageButton;
    QPushButton *selectFolderButton;
    QPushButton *previousImageButton;
    QPushButton *nextImageButton;
    QLabel *imageLabel;
    int currentIndex = 0;
};

#endif // MAINWINDOW_H
