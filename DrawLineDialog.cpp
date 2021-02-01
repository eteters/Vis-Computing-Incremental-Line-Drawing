#include "DrawLineDialog.h"
#include "ui_DrawLineDialog.h"

#include <QDebug>

DrawLineDialog::DrawLineDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DrawLineDialog)
{
  ui->setupUi(this);

  connect(this, &DrawLineDialog::accepted, this, &DrawLineDialog::apply);
}

DrawLineDialog::~DrawLineDialog()
{
  delete ui;
}

void DrawLineDialog::setPixelRange(int minx, int miny, int maxx, int maxy)
{
  ui->x1SpinBox->setRange(minx, maxx);
  ui->y1SpinBox->setRange(miny, maxy);
  ui->x2SpinBox->setRange(minx, maxx);
  ui->y2SpinBox->setRange(miny, maxy);
}

void DrawLineDialog::apply()
{
  int x1 = ui->x1SpinBox->value();
  int y1 = ui->y1SpinBox->value();
  int x2 = ui->x2SpinBox->value();
  int y2 = ui->y2SpinBox->value();

  QColor color = palette().windowText().color();

  emit lineParametersChanged(x1, y1, x2, y2, color);
}
