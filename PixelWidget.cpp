#include "PixelWidget.h"

#include <QPainter>

PixelWidget::PixelWidget(QWidget *parent) : QWidget(parent), m_zoom(1)
{
  // Create 512x512 transparent image for pixel frame buffer
  m_image = QImage(QSize(512, 512), QImage::Format_ARGB32);
  m_image.fill(Qt::transparent);

  drawSplash();
}

//
// sizeHint() returns the size of this widget so layout can be performed
//
QSize PixelWidget::sizeHint() const
{
  // Size of zoomed image
  QSize result(m_image.size() * m_zoom);

  // For large zooms
  if(m_zoom >= 4)
  {
    // Grow size to accomodate grid lines
    result += QSize(1, 1);
  }

  return result;
}

//
// clear() sets all pixels to color
//
void PixelWidget::clear(const QColor &color)
{
  m_image.fill(color);
  update();
}

//
// writePixel() sets the pixel with x,y coordinates to color
//
// Coordinate system has 0,0 in the upper left
//
void PixelWidget::writePixel(int x, int y, const QColor &color)
{
  m_image.setPixelColor(x, y, color);
}

// Return the number of available pixels in x and y for the buffer
QSize PixelWidget::bufferSize() const
{
  return m_image.size();
}

// Returns the current zoom amount
int PixelWidget::zoom() const
{
  return m_zoom;
}

//
// setZoom() is a public slot used to change the zoom level of the pixel buffer
//
void PixelWidget::setZoom(int zoom)
{
  // Only update on change
  if(m_zoom != zoom)
  {
    // Ensure zoom is 1 or greater
    m_zoom = qMax(1, zoom);

    // Notify layouts that size has changed
    updateGeometry();

    // Force widget to re-evaluate size
    adjustSize();
  }
}

// Iteratively find a font small enough to fit the text in the provided
// rectangle. Once a suitable size is found, render the text with the painter
// and return the resulting bounding box. It's possible the text can't fit, but
// no there is no return indicating a failure.
QRect drawTextToFit(QPainter* painter, const QRect& rect, int flags,
                    const QString& text)
{
  painter->save();

  QFont font = painter->font();
  font.setPixelSize(rect.height());

  QRect result;
  do {
    font.setPixelSize(qMax(font.pixelSize() - 1, 1));
    painter->setFont(font);
    result = painter->boundingRect(rect, flags, text);

    // Emergency stop because box is too small to fit text
    if(font.pixelSize() == 1) break;

  } while (result.width() >= rect.width() || result.height() >= rect.height());

  painter->drawText(rect, flags, text, &result);

  painter->restore();

  return result;
}

//
// drawSplash() annotates buffer with coordinate axes, origin, max extents,
// displays text with course info, and creates some line art to look cool.
//
void PixelWidget::drawSplash()
{
  // Clear buffer to background color
  clear(palette().window().color());

  QPainter painter(&m_image);

  // Draw lines in lower left corner
  painter.setPen(palette().mid().color());

  // Max pixel coordinates for x and y
  int maxx = m_image.width() - 1;
  int maxy = m_image.height() - 1;

  int step = qMin(maxx, maxy)/32;
  for(int i = step; i <= qMin(maxx, maxy); i += step)
  {
    painter.drawLine(QLine(0, i, i, maxy));
  }

  // Draw rectangle showing bounds of pixel buffer
  painter.setPen(palette().windowText().color());

  // Subtract pen width from size of rectangle
  int penWidth = painter.pen().width();
  painter.drawRect(m_image.rect().adjusted(0, 0, -penWidth, -penWidth));

  // Shrink drawing area
  int margin = 5;
  QRect inset = m_image.rect().marginsRemoved({margin, margin, margin, margin});

  // Annotate origin and max extents of buffer
  QString position = QString("(%1, %2)")
      .arg(m_image.rect().left())
      .arg(m_image.rect().top());

  QRectF upperLeft;
  painter.drawText(inset, Qt::AlignLeft | Qt::AlignTop, position, &upperLeft);

  position = QString("(%1, %2)")
        .arg(m_image.rect().right())
        .arg(m_image.rect().bottom());

  painter.drawText(inset, Qt::AlignRight | Qt::AlignBottom, position);

  // Draw red x axis
  QLine line(upperLeft.right() + margin, upperLeft.center().y(),
             inset.right(), upperLeft.center().y());
  painter.setPen(Qt::red);
  painter.setBrush(Qt::red);
  painter.drawLine(line);
  // Draw arrow head
  painter.drawLine(line.p2(), line.p2() - QPoint(5, -5));
  painter.drawLine(line.p2(), line.p2() - QPoint(5, 5));

  // Draw green y axis
  painter.setPen(Qt::green);
  painter.setBrush(Qt::green);
  line = QLine(upperLeft.height(), upperLeft.bottom() + margin,
               upperLeft.height(), inset.bottom());
  painter.drawLine(line);
  // Draw arrow head
  painter.drawLine(line.p2(), line.p2() - QPoint(-5, 5));
  painter.drawLine(line.p2(), line.p2() - QPoint(5, 5));

  // Adjust drawing area a little to improve final look
  inset = inset.marginsRemoved({10, 0, 0, 10});

  margin += upperLeft.height();
  inset = inset.marginsRemoved({margin, margin, margin, margin});

  // Draw course name
  painter.setPen(palette().windowText().color());
  painter.setBrush(palette().window());
  QRect textBox = drawTextToFit(&painter, inset, Qt::AlignTop | Qt::AlignRight,
                                "Visual\nComputing");

  // Draw course number
  QRect subText(textBox.bottomLeft(), textBox.bottomRight());
  subText.setHeight(textBox.height()/8);
  drawTextToFit(&painter, subText, Qt::AlignTop | Qt::AlignRight,
                "ECE/CS 8001");

  // Draw copyright
  QRect copyright(inset.left(), inset.bottom() - subText.height(),
                  inset.width(), subText.height());
  drawTextToFit(&painter, copyright, Qt::AlignBottom | Qt::AlignRight,
                "\u00A9 2021 Joshua Fraser and K. Palaniappan\n"
                "University of Missouri \u2013 Columbia");
}

//
// paintEvent() performs the drawing of the widget
//
void PixelWidget::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event);

  QSize zoomedSize(m_image.size() * m_zoom);

  QPainter painter(this);
  // Paint image to cover the full zoomed widget creating pixelation
  painter.drawImage(QRect(QPoint(), zoomedSize), m_image, m_image.rect());

  // If zoom is large enough, draw grid
  if(m_zoom >= 4)
  {
    QColor gridColor(128, 128, 128, 128);

    painter.setPen(gridColor);

    for(int x = 0; x <= zoomedSize.width(); x += m_zoom)
      painter.drawLine(x, 0, x, zoomedSize.height());
    for(int y = 0; y <= zoomedSize.height(); y += m_zoom)
      painter.drawLine(0, y, zoomedSize.width(), y);
  }
}
