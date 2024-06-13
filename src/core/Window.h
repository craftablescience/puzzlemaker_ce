#pragma once

#include <functional>
#include <vector>

#include <QMainWindow>

class QAction;
class QCloseEvent;

class Window : public QMainWindow {
    Q_OBJECT;

public:
    explicit Window(QWidget* parent = nullptr);

    void newFile(bool fromDirectory, const QString& startPath = QString());

    void openFile(const QString& startPath = QString(), const QString& filePath = QString());

    bool saveFile();

    bool saveFileAs();

    void closeFile();

    void about();

    void aboutQt();

    void markModified(bool modified_);

    [[nodiscard]] bool promptUserToKeepModifications();

    void clearContents();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    QAction* newFileAction;
    QAction* openFileAction;
    QAction* saveFileAction;
    QAction* saveFileAsAction;
    QAction* closeFileAction;

    bool modified;

    void freezeActions(bool freeze, bool freezeCreationActions = true) const;

    bool loadFile(const QString& path);
};
