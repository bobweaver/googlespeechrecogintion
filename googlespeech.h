#ifndef GOOGLESPEECH_H
#define GOOGLESPEECH_H

#include <QQuickItem>

class GoogleSpeech : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(GoogleSpeech)

public:
    GoogleSpeech(QQuickItem *parent = 0);
    ~GoogleSpeech();
};

#endif // GOOGLESPEECH_H

