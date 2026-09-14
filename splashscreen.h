/****************************************************************************
**
** SplashScreen - a small, frameless "loading" window shown while AmigaED
** starts up. On a slow machine, building ~100+ QActions/menus/toolbars
** and every editor panel from scratch can take a noticeable moment with
** nothing at all on screen yet - this gives the user something to look
** at (the AmigaED logo, a progress bar, and a changing line of text
** naming whatever startup step is running right now) instead of a
** seemingly frozen/empty window.
**
** Created in main.cpp BEFORE MainWindow is constructed (so it can appear
** before any of MainWindow's - comparatively expensive - own setup even
** begins), then handed into MainWindow's constructor, which calls
** setProgress() at each major step along the way (see
** MainWindow::updateSplash() and its call sites) before finally being
** closed and deleted by main.cpp once MainWindow itself is shown.
**
** Deliberately reflects AmigaED's OWN real startup work rather than
** simulating one with artificial delays - on a fast machine, it will
** simply flash past every step and disappear almost immediately, exactly
** as it should.
**
****************************************************************************/

#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QWidget>
#include <QString>

class QLabel;
class QProgressBar;

class SplashScreen : public QWidget
{
    Q_OBJECT

public:
    explicit SplashScreen(QWidget *parent = nullptr);

    // Advances the progress bar to 'percent' (0-100) and swaps in
    // 'statusText' as the line naming what's happening right now, then
    // forces an immediate repaint. Needed because MainWindow's entire
    // constructor runs before the Qt event loop ever starts - without
    // explicitly pumping events here, the splash would just sit there
    // showing its initial state, frozen, until startup was already over.
    void setProgress(int percent, const QString &statusText);

private:
    QLabel *p_logoLabel = nullptr;
    QProgressBar *p_progressBar = nullptr;
    QLabel *p_statusLabel = nullptr;
};

#endif // SPLASHSCREEN_H
