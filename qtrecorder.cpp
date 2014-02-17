/*
  Copyright (C) 2013 Stefano Verzegnassi

    This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; version 3.

    This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "qtrecorder.h"
#include <QFile>
Recorder::Recorder(QObject *parent) :
    QObject(parent),
    m_codec("audio/FLAC"),
    m_quality(0),
    m_volume(100),
    m_state(QMediaRecorder::StoppedState),
    m_error(QMediaRecorder::ResourceError)
{
    audioRecorder = new QAudioRecorder;

    connect(audioRecorder, SIGNAL(durationChanged(qint64)), this,
            SLOT(_q_durationChanged()));
    connect(audioRecorder, SIGNAL(stateChanged(QMediaRecorder::State)), this,
            SLOT(_q_stateChanged()));
    connect(audioRecorder, SIGNAL(error(QMediaRecorder::Error)), this,
            SLOT(_q_error()));
}

void Recorder::_q_error()
{
   m_error = audioRecorder->error();
   m_errorString = audioRecorder->errorString();

   emit errorChanged();
}

Recorder::Error Recorder::error() const
{
    return Error(m_error);
}

QString Recorder::errorString() const
{
    return m_errorString;
}

void Recorder::_q_stateChanged()
{
    const QMediaRecorder::State oldState = m_state;
    const QMediaRecorder::State state = audioRecorder->state();

    m_state = audioRecorder->state();

    if (state != oldState) {
        switch (state) {
            case QMediaRecorder::StoppedState:
                emit stopped();
                break;
            case QMediaRecorder::RecordingState:
                if (oldState == QMediaRecorder::PausedState) {
                    emit resumed();
                    break;
                } else {
                    emit recording();
                    break;
                }
            case QMediaRecorder::PausedState:
                emit paused();
                break;
            }

        emit stateChanged();
    }
}

Recorder::State Recorder::state() const
{
    return State(m_state);
}

QString Recorder::path() const
{
    return m_path;
}

void Recorder::setPath(const QString &path)
{
    if (m_path == path)
        return;
    m_path = path;
    emit pathChanged();
}

QString Recorder::codec() const
{
    return m_codec;
}

void Recorder::setCodec(const QString &codec)
{
    if (m_codec == codec)
        return;

    m_codec = codec;
    emit codecChanged();
}

int Recorder::quality() const
{
    return m_quality;
}

void Recorder::setQuality(const int &quality)
{
    if (m_quality == quality)
        return;

    m_quality = quality;
    emit qualityChanged();
}

qreal Recorder::volume() const
{
    return m_volume;
}

void Recorder::setVolume(const qreal &volume)
{
    if (m_volume == volume)
        return;

    m_volume = volume;
    emit volumeChanged();
}

qint64 Recorder::duration() const
{
    return m_duration;
}

void Recorder::_q_durationChanged()
{
    m_duration = audioRecorder->duration();
    emit durationChanged();
}

Recorder::~Recorder()
{
    delete audioRecorder;
}

void Recorder::start() //TODO: reduce noise settings
{
    if (audioRecorder->state() == QMediaRecorder::StoppedState) {
        QAudioEncoderSettings audioSettings;

        // Set codec
        audioSettings.setCodec(m_codec);

        // Set quality: 0=VeryLow, 1=Low, 2=Normal, 3=High, 4=VeryHigh
        audioSettings.setEncodingMode(QMultimedia::ConstantQualityEncoding);
        audioSettings.setQuality(QMultimedia::EncodingQuality(m_quality));

        if (audioSettings.codec() == "audio/PCM" || "audio/FLAC") {
            audioSettings.setSampleRate(-1);

            if (m_quality == 0)
                {audioSettings.setSampleRate(8000);}

            if (m_quality == 1)
                {audioSettings.setSampleRate(16000);}

            if (m_quality == 2)
                {audioSettings.setSampleRate(22050);}

            if (m_quality == 3)
                {audioSettings.setSampleRate(44100);}

            if (m_quality == 4)
                {audioSettings.setSampleRate(88200);}
            }

        audioRecorder->setEncodingSettings(audioSettings);

        audioRecorder->setContainerFormat(getContainerFromCodec(m_codec));

        // Set volume
        audioRecorder->setVolume(m_volume);

        // Set output location
        cPath = "/home/joseph/.qt-googlevoice/output.flac";//m_path + getExtensionFromCodec(m_codec);
//        QFile f;
//        f = "/home/joseph/.qt-googlevoice/output.flac";
//        if (f.is){
//            f.remove();
//        }


        audioRecorder->setOutputLocation(QUrl(cPath));

        audioRecorder->record();
    }
}

QString Recorder::getFilePath()
{
    return cPath;
}

void Recorder::stop()
{
    if (audioRecorder->state() == QMediaRecorder::RecordingState ||
            audioRecorder->state() == QMediaRecorder::PausedState) {

        audioRecorder->stop();
    }
}

void  Recorder::pause()
{
    if (audioRecorder->state() == QMediaRecorder::RecordingState) {
        audioRecorder->pause();
    }
}

void Recorder::resume()
{
    if (audioRecorder->state() == QMediaRecorder::PausedState) {
        audioRecorder->record();
    }
}

// It parses codec name to get the corrisponding container
QString Recorder::getContainerFromCodec(QString codec)
{
    QString container = "";

    if (codec == "audio/vorbis")
        { container = "ogg";}

    if (codec == "audio/PCM")
        { container = "wav";}

    if (codec == "audio/FLAC")
        { container = "raw";}

    if (codec == "audio/AMR")
        { container = "amr";}

    if (codec == "audio/mpeg")
        { container = "raw";}

    return container;
}

// It parses codec name to get the corrisponding extension
QString Recorder::getExtensionFromCodec(QString codec)
{
    QString extension = "";

    if (codec == "audio/vorbis")
        { extension = ".ogg";}

    if (codec == "audio/PCM")
        { extension = ".wav";}

    if (codec == "audio/FLAC")
        { extension = ".flac";}

    if (codec == "audio/AMR")
        { extension = ".amr";}

    if (codec == "audio/mpeg")
        { extension = ".mp3";}

    return extension;
}

QStringList Recorder::getSupportedCodecs()
{
    QStringList allSupportedCodecs = audioRecorder->supportedAudioCodecs();
    QStringList codecsList;

    foreach (QString codec, allSupportedCodecs) {
        if (codec.contains("audio/mpeg", Qt::CaseInsensitive) == true)
            {codecsList << codec;}
        if (codec.contains("audio/vorbis", Qt::CaseInsensitive) == true)
            {codecsList << codec;}
        if (codec.contains("audio/FLAC", Qt::CaseInsensitive) == true)
            {codecsList << codec;}
        if (codec.contains("audio/PCM", Qt::CaseInsensitive) == true)
            {codecsList << codec;}
        if (codec.contains("audio/AMR", Qt::CaseInsensitive) == true)
            {codecsList << codec;}
    }

    return codecsList;
}


///*
// * Copyright (C) 2012-2013 Joseph Mills.
// *
// * This program is free software; you can redistribute it and/or modify
// * it under the terms of the GNU General Public License as published by
// * the Free Software Foundation; version 3.
// *
// * This program is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// * GNU General Public License for more details.
// *
// * You should have received a copy of the GNU General Public License
// * along with this program.  If not, see <http://www.gnu.org/licenses/>.
// */

//#include "qtrecorder.h"

//void QtRecorder::_q_error(QAudioRecorder::Error errorCode)
//{
//    m_error = errorCode;
//    m_errorString = m_recorder->errorString();
//    emit error(Error(errorCode), m_errorString);
//    emit errorChanged();
//}

//QtRecorder::QtRecorder(QObject *parent)
//    : QObject(parent)
//    ,m_Codec("audio/FLAC")
//    ,m_SampleRate(16000)
//    ,m_ChannelCount(0)
//    ,m_BitRate(-1)
//    ,m_outputLocation("")
//    , m_muted(false)
//    , m_complete(false)
//    , m_recordingState(QMediaRecorder::StoppedState)
//    , m_Input("")
//{
//    m_recorder = new QAudioRecorder(this);
//    connect(m_recorder, SIGNAL(stateChanged(QMediaRecorder::State)),
//            this, SLOT(_q_statusChanged()));
//    connect(m_recorder, SIGNAL(statusChanged(QMediaRecorder::Status)),
//            this, SLOT(_q_statusChanged()));
//    connect(m_recorder, SIGNAL(error(QMediaRecorder::Error)),
//            this, SLOT(_q_error(QMediaRecorder::Error)));

//}

//QtRecorder::~QtRecorder()
//{
//    delete m_recorder;
//}

//QString QtRecorder::codec() const
//{
//    return m_Codec;
//}
//void QtRecorder::setRecoderCodec(const QString &codec)
//{
//    m_Codec = codec;
////    emit codecChanged();
//}


//QString QtRecorder::inputCard() const
//{
//    return m_Input;
//}
//void QtRecorder::setInputCard(const QString &inputCard)
//{
//    m_Input = inputCard;
//}


//int QtRecorder::sampleRate()
//{
//    return m_SampleRate;
//}

//void QtRecorder::setSampleRate( int &sampleRate)
//{
//    if (m_SampleRate == sampleRate)
//        return;
//    m_SampleRate = sampleRate;
//    emit sampleRateChanged();
//}


//int QtRecorder::channelCount()
//{
//    return m_ChannelCount;
//}
//void QtRecorder::setChannelCount( int &channelCount)
//{
//    if (m_ChannelCount == channelCount)
//        return;
//    m_ChannelCount = channelCount;
//    emit channelCountChanged();

//}

//int QtRecorder::bitRate()
//{
//    return m_BitRate;
//}
//void QtRecorder::setBitRate(int &bitRate)
//{
//    if (m_BitRate == bitRate)
//        return;
//    m_BitRate = bitRate;
//    emit bitRateChanged();
//}

//QString QtRecorder::outputLocation() const
//{
//    return m_outputLocation;
//}

//void QtRecorder::setOutputLocation(const QString &outputLocation)
//{
//   if (m_outputLocation == outputLocation)
//        return;
//       m_outputLocation = outputLocation;
//    emit outputLocationChanged();
//}

////Make the Model for the cards
//void QtRecorder::getInputs()
//{
//    QStringList result;
//    result = m_recorder->audioInputs();
//        foreach (const QString &str, result) {
//        qDebug() << str;
//        }
//}

//void QtRecorder::getSupportedAudioCodecs()
//{
//    QStringList result;
//    result =m_recorder->supportedAudioCodecs();
//        foreach (const QString &stp,  result)
//        {
//        qDebug() << stp;
//        }
//}

////  Quality

//QMultimedia::EncodingQuality QtRecorder::quality()
//{
//    return QMultimedia::EncodingQuality(m_quality);
//}

//QMultimedia::EncodingQuality QtRecorder::setQuality(QMultimedia::EncodingQuality &quality)
//{
//    m_quality = quality;
//    emit qualityChanged();
//}


//////////////////////ENUM


///*!
//    \qmlproperty enumeration QtMultimedia::AudioRecorder::status

//    This property holds the status of media loading. It can be one of:

//    \list
//    \li UnavailableStatus		The recorder is not available or not supported by connected media object.
//    \li UnloadedStatus		The recorder is avilable but not loaded.
//    \li LoadingStatus		The recorder is initializing.
//    \li LoadedStatus		The recorder is initialized and ready to record media.
//    \li StartingStatus		Recording is requested but not active yet.
//    \li RecordingStatus		Recording is active.
//    \li PausedStatus		Recording is paused.
//    \liFinalizingStatus		Recording is stopped with media being finalized.
//    \endlist
//*/


////status
//QtRecorder::Status QtRecorder::status() const
//{
//    return Status(m_status);
//}

//void QtRecorder::_q_statusChanged()
//{
//    const QMediaRecorder::Status oldStatus = m_status;
//    const QMediaRecorder::State lastState = m_recordingState;
//    const QMediaRecorder::State state = m_recorder->state();
//    m_recordingState = state;
//    m_status = m_recorder->status();
//    if (m_status != oldStatus)
//        emit statusChanged();
//    if (lastState != state) {
//        switch (state) {
//        case QAudioRecorder::StoppedState:
//            emit stopped();
//            break;
//        case QAudioRecorder::PausedState:
//            emit paused();
//            break;
//        case QAudioRecorder::RecordingState:
//            emit run();
//            break;
//        }
//        emit statusChanged();
//    }
//}

//// state

//QtRecorder::RecordingState QtRecorder::recordingState() const
//{
//    return RecordingState(m_recordingState);
//}


////errors
//QtRecorder::Error QtRecorder::error() const
//{
//    return Error(m_error);
//}

//QString QtRecorder::errorString() const
//{
//    return m_errorString;
//}

////qreal QtRecorder::volume() const
////{
////    return !m_complete ? m_volume : qreal(m_recorder->volume()) / 100;
////}

////void QtRecorder::setVolume(qreal &volume)
////{
////    if (volume < 0 || volume > 1) {
////        qmlInfo(this) << tr("volume should be between 0.0 and 1.0");
////        return;
////    }
////    if (m_volume == volume)
////        return;
////    m_volume = volume;

////    if (m_complete)
////        m_recorder->setVolume(qRound(volume * 100));
////}

//void QtRecorder::setAudioSettings()
//{
//    m_recorder->setAudioInput(m_Input);
//    //    set up the settings
//    QAudioEncoderSettings settings;
//    settings.setCodec(m_Codec);
//    settings.setSampleRate(-1);
//    settings.setChannelCount(m_ChannelCount);
//    settings.setBitRate(m_BitRate);
//    settings.setQuality(QMultimedia::EncodingQuality(m_quality));

//    m_recorder->setEncodingSettings(settings);
//    QString fileName = m_outputLocation;
//    m_recorder->setOutputLocation(QUrl(fileName));

//}


//bool QtRecorder::isMuted() const
//{
//        return   m_muted;
//}

//void QtRecorder::setMuted(bool &muted)
//{
//    if (m_muted == muted)
//        return;
//    m_muted = muted;
//    emit mutedChanged();

//}

//void QtRecorder::componentComplete()
//{
////    if (!qFuzzyCompare(m_volume, qreal(1.0)))
////        m_recorder->setVolume(m_volume * 100);
//    if (m_muted)
//        m_recorder->setMuted(m_muted);
//        m_recorder->setOutputLocation(m_outputLocation);

////    }
//    m_complete = true;
//}

//void QtRecorder::run()
//{
//    setAudioSettings();
//    if (m_recorder->state() == QMediaRecorder::StoppedState || m_recorder->state() == QMediaRecorder::PausedState)
//    {
//    m_recorder->record();
//    }
//    else if (m_recorder->state() == QMediaRecorder::RecordingState)
//    {
//    m_recorder->stop();
//    }
//}

//void QtRecorder::stop()
//{
//    if (m_recorder->state() == QMediaRecorder::RecordingState ||
//    m_recorder->state() == QMediaRecorder::PausedState)
//    {
//    m_recorder->stop();
//    }
//}
//void QtRecorder::pause()
//{
//    if (m_recorder->state() == QMediaRecorder::RecordingState)
//    {
//    m_recorder->pause();
//    }
//}
