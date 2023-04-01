#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Création des menus
    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *menuFichier = new QMenu(tr("&Fichier"), this);
    QMenu *ouvrir = new QMenu(tr("&Ouvrir"), this);
    QAction *openActionImage = new QAction(tr("&Image"), this);
    QAction *openActionFolder = new QAction(tr("&Dossier"), this);

    // Initialisation de l'architecture des menus
    setMenuBar(menuBar);
    menuBar->addMenu(menuFichier);
    menuFichier->addMenu(ouvrir);
    ouvrir->addAction(openActionImage);
    ouvrir->addAction(openActionFolder);

    // Connexion des menus aux fonctions
    connect(openActionImage, &QAction::triggered, this, &MainWindow::selectImage);
    connect(openActionFolder, &QAction::triggered, this, &MainWindow::selectFolder);

    // Création des boutons
    selectImageButton = new QPushButton("Select Image", this);
    selectFolderButton = new QPushButton("Select Folder", this);
    previousImageButton = new QPushButton("Previous", this);
    nextImageButton = new QPushButton("Next", this);

    // Création de la zone d'image
    imageLabel = new QLabel(this);

    // Création du layout des boutons previous et next
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(previousImageButton);
    buttonsLayout->addWidget(nextImageButton);
    buttonsLayout->addStretch();

    // Création du layout des boutons select
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(imageLabel);
    mainLayout->addWidget(selectImageButton);
    mainLayout->addWidget(selectFolderButton);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonsLayout);

    // Initialisation du layout de mainWindow
    QWidget *widget = new QWidget(this);
    widget->setLayout(mainLayout);
    setCentralWidget(widget);
    // cache les boutons previous et next tant qu'un dossier n'a pas été sélectionné
    previousImageButton->hide();
    nextImageButton->hide();

    // Connexion des boutons aux fonctions
    connect(selectImageButton, &QPushButton::clicked, this, &MainWindow::selectImage);
    connect(selectFolderButton, &QPushButton::clicked, this, &MainWindow::selectFolder);
    connect(previousImageButton, &QPushButton::clicked, this, &MainWindow::previousImage);
    connect(nextImageButton, &QPushButton::clicked, this, &MainWindow::nextImage);
}

MainWindow::~MainWindow()
{
}

// Fonction d'affichage de l'image dans le Qlabel
void MainWindow::displayImage(QString imagePath){
    if(!imagePath.isEmpty()){
        QImage image(imagePath);
        imageLabel->setPixmap(QPixmap::fromImage(image));
        selectImageButton->hide();
        selectFolderButton->hide();
        // Adaptation de la taille de la fenêtre en fonction de la taille de l'image
        int width = image.size().width();
        int height = image.size().height();
        setFixedSize(width, height);
    }
}

// Fonction pour choisir une image précise
void MainWindow::selectImage(){
    QString imagePath = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.jpeg *.JPG *.gif)"));
    displayImage(imagePath);
}

// Fonction pour choisir un dossier
QStringList imagesPathList;
void MainWindow::selectFolder(){
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (folderPath.isEmpty()){
        return;
    }
    QDirIterator it(folderPath, QStringList() << "*.png" << "*.jpg" << "*jpeg" << "*.JPG" << "*.gif", QDir::Files, QDirIterator::Subdirectories);
    // Vide la liste précédente pour charger les images du nouveau dossier sélectionné
    imagesPathList.clear();
    // Ajoute chaques paths dans la liste
    while (it.hasNext()){
        imagesPathList.append(it.next());
    }
    // Initialise l'index de l'image affichée à 0
    currentIndex = 0;
    displayImage(imagesPathList[currentIndex]);
    nextImageButton->show();
    previousImageButton->show();
}

// Fonction pour parcourir le dossier vers l'arrière
void MainWindow::previousImage(){
    if (imagesPathList.empty()){
        return;
    }
    currentIndex--;
    if (currentIndex < 0){
        currentIndex = imagesPathList.count() - 1;
    }
    displayImage(imagesPathList[currentIndex]);
}

// Fonction pour parcourir le dossier vers l'avant
void MainWindow::nextImage(){
    if (imagesPathList.empty()){
        return;
    }
    currentIndex++;
    if (currentIndex >= imagesPathList.count()){
        currentIndex = 0;
    }
    displayImage(imagesPathList[currentIndex]);
}
