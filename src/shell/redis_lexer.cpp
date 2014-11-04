#include "shell/redis_lexer.h"

#include "core/redis/redis_driver.h"

namespace fastoredis
{
    RedisApi::RedisApi(QsciLexer *lexer)
        : QsciAbstractAPIs(lexer)
    {
    }

    void RedisApi::updateAutoCompletionList(const QStringList &context, QStringList &list)
    {
        QStringList allCommands = RedisDriver::allCommands();
        for(QStringList::const_iterator it = context.begin(); it != context.end(); ++it){
            QString val = *it;
            for(QStringList::const_iterator jt = allCommands.begin(); jt != allCommands.end(); ++jt){
                QString jval = *jt;
                if(jval.startsWith(val, Qt::CaseInsensitive)){
                    list.append(jval);
                }
            }
        }
    }

    QStringList RedisApi::callTips(const QStringList &context, int commas, QsciScintilla::CallTipsStyle style, QList<int> &shifts)
    {
        return QStringList();
    }

    RedisLexer::RedisLexer(QObject *parent)
        : QsciLexerCustom(parent)
    {
        setAPIs(new RedisApi(this));
    }

    const char *RedisLexer::language() const
    {
        return "Redis";
    }

    QString RedisLexer::description(int style) const
    {
        switch (style)
        {
        case Default:
             return tr("Default");

        case Command:
            return tr("Command");

        case Types:
            return tr("Types");
        }

        return QString(style);
    }

    QColor RedisLexer::defaultColor(int style) const
    {
        switch(style) {
            case Default:
                return Qt::black;
            case Command:
                return Qt::red;
            case Types:
                return Qt::blue;
        }
        return Qt::black;
    }

    void RedisLexer::styleText(int start, int end)
    {
        if(!editor()){
            return;
        }

        char *data = new char[end - start + 1];
        editor()->SendScintilla(QsciScintilla::SCI_GETTEXTRANGE, start, end, data);
        QString source(data);
        delete [] data;

        if(source.isEmpty()){
            return;
        }

        paintCommands(source, start);
        paintTypes(source, start);
    }

    void RedisLexer::paintCommands(const QString &source, int start)
    {
        static const QStringList &commands = RedisDriver::commandsKeywords();
        for(QStringList::const_iterator it = commands.begin(); it != commands.end(); ++it){
            QString word = *it;
            int index = 0;
            int begin = 0;
            while( (begin = source.indexOf(word, index, Qt::CaseInsensitive)) != -1){
                index = begin+1;

                startStyling(start + begin);
                setStyling(word.length(), Command);
                startStyling(start + begin);
            }
        }
    }

    void RedisLexer::paintTypes(const QString &source, int start)
    {
        static const QStringList &commands = RedisDriver::typesKeywords();
        for(QStringList::const_iterator it = commands.begin(); it != commands.end(); ++it){
            QString word = *it;
            int index = 0;
            int begin = 0;
            while( (begin = source.indexOf(word, index, Qt::CaseInsensitive)) != -1){
                index = begin+1;

                startStyling(start + begin);
                setStyling(word.length(), Types);
                startStyling(start + begin);
            }
        }
    }
}
