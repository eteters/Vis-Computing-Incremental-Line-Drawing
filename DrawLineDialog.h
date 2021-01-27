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

private:
  Ui::DrawLineDialog *ui;
};

#endif // DRAWLINEDIALOG_H
