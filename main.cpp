#include <QApplication>
#include <QPushButton>
#include <QtGui/QtGui>

#include <QFuture>
#include <QtConcurrent/QtConcurrent>

#include <QGraphicsScene>
#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>


#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QFileDialog>


QImage blurImage (QImage sourse, int blurRadius)
{
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(sourse));

    auto *blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(blurRadius);
    item.setGraphicsEffect(blur);
    scene.addItem(&item);
    QImage result (sourse.size(), QImage::Format_ARGB32);
    result.fill(Qt::transparent);
    QPainter painter(&result);
    scene.render(&painter, QRectF(),QRectF(0,0,sourse.width(),sourse.height()));
    return result;
};

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    auto* window = new QWidget;
    auto* label = new QLabel(window);
    auto* slider = new QSlider(window);
    auto* button = new QPushButton("open",window);
    auto* layout = new QGridLayout(window);

    QString filePath;
    slider->setOrientation(Qt::Horizontal);
    slider->setMinimum(0);
    slider->setMaximum(10);

    layout->addWidget(label,0,0);
    layout->addWidget(slider, 4,0);
    layout->addWidget(button,5,0 );

    QObject::connect(button, &QPushButton::clicked, [&filePath] {
       filePath = QFileDialog::getOpenFileName(nullptr,
           "open image", "/opt/");
   });

    QObject::connect(button, &QPushButton::clicked, [label, &filePath]()
    {
        label->setPixmap(QPixmap::fromImage(QImage(filePath)));
    });

    QObject::connect(slider, &QSlider::valueChanged,[label,&filePath, slider]()
    {
        label->setPixmap(QPixmap::fromImage(blurImage(QImage(filePath),
            slider->value()).scaled(
     label->width(),
     label->height(), Qt::KeepAspectRatio)));
    });

    window->show();
    return QApplication::exec();
}
