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
    QAction *enregistrer = new QAction(tr("&Enregistrer"), this);
    QAction *transparence = new QAction(tr("&Transparence"), this);
    QAction *saturationPlus = new QAction(tr("&Saturation+"), this);
    QAction *saturationMoins = new QAction(tr("&Saturation-"), this);
    QAction *contrastePlus = new QAction(tr("&Contraste+"), this);
    QAction *contrasteMoins = new QAction(tr("&Contraste-"), this);

    // Initialisation de l'architecture des menus
    setMenuBar(menuBar);
    menuBar->addMenu(menuFichier);
    menuFichier->addMenu(ouvrir);
    ouvrir->addAction(openActionImage);
    ouvrir->addAction(openActionFolder);
    menuFichier->addAction(enregistrer);
    menuBar->addAction(transparence);
    menuBar->addAction(saturationPlus);
    menuBar->addAction(saturationMoins);
    menuBar->addAction(contrastePlus);
    menuBar->addAction(contrasteMoins);

    // Connexion des menus aux fonctions
    connect(openActionImage, &QAction::triggered, this, &MainWindow::selectImage);
    connect(openActionFolder, &QAction::triggered, this, &MainWindow::selectFolder);
    connect(transparence, &QAction::triggered, this, &MainWindow::makeImageTransparent);
    connect(enregistrer, &QAction::triggered, this, &MainWindow::saveImage);
    connect(saturationPlus, &QAction::triggered, this, &MainWindow::increaseSaturation);
    connect(saturationMoins, &QAction::triggered, this, &MainWindow::decreaseSaturation);
    connect(contrastePlus, &QAction::triggered, this, &MainWindow::increaseContrast);
    connect(contrasteMoins, &QAction::triggered, this, &MainWindow::decreaseContrast);

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

// Rendre l'image transparente
void MainWindow::makeImageTransparent(){
    QPixmap currentPixmap = imageLabel->pixmap();
    QPixmap copiedPixmap = currentPixmap.copy();
    QImage image = copiedPixmap.toImage();
    image = image.convertToFormat(QImage::Format_ARGB32);
    // Réduire le alpha des pixels
    for(int x = 0; x < image.width(); x++){
        for(int y = 0; y < image.height(); y++) {
            QColor color = QColor(image.pixel(x, y));
            color.setAlpha(128);
            image.setPixelColor(x, y, color);
        }
    }
    imageLabel->setPixmap(QPixmap::fromImage(image));
}

// Enregistrer l'image affichée (préciser l'extension dans le nom du fichier)
void MainWindow::saveImage(){
    QPixmap pixmap = imageLabel->pixmap();
    if (pixmap.isNull()) {
        QMessageBox::warning(this, tr("Erreur"), tr("Aucune image à enregistrer."));
            return;
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Enregistrer l'image"), "", tr("Images (*.png *.jpg *.bmp)"));
    if (fileName.isEmpty()){
        return;
    }
    if (!pixmap.save(fileName)) {
        QMessageBox::warning(this, tr("Erreur"), tr("Impossible d'enregistrer l'image."));
    }
}

// Augmente la saturation de l'image
void MainWindow::increaseSaturation(){
    QPixmap pixmap = imageLabel->pixmap();
    if (pixmap.isNull()){
        return;
    }
    QImage image = pixmap.toImage();
    // Augmente la saturation de chaque pixel
    for(int y = 0; y < image.height(); y++){
        for(int x = 0; x < image.width(); x++){
            QColor color = QColor(image.pixel(x,y));
            int saturation = color.saturation();
            saturation += 15;
            if(saturation > 255){
                saturation = 255;
            }
            color.setHsv(color.hue(), saturation, color.value(), color.alpha());
            image.setPixelColor(x, y, color);
        }
    }
    imageLabel->setPixmap(QPixmap::fromImage(image));
}

// Diminue la saturation de l'image
void MainWindow::decreaseSaturation(){
    QPixmap pixmap = imageLabel->pixmap();
    if (pixmap.isNull()){
        return;
    }
    QImage image = pixmap.toImage();
    // Diminue la saturation de chaques pixels
    for (int y = 0; y < image.height(); y++){
        for (int x = 0; x < image.width(); x++){
            QColor color = image.pixelColor(x, y);
            // Convertit la couleur en HSL
            int h, s, l;
            color.getHsl(&h, &s, &l);
            // Diminue la saturation
            s = qMax(s - 15, 0);
            // Convertit la couleur en RGB
            color = QColor::fromHsl(h, s, l);
            image.setPixelColor(x, y, color);
        }
    }
    pixmap = QPixmap::fromImage(image);
    imageLabel->setPixmap(pixmap);
}

// Augmente les contrastes de l'image
void MainWindow::increaseContrast()
{
    QPixmap pixmap = imageLabel->pixmap();
    if (pixmap.isNull()) {
        return;
    }
    QImage image = pixmap.toImage();
    int width = image.width();
    int height = image.height();
    // Modification de tous les pixels
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            QColor color = QColor(image.pixel(x, y));
            int r = (color.red() - 128) * 1.1 + 128;
            int g = (color.green() - 128) * 1.1 + 128;
            int b = (color.blue() - 128) * 1.1 + 128;
            // Vérification que les valeurs ne dépassent pas 255 ou ne sont pas négatives
            r = qBound(0, r, 255);
            g = qBound(0, g, 255);
            b = qBound(0, b, 255);
            image.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    pixmap = QPixmap::fromImage(image);
    imageLabel->setPixmap(pixmap);
}

void MainWindow::decreaseContrast(){
    QPixmap pixmap = imageLabel->pixmap();
    if (pixmap.isNull()) {
        return;
    }
    QImage img = pixmap.toImage();
    // Modification de tous les pixels
    for(int y = 0; y < img.height(); y++){
        for(int x = 0; x < img.width(); x++){
            QRgb pixel = img.pixel(x, y);
            int red = qRed(pixel);
            int green = qGreen(pixel);
            int blue = qBlue(pixel);
            int gray = qGray(pixel);
            red = gray + 0.7 * (red - gray);
            green = gray + 0.7 * (green - gray);
            blue = gray + 0.7 * (blue - gray);
            // Vérification que les valeurs ne dépassent pas 255 ou ne sont pas négatives
            red = qBound(0, red, 255);
            green = qBound(0, green, 255);
            blue = qBound(0, blue, 255);
            img.setPixel(x, y, qRgb(red, green, blue));
        }
    }
    imageLabel->setPixmap(QPixmap::fromImage(img));
}
