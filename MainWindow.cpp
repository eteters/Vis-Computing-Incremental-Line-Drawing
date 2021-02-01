#include "MainWindow.h"
#include "ui_MainWindow.h"

// For absolute value of float, weird its not included when the version for int is
#include <cmath>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow), m_drawLineDialog(new DrawLineDialog(this))
{
  ui->setupUi(this);

  ui->actionZoom_In->setShortcut(QKeySequence::ZoomIn);
  ui->actionZoom_Out->setShortcut(QKeySequence::ZoomOut);

  setZoom(1);

  // Connect line dialog
  connect(ui->actionDraw_Line, &QAction::triggered,
          m_drawLineDialog, &DrawLineDialog::show);

  // Query colors from widget's palette
  QColor background = palette().window().color();
  QColor foreground = palette().windowText().color();

  // Clear the pixel buffer with background
  ui->pixelWidget->clear(background);

  // Get maximum extents from pixel buffer
  int maxx = ui->pixelWidget->bufferSize().width() - 1;
  int maxy = ui->pixelWidget->bufferSize().height() - 1;

  // set spin box ranges
  m_drawLineDialog->setPixelRange(0,0, maxx, maxy);

  connect(m_drawLineDialog, &DrawLineDialog::lineParametersChanged,
          this, &MainWindow::drawLineBasic);

  int xlist [3] = {10, (maxx-10) / 2, maxx - 10};
  int ylist [3] = {10, (maxy-10) / 2, maxy - 10};

  // Fun auto test cases :P
  // I feel there must be a better way to do this,
  // has C++ moved past these horrible for loops yet?
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

}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::drawLineBasic(int x1, int y1, int x2, int y2, const QColor &color)
{

  ui->pixelWidget->writePixel(x1, y1, color);

  float xRange = x2 - x1;
  float yRange = y2 - y1;

  float slope = yRange / xRange;

  int newX1 = x1, newX2 = x2;
  int newY1 = y1, newY2 = y2;

  if (slope < 0) {
    newX1 = x2, newX2 = x1;
    newY1 = y2, newY2 = y1;
  }

  if(std::abs(slope) <= 1) {

    float currentY = newY1;
    for (int currentX = newX1; currentX < newX2 ; currentX++  ) {
      currentY =  currentY + slope ;
      ui->pixelWidget->writePixel(currentX, qRound(currentY), color);
    }

  } else {

      float currentX = newX1;
      for (int currentY = newY1; currentY < newY2 ; currentY++  ) {
        currentX = currentX + (1.0f/slope) ;
        ui->pixelWidget->writePixel(qRound( currentX ), currentY, color);
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

  ui->actionActual_Size->setEnabled(ui->pixelWidget->zoom() != 1);
  ui->actionZoom_Out->setEnabled(ui->pixelWidget->zoom() != 1);

}

