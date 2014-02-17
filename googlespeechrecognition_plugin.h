#ifndef GOOGLESPEECHRECOGNITION_PLUGIN_H
#define GOOGLESPEECHRECOGNITION_PLUGIN_H

#include <QQmlExtensionPlugin>

class GoogleSpeechRecognitionPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif // GOOGLESPEECHRECOGNITION_PLUGIN_H

