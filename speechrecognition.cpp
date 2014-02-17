#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslSocket>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "speechrecognition.h"
#include <QFile>
#include "speechrecognition.h"
#include <QDebug>
const char* SpeechRecognition::kContentType = "audio/x-flac; rate=8000";
const char* SpeechRecognition::kUrl = "http://www.google.com/speech-api/v1/recognize?xjerr=1&client=directions&lang=en";

SpeechRecognition::SpeechRecognition(QObject* parent)
  : QObject(parent)
{
    network_ = new QNetworkAccessManager(this);
    connect(network_, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

void SpeechRecognition::start(){
    const QUrl url(kUrl);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, kContentType);
    req.setAttribute(QNetworkRequest::DoNotBufferUploadDataAttribute, false);
    req.setAttribute(QNetworkRequest::CacheLoadControlAttribute,
                     QNetworkRequest::AlwaysNetwork);
    QFile *compressedFile = new QFile("/home/joseph/.qt-googlevoice/output.flac");
    compressedFile->open(QIODevice::ReadOnly);
    reply_ = network_->post(req, compressedFile);
}

void SpeechRecognition::replyFinished(QNetworkReply* reply) {

  Result result = Result_ErrorNetwork;
  Hypotheses hypotheses;

  if (reply->error() != QNetworkReply::NoError) {
    qDebug() << "ERROR \n" << reply->errorString();
  } else {
      qDebug() << "Running ParserResponse for \n" << reply << result;
      ParseResponse(reply, &result, &hypotheses);
  }
  emit Finished(result, hypotheses);
  reply_->deleteLater();
  reply_ = NULL;
}

void SpeechRecognition::ParseResponse(QIODevice* reply, Result* result,
                                      Hypotheses* hypotheses)
{
 QString getReplay ;
 getReplay = reply->readAll();
 qDebug() << "The Replay " << getReplay;
 QJsonDocument jsonDoc = QJsonDocument::fromJson(getReplay.toUtf8());
  QVariantMap data = jsonDoc.toVariant().toMap();

  const int status = data.value("status", Result_ErrorNetwork).toInt();
  *result = static_cast<Result>(status);

  if (status != Result_Success)
    return;

  QVariantList list = data.value("hypotheses", QVariantList()).toList();
  foreach (const QVariant& variant, list) {
    QVariantMap map = variant.toMap();

    if (!map.contains("utterance") || !map.contains("confidence"))
      continue;

    Hypothesis hypothesis;
    hypothesis.utterance = map.value("utterance", QString()).toString();
    hypothesis.confidence = map.value("confidence", 0.0).toReal();
    *hypotheses << hypothesis;
    qDebug() << "confidence = " << hypothesis.confidence << "\n Your Results = "<< hypothesis.utterance;
    setResults(hypothesis.utterance);
//  QFile f;
////  f.setFileName("fo");
//  if (f.exists("/home/joseph/.qt-googlevoice/output.flac"))
//      f.remove("/home/joseph/.qt-googlevoice/output.flac");
//  }
}
}

  void SpeechRecognition::setResults(const QString &results)
{
    if(m_results == results)
    return;
        m_results = results;
    emit resultsChanged();
}

QString SpeechRecognition::results()const
{
    return m_results;
}
