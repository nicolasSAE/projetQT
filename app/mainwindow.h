#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QDirIterator>
#include <QMenuBar>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPainter>

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
    void makeImageTransparent();
    void saveImage();
    void increaseSaturation();
    void decreaseSaturation();
    void increaseContrast();
    void decreaseContrast();

private:
    QPushButton *selectImageButton;
    QPushButton *selectFolderButton;
    QPushButton *previousImageButton;
    QPushButton *nextImageButton;
    QLabel *imageLabel;
    int currentIndex = 0;
};

#endif // MAINWINDOW_H
