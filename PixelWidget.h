#ifndef PIXELWIDGET_H
#define PIXELWIDGET_H

#include <QImage>
#include <QSize>
#include <QWidget>

class PixelWidget : public QWidget
{
  Q_OBJECT
public:
  explicit PixelWidget(QWidget *parent = nullptr);

  QSize sizeHint() const override;

  void clear(const QColor& color);
  void writePixel(int x, int y, const QColor& color);
  QSize bufferSize() const;

  int zoom() const;

public slots:
  void setZoom(int zoom);
  void drawSplash();

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  QImage m_image;
  int m_zoom;
};

#endif // PIXELWIDGET_H
