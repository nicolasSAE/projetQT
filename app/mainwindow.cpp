#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Create the select button
    selectButton = new QPushButton("Select Image", this);
    selectButtonFolder = new QPushButton("Select Folder", this);

    // Create the image label
    imageLabel = new QLabel(this);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    // Create the layout and add the select button and image label
    layout = new QHBoxLayout();
    layout->addWidget(selectButton);
    layout->addWidget(selectButtonFolder);
    layout->addWidget(imageLabel);

    // Set the layout of the main window
    QWidget *widget = new QWidget(this);
    widget->setLayout(layout);
    setCentralWidget(widget);

    // Connect the select button to the selectImage slot
    connect(selectButton, &QPushButton::clicked, this, &MainWindow::selectImage);
    connect(selectButtonFolder, &QPushButton::clicked, this, &MainWindow::selectFolder);
}

MainWindow::~MainWindow()
{
}

void MainWindow::selectImage()
{
    // Get the path of the selected image file
    QString imagePath = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));

    // Load the selected image into the image label
    QPixmap image(imagePath);
    imageLabel->setPixmap(image);
}

void MainWindow::selectFolder()
{
    // Get the path of the selected image file
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Select Folder"), "");

    QDir directory(folderPath);
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp";
    QStringList images = directory.entryList(filters, QDir::Files);
}
