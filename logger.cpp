#include "logger.h"

Logger::Logger(QObject *parent, QString fileName, QPlainTextEdit *editor) : QObject(parent)
{
    this->editor = editor;
    this->showDateTime = true;
    if(!fileName.isEmpty()) {
        file = new QFile;
        file->setFileName(fileName);
        if(file->exists()) file->remove();
        file->setFileName(fileName);
        file->open(QIODevice::Append | QIODevice::Text);
    }

}

void Logger::write(const QString &string) {
    QString text = string;
    if(showDateTime) {
        text = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss ") + text;
    }
    QTextStream out(this->file);
    out.setCodec("UTF-8");
    if(file != 0) {
        out << text << '\n';
    }
    if(editor != 0)
        editor->appendPlainText(text);
}

void Logger::setShowDateTime(bool value) {
    showDateTime = value;
}

Logger::~Logger() {
    if(file != 0) {
        file->close();
    }
}
