#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *fileMenu = new QMenu(tr("&Fichier"), this);
    QAction *openAction = new QAction(tr("&Ouvrir"), this);
    QAction *quitAction = new QAction(tr("&Quitter"), this);

    // Create the select button
    selectButton = new QPushButton("Select Image", this);
    selectFolderButton = new QPushButton("Select Folder", this);
    nextImageButton = new QPushButton("Next", this);
    previousImageButton = new QPushButton("Previous", this);

    fileMenu->addAction(openAction);
    fileMenu->addAction(quitAction);
    menuBar->addMenu(fileMenu);
    setMenuBar(menuBar);

    // Create the image label
    imageLabel = new QLabel(this);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    // Create the layout and add the select button and image label
    layout = new QHBoxLayout();
    layout->addWidget(selectButton);
    layout->addWidget(selectFolderButton);
    layout->addWidget(nextImageButton);
    layout->addWidget(previousImageButton);
    layout->addWidget(imageLabel);

    // Set the layout of the main window
    QWidget *widget = new QWidget(this);
    widget->setLayout(layout);
    setCentralWidget(widget);

    nextImageButton->hide();
    previousImageButton->hide();

    // Connecte l'action "Ouvrir" à une méthode de votre classe MainWindow
    connect(openAction, &QAction::triggered, this, &MainWindow::selectImage);

    // Connecte l'action "Quitter" à la méthode "close" de la classe QMainWindow
    connect(quitAction, &QAction::triggered, this, &QMainWindow::close);

    // Connect the select button to the selectImage slot
    connect(selectButton, &QPushButton::clicked, this, &MainWindow::selectImage);
    connect(selectFolderButton, &QPushButton::clicked, this, &MainWindow::selectFolder);
    connect(nextImageButton, &QPushButton::clicked, this, &MainWindow::nextImage);
    connect(previousImageButton, &QPushButton::clicked, this, &MainWindow::previousImage);
}

MainWindow::~MainWindow()
{
}

// Fonction pour afficher une image dans le QLabel
void MainWindow::displayImage(QString imagePath) {
    QPixmap image(imagePath);
    imageLabel->setPixmap(image);
    selectButton->hide();
    selectFolderButton->hide();
}

void MainWindow::selectImage()
{
    // Get the path of the selected image file
    QString imagePath = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));

    displayImage(imagePath);
}

QStringList imagesPathList;
void MainWindow::selectFolder() {
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QDirIterator it(folderPath, QStringList() << "*.png" << "*.jpg" << "*.bmp", QDir::Files, QDirIterator::Subdirectories);

    // Vide la liste précédente pour charger les images du nouveau dossier sélectionné
    imagesPathList.clear();

    // Ajoute chaque chemin d'accès d'image dans la liste
    while (it.hasNext()) {
        imagesPathList.append(it.next());
    }

    // Initialise l'index de l'image affichée à 0 (la première image)
    currentIndex = 0;

    // Affiche la première image
    displayImage(imagesPathList[currentIndex]);

    nextImageButton->show();
    previousImageButton->show();
}

void MainWindow::previousImage() {
    // Diminue l'index de l'image affichée de 1
    currentIndex--;

    // Si on arrive au début de la liste, on revient à la fin
    if (currentIndex < 0) {
        currentIndex = imagesPathList.count() - 1;
    }

    // Affiche l'image précédente
    displayImage(imagesPathList[currentIndex]);
}

void MainWindow::nextImage() {
    // Vérifiez s'il y a des images à afficher
    if (imagesPathList.empty()) {
        return;
    }

    // Avancez dans la liste des images et affichez la prochaine image
    currentIndex = (currentIndex + 1) % imagesPathList.size();
    QPixmap image(imagesPathList[currentIndex]);
    imageLabel->setPixmap(image);
}

