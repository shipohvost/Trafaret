#ifndef HEXLINEEDIT_H
#define HEXLINEEDIT_H

#include <QLineEdit>
class QMimeData;

class HexLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit HexLineEdit(QWidget *parent = nullptr);
    ~HexLineEdit();

    void setMaxBytes(int bytes);
    int getMaxBytes() const;
    QByteArray binaryData() const;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void insertFromMimeData(const QMimeData *source);

private slots:
    void formatHex(const QString &text);

private:
    QTimer *cursorTimer;
    bool cursorVisible;
    int maxBytes;

    void setupCursor();
    void drawCustomCursor();
    void updatePlaceholder();
    void restoreCursorPosition(int oldPos, const QString &oldText, const QString &newText);
};

#endif // HEXLINEEDIT_H
