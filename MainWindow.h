#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();


  void drawLineBasic(int x1, int y1, int x2, int y2, const QColor& color);

public slots:
  void zoomIn();
  void zoomOut();
  void zoomActualSize();

  void setZoom(int);

private:
  Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
