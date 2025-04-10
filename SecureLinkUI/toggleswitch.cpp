#include "toggleswitch.h"
#include <QPainter>
#include <QColor>

ToggleSwitch::ToggleSwitch(QWidget *parent)
    : QAbstractButton(parent), m_offset(0)
{
    setCheckable(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setCursor(Qt::PointingHandCursor);

    animation = new QPropertyAnimation(this, "offset", this);
    animation->setDuration(150);
    animation->setEasingCurve(QEasingCurve::InOutQuad);

    connect(this, &QAbstractButton::toggled, [=](bool checked) {
        animation->stop();
        animation->setStartValue(m_offset);
        animation->setEndValue(checked ? width() - height() : 0);
        animation->start();
    });
}

int ToggleSwitch::offset() const {
    return m_offset;
}

void ToggleSwitch::setOffset(int offset) {
    m_offset = offset;
    update();
}

QSize ToggleSwitch::sizeHint() const {
    return QSize(60, 30);
}

void ToggleSwitch::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Background
    QRectF background(0, 0, width(), height());
    QColor bgColor = isChecked() ? QColor(255, 204, 0) : QColor(90, 90, 90);
    p.setBrush(bgColor);
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(background, height() / 2.0, height() / 2.0);

    // Circle
    QRectF circle(m_offset, 0, height(), height());
    p.setBrush(Qt::white);
    p.drawEllipse(circle);
}
