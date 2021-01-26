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

  // Draw line
  drawLineBasic(0, 0, 50, 99, foreground);

//  ui->pixelWidget->setZoom(4);

}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::drawLineBasic(int x1, int y1, int x2, int y2, const QColor &color)
{
  ui->pixelWidget->writePixel(x1, y1, color);

  float xRange = std::abs(x2 - x1) + 0.000001;
  float yRange = std::abs(y2 - y1);

  float slope = yRange / xRange;

//  for (int currentX = 0; currentX < xRange ; currentX++  ) {
//    for (int currentY = 0; currentY < yRange ; currentY++ ) {
//      if( (currentY * 1.0 / currentX * 1.0) == (slope) )
//        ui->pixelWidget->writePixel(currentX, currentY, color);
//    }
//  }

//  for (float currentX = x1; currentX < x2 ; currentX+=0.001  ) {
//      float newY = slope * currentX - slope * x1 + y1;

//      ui->pixelWidget->writePixel(qRound(currentX), qRound(newY), color);
//   }

  // needs more absolute value
    for (int currentX = x1; currentX < x2 ; currentX++  ) {
      int currentY = std::abs(qRound(y1 + yRange * (currentX - x1 * 1.0) / xRange ));
      ui->pixelWidget->writePixel(currentX, currentY, color);
    }

  ui->pixelWidget->writePixel(x2, y2, color);

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

