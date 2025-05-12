#include "hexlineedit.h"
#include <QFocusEvent>
#include <QKeyEvent>
#include <QMimeData>
#include <QPaintEvent>
#include <QPainter>
#include <QRegularExpression>
#include <QTimer>

HexLineEdit::HexLineEdit(QWidget *parent)
    : QLineEdit(parent)
    , cursorVisible(true)
    , maxBytes(256)
{
    setMaxBytes(maxBytes);
    connect(this, &QLineEdit::textChanged, this, &HexLineEdit::formatHex);
    setupCursor();
}

HexLineEdit::~HexLineEdit()
{
    cursorTimer->stop();
    delete cursorTimer;
}

void HexLineEdit::setMaxBytes(int bytes)
{
    maxBytes = qMax(1, bytes);
    setMaxLength(maxBytes * 2 + (maxBytes - 1));
    updatePlaceholder();
}

int HexLineEdit::getMaxBytes() const
{
    return maxBytes;
}

QByteArray HexLineEdit::binaryData() const
{
    return QByteArray::fromHex(text().remove(' ').toLatin1());
}

void HexLineEdit::keyPressEvent(QKeyEvent *event)
{
    if ((event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9)
        || (event->key() >= Qt::Key_A && event->key() <= Qt::Key_F) || event->key() == Qt::Key_Space
        || event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete
        || event->key() == Qt::Key_Home || event->key() == Qt::Key_End) {
        QLineEdit::keyPressEvent(event);
    } else if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right) {
        int currentPos = cursorPosition();
        int newPos = currentPos;

        if (event->key() == Qt::Key_Right) {
            // Движение вправо
            newPos++;
            // Если новая позиция - пробел, перескакиваем его
            if (newPos < text().length() && text().at(newPos) == ' ')
                newPos++;
        } else if (event->key() == Qt::Key_Left) {
            // Движение влево
            newPos--;
            // Если новая позиция - пробел, перескакиваем его
            if (newPos >= 0 && text().at(newPos) == ' ')
                newPos--;
        }

        // Обеспечиваем, чтобы позиция была в допустимых пределах
        newPos = qBound(0, newPos, text().length());

        // Устанавливаем новую позицию курсора
        setCursorPosition(newPos);
    } else {
        event->ignore();
    }
}

void HexLineEdit::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);
    drawCustomCursor();
}

void HexLineEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    cursorVisible = true;
    update();
    cursorTimer->start(500);
}

void HexLineEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    cursorTimer->stop();
    cursorVisible = false;
    update();
}

void HexLineEdit::insertFromMimeData(const QMimeData *source)
{
    if (!source->hasText())
        return;

    QString text = source->text().toUpper();
    QString clean = text.remove(QRegularExpression("[^0-9A-F]"));

    if (clean.length() > maxBytes * 2) {
        clean = clean.left(maxBytes * 2);
    }

    QString formatted;
    for (int i = 0; i < clean.length(); i += 2) {
        if (!formatted.isEmpty())
            formatted += ' ';
        formatted += clean.mid(i, 2);
    }

    setText(formatted);
}

void HexLineEdit::formatHex(const QString &text)
{
    disconnect(this, &QLineEdit::textChanged, this, &HexLineEdit::formatHex);

    int oldPos = cursorPosition();
    QString clean = text.toUpper().remove(QRegularExpression("[^0-9A-F]"));

    if (clean.length() > maxBytes * 2) {
        clean = clean.left(maxBytes * 2);
    }

    QString formatted;
    for (int i = 0; i < clean.length(); i += 2) {
        if (!formatted.isEmpty())
            formatted += ' ';
        formatted += clean.mid(i, 2);
    }

    setText(formatted);
    restoreCursorPosition(oldPos, text, formatted);

    connect(this, &QLineEdit::textChanged, this, &HexLineEdit::formatHex);
}

void HexLineEdit::setupCursor()
{
    cursorTimer = new QTimer(this);
    connect(cursorTimer, &QTimer::timeout, this, [this]() {
        cursorVisible = !cursorVisible;
        update();
    });
    cursorTimer->start(500);
}

void HexLineEdit::drawCustomCursor()
{
    if (!hasFocus())
        return;

    QPainter painter(this);
    QRect rect = cursorRect();

    if (cursorPosition() != text().length()) {
        if (cursorVisible) {
            painter.setPen(QPen(palette().color(QPalette::Text), 1));
            painter.setBrush(Qt::NoBrush);
            painter.drawRect(rect.adjusted(5, 0, 2, -3));
        }
    } else {
        if (cursorVisible) {
            painter.setPen(palette().color(QPalette::Text));
            painter.drawLine(rect.topLeft() + QPoint(5, 0), rect.bottomLeft() + QPoint(5, 0));
        }
    }
}

void HexLineEdit::updatePlaceholder()
{
    setPlaceholderText(QString("Введите HEX-данные (максимум %1 байт)").arg(maxBytes));
}

void HexLineEdit::restoreCursorPosition(int oldPos, const QString &oldText, const QString &newText)
{
    if (oldPos >= oldText.length()) {
        setCursorPosition(newText.length());
        return;
    }

    int hexCharsBefore = 0;
    for (int i = 0; i < oldPos; ++i) {
        if (oldText.at(i).isLetterOrNumber())
            hexCharsBefore++;
    }

    int newPos = 0;
    int hexCount = 0;
    for (; newPos < newText.length() && hexCount < hexCharsBefore; ++newPos) {
        if (newText.at(newPos).isLetterOrNumber())
            hexCount++;
    }

    setCursorPosition(newPos);
}

void HexLineEdit::mousePressEvent(QMouseEvent *event)
{
    QLineEdit::mousePressEvent(event);
    int pos = cursorPosition();
    if (pos < text().length() && text().at(pos) == ' ') {
        setCursorPosition(pos - 1);
    }
}
