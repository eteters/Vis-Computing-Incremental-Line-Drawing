#ifndef DRAWLINEDIALOG_H
#define DRAWLINEDIALOG_H

#include <QDialog>

namespace Ui {
class DrawLineDialog;
}

class DrawLineDialog : public QDialog
{
  Q_OBJECT

public:
  explicit DrawLineDialog(QWidget *parent = nullptr);
  ~DrawLineDialog();

public slots:
  void setPixelRange(int minx, int miny, int maxx, int maxy);

  void apply();

signals:
  void lineParametersChanged(int, int, int, int, const QColor);

private:
  Ui::DrawLineDialog *ui;
};

#endif // DRAWLINEDIALOG_H
