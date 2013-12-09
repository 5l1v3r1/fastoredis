#include "gui/widgets/LogWidget.h"

#include <QHBoxLayout>
#include <QScrollBar>
#include <QMenu>
#include <QTime>
#include <QAction>
#include <QPlainTextEdit>
#include "common/macros.h"

namespace fastoredis
{
    LogWidget::LogWidget(QWidget* parent) 
        : BaseClass(parent), _logTextEdit(new QTextEdit(this))
    {
        _logTextEdit->setReadOnly(true);
        _logTextEdit->setContextMenuPolicy(Qt::CustomContextMenu);
        VERIFY(connect(_logTextEdit,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenu(const QPoint &))));
        QHBoxLayout *hlayout = new QHBoxLayout;
        hlayout->setContentsMargins(0,0,0,0);
        hlayout->addWidget(_logTextEdit);
        _clear = new QAction("Clear All", this);
        VERIFY(connect(_clear, SIGNAL(triggered()),_logTextEdit, SLOT(clear())));
        setLayout(hlayout);      
    }

    void LogWidget::showContextMenu(const QPoint &pt)
    {
        QMenu *menu = _logTextEdit->createStandardContextMenu();
        menu->addAction(_clear);
        _clear->setEnabled(!_logTextEdit->toPlainText().isEmpty());

        menu->exec(_logTextEdit->mapToGlobal(pt));
        delete menu;
    }

    void LogWidget::addMessage(const QString &message, common::logging::LEVEL_LOG level)
    {
        QTime time = QTime::currentTime();
        _logTextEdit->setTextColor(level == common::logging::CRITICAL ? QColor(Qt::red):QColor(Qt::black));
        _logTextEdit->append(time.toString("h:mm:ss AP: ") + message);
        QScrollBar *sb = _logTextEdit->verticalScrollBar();
        sb->setValue(sb->maximum());
    }
}
