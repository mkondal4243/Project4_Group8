#include "motionalertswidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateTime>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QRandomGenerator>

MotionAlertsWidget::MotionAlertsWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *title = new QLabel("Motion Alert Monitoring", this);
    title->setStyleSheet("font-size: 22px; font-weight: bold; color: gold;");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    QLabel *desc = new QLabel("Live alerts from simulated motion detection system.", this);
    desc->setStyleSheet("color: #aaa; font-size: 13px;");
    desc->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(desc);

    motionStatus = new QLabel("✅ System Idle – No Motion", this);
    motionStatus->setStyleSheet("font-size: 15px; color: lightgreen; border: 1px solid #444; padding: 12px; border-radius: 8px;");
    motionStatus->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(motionStatus);

    QHBoxLayout *motionLayout = new QHBoxLayout();
    motionLabel = new QLabel("Motion Sensitivity (5)");
    motionLabel->setStyleSheet("color: white;");
    motionSlider = new QSlider(Qt::Horizontal);
    motionSlider->setRange(1, 10);
    motionSlider->setValue(5);
    connect(motionSlider, &QSlider::valueChanged, this, &MotionAlertsWidget::updateMotionLabel);
    motionLayout->addWidget(motionLabel);
    motionLayout->addWidget(motionSlider);
    mainLayout->addLayout(motionLayout);

    QHBoxLayout *noiseLayout = new QHBoxLayout();
    noiseLabel = new QLabel("Noise Sensitivity (5)");
    noiseLabel->setStyleSheet("color: white;");
    noiseSlider = new QSlider(Qt::Horizontal);
    noiseSlider->setRange(1, 10);
    noiseSlider->setValue(5);
    connect(noiseSlider, &QSlider::valueChanged, this, &MotionAlertsWidget::updateNoiseLabel);
    noiseLayout->addWidget(noiseLabel);
    noiseLayout->addWidget(noiseSlider);
    mainLayout->addLayout(noiseLayout);

    exportButton = new QPushButton("Export Alerts");
    exportButton->setStyleSheet("padding: 8px 16px; background-color: #444; color: white; border-radius: 8px;");
    connect(exportButton, &QPushButton::clicked, this, &MotionAlertsWidget::exportAlertsToFile);
    mainLayout->addWidget(exportButton, 0, Qt::AlignCenter);

    lastExportLabel = new QLabel("No exports yet.");
    lastExportLabel->setStyleSheet("color: #aaa; font-size: 12px;");
    lastExportLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(lastExportLabel);

    motionTimer = new QTimer(this);
    connect(motionTimer, &QTimer::timeout, this, &MotionAlertsWidget::simulateMotionEvent);
    motionTimer->start(10000);

    setLayout(mainLayout);
    setStyleSheet("background-color: #2b2b2b;");
}

void MotionAlertsWidget::simulateMotionEvent()
{
    QStringList locations = {"Living Room", "Garage", "Front Door"};
    QString location = locations.at(QRandomGenerator::global()->bounded(locations.size()));

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString alert = QString("🚨 Motion Detected at %1 — %2 (Motion %3 | Noise %4)")
                        .arg(location)
                        .arg(timestamp)
                        .arg(motionSlider->value())
                        .arg(noiseSlider->value());

    motionStatus->setText(alert);
    motionStatus->setStyleSheet("font-size: 15px; color: red; background-color: #220000; padding: 12px; border-radius: 8px;");
    alertHistory.append(alert);
}

void MotionAlertsWidget::exportAlertsToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export Alerts", "motion_alerts.txt", "Text Files (*.txt)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            for (const QString &entry : alertHistory) {
                out << entry << "\n";
            }
            file.close();
            lastExportLabel->setText("✅ Exported " + QString::number(alertHistory.size()) + " alerts.");
        } else {
            QMessageBox::warning(this, "Error", "Failed to save file.");
        }
    }
}

void MotionAlertsWidget::updateMotionLabel(int value)
{
    motionLabel->setText(QString("Motion Sensitivity (%1)").arg(value));
}

void MotionAlertsWidget::updateNoiseLabel(int value)
{
    noiseLabel->setText(QString("Noise Sensitivity (%1)").arg(value));
}
