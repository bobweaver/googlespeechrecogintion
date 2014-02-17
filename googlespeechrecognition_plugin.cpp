#include "googlespeechrecognition_plugin.h"
#include "googlespeech.h"

#include <qqml.h>

void GoogleSpeechRecognitionPlugin::registerTypes(const char *uri)
{
    // @uri GoogleSpeech
    qmlRegisterType<GoogleSpeech>(uri, 1, 0, "GoogleSpeech");
}


