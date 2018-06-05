#include "mainwindow.hpp"
#include "algorithms/floyd-steinberg.hpp"

#include <QFileDialog>
#include <QCameraInfo>

#include <iostream>

MainWindow::MainWindow(QWidget *parent, QImage* image, std::vector<Mollusc>* molluscs, bool useCam)
    : QMainWindow(parent)
    , m_molluscs(molluscs)
    , m_selectedMolluscIndex(0)
    , m_layout(new QGridLayout())
    , m_scrollArea(new QScrollArea())
    , m_infoWidget(new QWidget())
    , m_dWidget(new QDockWidget(this))
    , m_classLabel(new QLabel())
    , m_familyLabel(new QLabel())
    , m_genusLabel(new QLabel())
    , m_speciesLabel(new QLabel())
    , m_scientificNameLabel(new QLabel())
    , m_localityLabel(new QLabel())
    , m_dateLabel(new QLabel())
    , m_areaLabel(new QLabel())
    , m_provinceLabel(new QLabel())
    , m_countryLabel(new QLabel())
    , m_subContinentLabel(new QLabel())
    , m_continentLabel(new QLabel())
    , m_image1Label(new QLabel("image1Label"))
    , m_image2Label(new QLabel("image2Label"))
    , m_image3Label(new QLabel("image3Label"))
    , m_useCam(useCam)
{
    QGraphicsView *view = new QGraphicsView;
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QSize imageSize = image->size();
    QGraphicsScene *scene = new QGraphicsScene(0, 0, imageSize.width(), imageSize.height(), this);

    scene->addPixmap(QPixmap::fromImage(*image));
    view->setScene(scene);
    this->m_view = view;

    this->setCentralWidget(view);
}

MainWindow::~MainWindow() {}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_F11: {
            if (!this->isFullScreen()) {
                this->showFullScreen();
            }
            else {
                this->showMaximized();
            }
            break;
        }
        case Qt::Key_I: {
            this->showSnailInfo();
            break;
        }
        case Qt::Key_P: {
            this->takePicture();
            break;
        }
    }
}

void MainWindow::showSnailInfo()
{
    //TODO: Later change with selected snail index and not incremented index
    Mollusc selectedMollusc = m_molluscs->at(++m_selectedMolluscIndex);
    
    //TODO: Later change images with specific data of highlighted snail
    this->showSidebar(
        QString::fromStdString(selectedMollusc.m_class),
        QString::fromStdString(selectedMollusc.m_family),
        QString::fromStdString(selectedMollusc.m_genus),
        QString::fromStdString(selectedMollusc.m_species),
        QString::fromStdString(selectedMollusc.m_scientificName),
        QString::fromStdString(selectedMollusc.m_locality),
        QString::fromStdString(selectedMollusc.m_date),
        QString::fromStdString(selectedMollusc.m_area),
        QString::fromStdString(selectedMollusc.m_province),
        QString::fromStdString(selectedMollusc.m_country),
        QString::fromStdString(selectedMollusc.m_subContinent),
        QString::fromStdString(selectedMollusc.m_continent),
        QImage("./../data/ZMB_Mol_100073_1.png").scaledToHeight(100),
        QImage("./../data/ZMB_Mol_100073_2.png").scaledToHeight(100),
        QImage("./../data/ZMB_Mol_100073_3.png").scaledToHeight(100));
}

void MainWindow::showSidebar(
    const QString &classContent, 
    const QString &familyContent, 
    const QString &genusContent, 
    const QString &speciesContent, 
    const QString &scientificNameContent,
    const QString &localityContent,
    const QString &dateContent,
    const QString &areaContent,
    const QString &provinceContent,
    const QString &countryContent,
    const QString &subContinentContent,
    const QString &continentContent,
    const QImage &image1,
    const QImage &image2,
    const QImage &image3)
{
    m_classLabel->setText("Klasse: " + classContent);
    m_familyLabel->setText("Familie: " + familyContent);
    m_genusLabel->setText("Geschlecht: " + genusContent);
    m_speciesLabel->setText("Spezies: " + speciesContent);
    m_scientificNameLabel->setText("Wissenschatflicher Begriff: " + scientificNameContent);
    m_localityLabel->setText("Fundort: " + localityContent);
    m_dateLabel->setText("Datum: " + dateContent);
    m_areaLabel->setText("Gebiet: " + areaContent);
    m_provinceLabel->setText("Provinz: " + provinceContent);
    m_countryLabel->setText("Land: " + countryContent);
    m_subContinentLabel->setText("Teilkontinent: " + subContinentContent);
    m_continentLabel->setText("Kontinent: " + continentContent);

    m_image1Label->setPixmap(QPixmap::fromImage(image1));
    m_image2Label->setPixmap(QPixmap::fromImage(image2));
    m_image3Label->setPixmap(QPixmap::fromImage(image3));


    m_infoWidget->setLayout(m_layout);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setWidget(m_infoWidget);

    m_layout->addWidget(m_classLabel);
    m_layout->addWidget(m_familyLabel);
    m_layout->addWidget(m_genusLabel);
    m_layout->addWidget(m_speciesLabel);
    m_layout->addWidget(m_scientificNameLabel);
    m_layout->addWidget(m_localityLabel);
    m_layout->addWidget(m_dateLabel);
    m_layout->addWidget(m_areaLabel);
    m_layout->addWidget(m_provinceLabel);
    m_layout->addWidget(m_countryLabel);
    m_layout->addWidget(m_subContinentLabel);
    m_layout->addWidget(m_continentLabel);

    m_layout->addWidget(m_image1Label);
    m_layout->addWidget(m_image2Label);
    m_layout->addWidget(m_image3Label);

    m_dWidget->setWindowTitle("Informationen");
    m_dWidget->setWidget(m_scrollArea);
    m_dWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    m_dWidget->setFeatures(QDockWidget::DockWidgetClosable);
    m_dWidget->setStyleSheet("QDockWidget::title { text-align: left; background: white;}");
    this->addDockWidget(Qt::RightDockWidgetArea, m_dWidget);
}

void MainWindow::takePicture() {
    if (m_useCam && QCameraInfo::availableCameras().size() > 0) {
        std::cout << "take picture" << std::endl;
    }
    else {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"),
            "C:/",
            tr("Images (*.png *.jpg)"));

        if (fileName == "") return;

        // read input image
        QRect display = QApplication::desktop()->screenGeometry();
        QImage image = QImage(fileName).scaled(display.size(), Qt::KeepAspectRatio);

        auto mosaic = FloydSteinberg(*m_molluscs);
        auto result = mosaic.createMosaic(image, 1000);

        QSize imageSize = result->size();
        QGraphicsScene *scene = new QGraphicsScene(0, 0, imageSize.width(), imageSize.height(), this);

        scene->addPixmap(QPixmap::fromImage(*result));
        m_view->setScene(scene);
    }
}