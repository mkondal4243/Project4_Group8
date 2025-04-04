#include "logtransferwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPalette>
#include <QLinearGradient>
#include <QFrame>
#include <QDateTime>
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QTimer>
#include <QMovie>
#include <QFileDialog>
#include <QTextStream>

LogTransferWindow::LogTransferWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setFixedSize(700, 500);
    this->setWindowTitle("SecureLink – Log Transfer");

    QPalette palette;
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0.0, QColor("#f0f9ff"));
    gradient.setColorAt(1.0, QColor("#ffffff"));
    palette.setBrush(QPalette::Window, gradient);
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    QWidget *central = new QWidget(this);
    central->setStyleSheet("background-color: white; border-radius: 12px;");
    central->setFixedSize(620, 420);
    central->move((width() - central->width()) / 2, (height() - central->height()) / 2);

    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    QLabel *title = new QLabel("Secure File Transfer");
    title->setStyleSheet("font-size: 18px; font-weight: bold; color: #003147;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: #ccc;");
    layout->addWidget(line);

    logList = new QListWidget();
    logList->setStyleSheet(R"(
        QListWidget {
            background-color: #f4f7fa;
            border: none;
            padding: 10px;
            font-size: 13px;
        }
        QListWidget::item {
            color: #003147;
            padding: 4px;
        }
        QListWidget::item:selected {
            background-color: #cfd8dc;
            border-radius: 4px;
        }
    )");

    QStringList initialLogs = { "log_01.txt", "log_02.txt", "log_03.txt", "log_04.txt" };
    for (const QString &log : initialLogs) {
        QListWidgetItem *item = new QListWidgetItem();
        QLabel *label = new QLabel(log);
        label->setStyleSheet("color: #003147; padding: 4px 6px;");
        logList->addItem(item);
        logList->setItemWidget(item, label);
    }
    layout->addWidget(logList);

    // 🌀 Loading Spinner
    QLabel *spinnerLabel = new QLabel();
    QMovie *movie = new QMovie(":/spinner.gif");
    spinnerLabel->setMovie(movie);
    spinnerLabel->setAlignment(Qt::AlignCenter);
    spinnerLabel->setVisible(false);
    layout->addWidget(spinnerLabel);

    QPushButton *downloadBtn = new QPushButton("Download Logs");
    downloadBtn->setFixedHeight(40);
    downloadBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #00C853;
            color: white;
            font-weight: bold;
            font-size: 14px;
            border-radius: 8px;
        }
        QPushButton:hover {
            background-color: #00B248;
        }
    )");
    layout->addWidget(downloadBtn);

    QPushButton *backBtn = new QPushButton("Back to Dashboard");
    backBtn->setFixedHeight(30);
    backBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #546E7A;
            color: white;
            font-weight: 500;
            font-size: 13px;
            border-radius: 6px;
        }
        QPushButton:hover {
            background-color: #37474F;
        }
    )");
    layout->addWidget(backBtn);

    connect(backBtn, &QPushButton::clicked, this, [=]() {
        this->hide();
        QTimer::singleShot(300, [=]() {
            emit backToDashboardRequested();
        });
    });

    connect(downloadBtn, &QPushButton::clicked, this, [=]() {
        movie->start();
        spinnerLabel->setVisible(true);

        QTimer::singleShot(1500, [=]() {
            spinnerLabel->setVisible(false);

            QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss AP");
            QString logMsg = "✔️ Logs downloaded at " + timestamp;

            QListWidgetItem *item = new QListWidgetItem();
            QLabel *label = new QLabel(logMsg);
            label->setStyleSheet("color: #2e7d32; font-style: italic; padding: 4px 6px;");

            QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(label);
            effect->setOpacity(0.0);
            label->setGraphicsEffect(effect);

            logList->addItem(item);
            logList->setItemWidget(item, label);
            logList->scrollToBottom();

            QPropertyAnimation *fadeIn = new QPropertyAnimation(effect, "opacity");
            fadeIn->setDuration(800);
            fadeIn->setStartValue(0.0);
            fadeIn->setEndValue(1.0);
            fadeIn->start(QPropertyAnimation::DeleteWhenStopped);

            if (logList->count() > 8) {
                logList->takeItem(4);
            }

            QString filePath = QFileDialog::getSaveFileName(this, "Save Logs", "SecureLogs.txt", "Text Files (*.txt)");
            if (!filePath.isEmpty()) {
                QFile file(filePath);
                if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream out(&file);
                    for (int i = 0; i < logList->count(); ++i) {
                        QWidget *widget = logList->itemWidget(logList->item(i));
                        if (auto *lbl = qobject_cast<QLabel*>(widget)) {
                            out << lbl->text() << "\n";
                        }
                    }
                    file.close();
                }
            }
        });
    });
}
