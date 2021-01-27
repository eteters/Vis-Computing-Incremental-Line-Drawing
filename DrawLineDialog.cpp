#include "DrawLineDialog.h"
#include "ui_DrawLineDialog.h"

DrawLineDialog::DrawLineDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DrawLineDialog)
{
  ui->setupUi(this);
}

DrawLineDialog::~DrawLineDialog()
{
  delete ui;
}
