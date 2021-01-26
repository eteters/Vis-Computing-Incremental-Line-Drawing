#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  ui->actionZoom_In->setShortcut(QKeySequence::ZoomIn);
  ui->actionZoom_Out->setShortcut(QKeySequence::ZoomOut);

  // Query colors from widget's palette
  QColor background = palette().window().color();
  QColor foreground = palette().windowText().color();

  // Clear the pixel buffer with background
  ui->pixelWidget->clear(background);

  // Get maximum extents from pixel buffer
  int maxx = ui->pixelWidget->bufferSize().width() - 1;
  int maxy = ui->pixelWidget->bufferSize().height() - 1;

  int xlist [3] = {10, (maxx-10) / 2, maxx - 10};
  int ylist [3] = {10, (maxy-10) / 2, maxy - 10};


  for (int i=0; i < 3; i++ ) {
    for (int j=0; j< 3; j++ ) {
      for (int i2=0; i2 < 3; i2++ ) {
        for (int j2=0; j2< 3; j2++ ) {
          qDebug() << xlist[i] << " , " << ylist[j] <<  " , " << xlist[i2] << " , " << ylist[j2] << "\n";
          drawLineBasic(xlist[i], ylist[j], xlist[i2], ylist[j2], foreground);
        }
      }
    }
  }


  // Draw line
//  drawLineBasic(0, 0, 50, 99, foreground);

//  // Test Cases
//  drawLineBasic(10, 40, 60, 40, foreground);
//  drawLineBasic(40, 10, 40, 60, foreground);

//  drawLineBasic(0, 0, maxx, maxy, foreground);

//  drawLineBasic(20, 70, 50, 99, foreground);
//  drawLineBasic(0, 0, 50, 99, foreground);

}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::drawLineBasic(int x1, int y1, int x2, int y2, const QColor &color)
{

  ui->pixelWidget->writePixel(x1, y1, color);

  float xRange = x2 - x1 + 0.0000001;
  float yRange = y2 - y1 + 0.0000001;

  float slope = yRange / xRange;

  if(std::abs(qRound(slope)) <= 1) {

    if(slope < 0) {
      for (int currentX = x2; currentX < x1 ; currentX++  ) {
        int currentY = qRound(y2 + slope * (currentX - x2 * 1.0) );
        ui->pixelWidget->writePixel(currentX, currentY, color);
      }
    } else {
      for (int currentX = x1; currentX < x2 ; currentX++  ) {
        int currentY = qRound(y1 + slope * (currentX - x1 * 1.0) );
        ui->pixelWidget->writePixel(currentX, currentY, color);
      }
    }
  } else {
    if (slope < 0) {
      for (int currentY = y2; currentY < y1 ; currentY++  ) {
        int currentX = qRound(x2 + (1/slope) * (currentY - y2 * 1.0));
        ui->pixelWidget->writePixel(currentX, currentY, color);
      }
    } else {
      for (int currentY = y1; currentY < y2 ; currentY++  ) {
        int currentX = qRound(x1 + (1/slope) * (currentY - y1 * 1.0));
        ui->pixelWidget->writePixel(currentX, currentY, color);
      }
    }
  }

  //ui->pixelWidget->writePixel(x2, y2, color);

}

void MainWindow::zoomIn()
{
  setZoom(ui->pixelWidget->zoom() * 2);
}

void MainWindow::zoomOut()
{
  setZoom(ui->pixelWidget->zoom() * 0.5f);
}

void MainWindow::zoomActualSize()
{
  setZoom(1);
}

void MainWindow::setZoom(int zoom)
{
  ui->pixelWidget->setZoom(zoom);
}

