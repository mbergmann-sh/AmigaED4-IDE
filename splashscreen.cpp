#include "splashscreen.h"

#include <QApplication>
#include <QEventLoop>
#include <QGuiApplication>
#include <QLabel>
#include <QPixmap>
#include <QProgressBar>
#include <QScreen>
#include <QVBoxLayout>

SplashScreen::SplashScreen(QWidget *parent)
    : QWidget(parent, Qt::SplashScreen | Qt::WindowStaysOnTopHint)
{
    // Qt::SplashScreen is the purpose-built window type for exactly this -
    // frameless, no taskbar entry, and already treated specially by every
    // window manager AmigaED targets. WindowStaysOnTopHint is added on top
    // since startup below blocks the event loop for stretches at a time
    // (see setProgress()) - without it, some window managers can let
    // another window steal focus and bury the splash mid-startup.
    setFixedSize(440, 260);
    setAttribute(Qt::WA_DeleteOnClose,
                 false); // main.cpp owns and deletes this explicitly, once MainWindow is shown

    // Dark, Amiga-Workbench-ish backdrop - the logo itself (aced-logo.png)
    // has a transparent background, so it sits directly on this rather
    // than in its own separate panel.
    setStyleSheet("SplashScreen { background-color: #202225; border: 1px solid #444851; }"
                  "QProgressBar { background-color: #2c2f33; border: 1px solid #444851; "
                  "border-radius: 3px; height: 14px; }"
                  "QProgressBar::chunk { background-color: #e07b1a; border-radius: 2px; }"
                  "QLabel#splashStatus { color: #d5d5d5; }");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 24, 24, 18);
    layout->setSpacing(14);

    p_logoLabel = new QLabel(this);
    QPixmap logo(":/images/aced-logo.png");
    if (!logo.isNull())
        p_logoLabel->setPixmap(logo.scaledToWidth(360, Qt::SmoothTransformation));
    p_logoLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(p_logoLabel);

    layout->addStretch(1);

    p_progressBar = new QProgressBar(this);
    p_progressBar->setRange(0, 100);
    p_progressBar->setValue(0);
    p_progressBar->setTextVisible(false);
    layout->addWidget(p_progressBar);

    p_statusLabel = new QLabel(this);
    p_statusLabel->setObjectName("splashStatus");
    p_statusLabel->setAlignment(Qt::AlignCenter);
    // No tr() here yet - MainWindow's constructor hasn't run readSettings()
    // at the point main.cpp constructs this, so no translator is installed
    // (and may never need to be, on a fast machine/default language) - see
    // setProgress()'s own callers for where localized text starts.
    p_statusLabel->setText(QStringLiteral("AmigaED 4.0"));
    layout->addWidget(p_statusLabel);

    // Center on whichever screen the app will actually appear on, rather
    // than always screen 0 - matters on a multi-monitor setup.
    const QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        const QRect avail = screen->availableGeometry();
        move(avail.center() - QPoint(width() / 2, height() / 2));
    }
}

void SplashScreen::setProgress(int percent, const QString &statusText)
{
    p_progressBar->setValue(percent);
    p_statusLabel->setText(statusText);

    // MainWindow's constructor (and everything it calls, e.g.
    // initializeGUI()/createActions()/createMenus()/createToolBars()) runs
    // to completion before the Qt event loop ever starts - so without an
    // explicit repaint + processEvents() here, this splash would just show
    // its very first setProgress() call, frozen, until all of that work
    // was already done and MainWindow was about to appear anyway.
    // ExcludeUserInputEvents: startup is not meant to be interactive -
    // this only keeps the splash itself painting, it doesn't turn it into
    // an accidental nested event loop that could process a stray click.
    repaint();
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
}
