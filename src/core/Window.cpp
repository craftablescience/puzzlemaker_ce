#include "Window.h"

#include <optional>

#include <QActionGroup>
#include <QApplication>
#include <QCloseEvent>
#include <QDirIterator>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMenuBar>
#include <QMessageBox>
#include <QStyle>
#include <QStyleFactory>

#include "../config/Config.h"
#include "../config/Options.h"

Window::Window(QWidget* parent)
        : QMainWindow(parent)
        , modified(false) {
    this->setWindowTitle(PUZZLEMAKER_CE_FULL_TITLE);
    this->setWindowIcon(QIcon(":/icon.png"));
    this->setMinimumSize(900, 500);

    // File menu
    auto* fileMenu = this->menuBar()->addMenu(tr("&File"));
    this->newFileAction = fileMenu->addAction(this->style()->standardIcon(QStyle::SP_FileIcon), tr("&New..."), Qt::CTRL | Qt::Key_N, [this] {
        this->newFile(false);
    });
    this->openFileAction = fileMenu->addAction(this->style()->standardIcon(QStyle::SP_DirIcon), tr("&Open..."), Qt::CTRL | Qt::Key_O, [this] {
        this->openFile();
    });
    this->saveFileAction = fileMenu->addAction(this->style()->standardIcon(QStyle::SP_DialogSaveButton), tr("&Save"), Qt::CTRL | Qt::Key_S, [this] {
        this->saveFile();
    });
    this->saveFileAction->setDisabled(true);

    this->saveFileAsAction = fileMenu->addAction(this->style()->standardIcon(QStyle::SP_DialogSaveButton), tr("Save &As..."), Qt::CTRL | Qt::SHIFT | Qt::Key_S, [this] {
        this->saveFileAs();
    });
    this->saveFileAsAction->setDisabled(true);

    this->closeFileAction = fileMenu->addAction(this->style()->standardIcon(QStyle::SP_BrowserReload), tr("&Close"), Qt::CTRL | Qt::Key_X, [this] {
        this->closeFile();
    });
    this->closeFileAction->setDisabled(true);

    fileMenu->addSeparator();
    fileMenu->addAction(this->style()->standardIcon(QStyle::SP_DialogCancelButton), tr("&Exit"), Qt::ALT | Qt::Key_F4, [this] {
        this->close();
    });

    // Options menu
    auto* optionsMenu = this->menuBar()->addMenu(tr("&Options"));

    auto* themeMenu = optionsMenu->addMenu(this->style()->standardIcon(QStyle::SP_DesktopIcon), tr("&Theme..."));
    auto* themeMenuGroup = new QActionGroup(this);
    themeMenuGroup->setExclusive(true);
    for (const auto& themeName : QStyleFactory::keys()) {
        auto* action = themeMenu->addAction(themeName, [=] {
            QApplication::setStyle(themeName);
            Options::set(OPT_STYLE, themeName);
        });
        action->setCheckable(true);
        if (themeName == Options::get<QString>(OPT_STYLE)) {
            action->setChecked(true);
        }
        themeMenuGroup->addAction(action);
    }

    optionsMenu->addSeparator();
    auto* optionStartMaximized = optionsMenu->addAction(tr("&Start Maximized"), [=] {
        Options::invert(OPT_START_MAXIMIZED);
    });
    optionStartMaximized->setCheckable(true);
    optionStartMaximized->setChecked(Options::get<bool>(OPT_START_MAXIMIZED));

    // Help menu
    auto* helpMenu = this->menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(this->style()->standardIcon(QStyle::SP_DialogHelpButton), tr("&About"), Qt::Key_F1, [this] {
        this->about();
    });
    helpMenu->addAction(this->style()->standardIcon(QStyle::SP_DialogHelpButton), "About &Qt", Qt::ALT | Qt::Key_F1, [this] {
        this->aboutQt();
    });

    // Call after the menu is created, it controls the visibility of the save button
    this->markModified(false);

    // todo: add widgets here

    this->clearContents();

    // Load the VPK if given one through the command-line or double-clicking a file
    // An error here means shut the application down
    const auto& args = QApplication::arguments();
    if ((args.length() > 1 && args[1].endsWith(PUZZLEMAKER_CE_PROJECT_CHAMBER_SAVE_EXTENSION) && QFile::exists(args[1])) && !this->loadFile(args[1])) {
        exit(1);
    }
}

void Window::newFile(bool fromDirectory, const QString& startPath) {
    if (this->modified && this->promptUserToKeepModifications()) {
        return;
    }
    // todo: new
}

void Window::openFile(const QString& startPath, const QString& filePath) {
	auto path = filePath;
	if (path.isEmpty()) {
		path = QFileDialog::getOpenFileName(this, tr("Open " PUZZLEMAKER_CE_PROJECT_CHAMBER_SAVE_NAME), startPath, PUZZLEMAKER_CE_PROJECT_CHAMBER_SAVE_NAME " (*" PUZZLEMAKER_CE_PROJECT_CHAMBER_SAVE_EXTENSION ");;All files (*.*)");
	}
    if (path.isEmpty()) {
        return;
    }
    if (!this->loadFile(path)) {
        this->clearContents();
    }
}

bool Window::saveFile() {
    // todo: save
    this->markModified(false);
    return true;
}

bool Window::saveFileAs() {
    // todo: save as
    this->markModified(false);
    return true;
}

void Window::closeFile() {
    this->clearContents();
}

void Window::about() {
    QString creditsText = "# " PUZZLEMAKER_CE_FULL_TITLE "\n"
                          "*Created by [craftablescience](https://github.com/craftablescience)*\n<br/>\n";
    QFile creditsFile(QCoreApplication::applicationDirPath() + "/CREDITS.md");
    if (creditsFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&creditsFile);
        while(!in.atEnd()) {
            creditsText += in.readLine() + '\n';
        }
        creditsFile.close();
    }

    QMessageBox about(this);
    about.setWindowTitle(tr("About"));
    about.setIconPixmap(QIcon(":/icon.png").pixmap(64, 64));
    about.setTextFormat(Qt::TextFormat::MarkdownText);
    about.setText(creditsText);
    about.exec();
}

void Window::aboutQt() {
    QMessageBox::aboutQt(this);
}

void Window::markModified(bool modified_) {
    this->modified = modified_;

    if (this->modified) {
        this->setWindowTitle(PUZZLEMAKER_CE_FULL_TITLE " (*)");
    } else {
        this->setWindowTitle(PUZZLEMAKER_CE_FULL_TITLE);
    }

    this->saveFileAction->setDisabled(!this->modified);
}

bool Window::promptUserToKeepModifications() {
    auto response = QMessageBox::warning(this,
            tr("Save changes?"),
            tr("This VPK has unsaved changes! Would you like to save these changes first?"),
            QMessageBox::Ok | QMessageBox::Discard | QMessageBox::Cancel);
    switch (response) {
        case QMessageBox::Cancel:
            return true;
        case QMessageBox::Discard:
            return false;
        case QMessageBox::Ok:
            this->saveFile();
            return false;
        default:
            break;
    }
    return true;
}

void Window::clearContents() {
    if (this->modified && this->promptUserToKeepModifications()) {
        return;
    }
    this->markModified(false);
    this->freezeActions(true, false); // Leave create/open unfrozen
}

void Window::closeEvent(QCloseEvent* event) {
    if (this->modified && this->promptUserToKeepModifications()) {
        event->ignore();
        return;
    }
    event->accept();
}

void Window::freezeActions(bool freeze, bool freezeCreationActions) const {
    this->newFileAction->setDisabled(freeze && freezeCreationActions);
    this->openFileAction->setDisabled(freeze && freezeCreationActions);
    this->saveFileAction->setDisabled(freeze || !this->modified);
    this->saveFileAsAction->setDisabled(freeze);
    this->closeFileAction->setDisabled(freeze);
}

bool Window::loadFile(const QString& path) {
    QString fixedPath = QDir(path).absolutePath();
    fixedPath.replace('\\', '/');

    this->clearContents();
    this->freezeActions(true);

    // todo: load file
    return true;
}
