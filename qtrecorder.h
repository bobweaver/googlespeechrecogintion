#ifndef LIBRECORDER_H
#define LIBRECORDER_H
#include <QtQml/qqmlparserstatus.h>
#include <QtQml/qqml.h>
#include <QAudioRecorder>
#include <QMediaRecorder>
#include <QMultimedia>
#include <QUrl>

class Recorder : public QObject
{
    Q_OBJECT
    Q_PROPERTY  (QString    path            READ path            WRITE setPath       NOTIFY pathChanged)
    Q_PROPERTY  (QString    codec           READ codec           WRITE setCodec      NOTIFY codecChanged)
    Q_PROPERTY  (int        quality         READ quality         WRITE setQuality    NOTIFY qualityChanged)
    Q_PROPERTY  (qreal      volume          READ volume          WRITE setVolume     NOTIFY volumeChanged)
    Q_PROPERTY  (qint64     duration        READ duration                            NOTIFY durationChanged)
    Q_PROPERTY  (Error      error           READ error                               NOTIFY errorChanged)
    Q_PROPERTY  (QString    errorString     READ errorString                         NOTIFY errorChanged)
    Q_PROPERTY  (State      state           READ state                               NOTIFY stateChanged)
    Q_ENUMS(Error)
    Q_ENUMS(State)

public:
    enum Error
    {
         NoError = QMediaRecorder::NoError,
         ResourceError = QMediaRecorder::ResourceError,
         FormatError = QMediaRecorder::FormatError,
         OutOfSpaceError = QMediaRecorder::OutOfSpaceError
    };

    enum State
    {
        StoppedState = QAudioRecorder::StoppedState,
        RecordingState = QAudioRecorder::RecordingState,
        PausedState = QAudioRecorder::PausedState
    };

    Recorder(QObject *parent = 0);
    ~Recorder();

    QString path() const;
    void setPath(const QString &path);

    QString codec() const;
    void setCodec(const QString &codec);

    int quality() const;
    void setQuality(const int &quality);

    qreal volume() const;
    void setVolume(const qreal &volume);

    qint64 duration() const;

    Error error() const;
    QString errorString() const;

    State state() const;

    Q_INVOKABLE QStringList getSupportedCodecs();
    Q_INVOKABLE QString getFilePath();

public Q_SLOTS:
    void start();
    void stop();
    void pause();
    void resume();

Q_SIGNALS:
    void pathChanged();

    void codecChanged();
    void qualityChanged();
    void volumeChanged();

    void durationChanged();

    void stateChanged();
    void recording();
    void stopped();
    void paused();
    void resumed();

    void errorChanged();

private Q_SLOTS:
    void _q_stateChanged();
    void _q_error();
    void _q_durationChanged();

private:
    QAudioRecorder *audioRecorder;
    QString cPath;

    QString m_path;
    QString m_codec;
    int m_quality;
    qreal m_volume;

    qint64 m_duration;

    QMediaRecorder::State m_state;

    QMediaRecorder::Error m_error;
    QString m_errorString;

    QString getContainerFromCodec(QString codec);
    QString getExtensionFromCodec(QString codec);

};
#endif // LIBRECORDER_H








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

//#ifndef  QTRECORDER_H
//#define QTRECORDER_H
//#include <QObject>
////#include <QtQml/qqmlparserstatus.h>
//#include <QUrl>
//#include <QAbstractListModel>
//#include <QtQml/qqml.h>
//#include <qmediaplayer.h>
//#include <QAudioEncoderSettings>
//#include <QAudioProbe>
//#include <QAudioRecorder>
//#include <QDesktopServices>
//#include <QDebug>
//#include <QtMultimedia>
//#include <QQmlInfo>

//QT_BEGIN_NAMESPACE

//class QtRecorder : public QObject //, public QQmlParserStatus
//{
//    Q_OBJECT
//    Q_PROPERTY( QString         codec                 READ codec                 WRITE setRecoderCodec     NOTIFY codecChanged            )
//    Q_PROPERTY( int         sampleRate              READ sampleRate                 WRITE setSampleRate            NOTIFY sampleRateChanged    )
//    Q_PROPERTY( int         channelCount                 READ channelCount                 WRITE setChannelCount     NOTIFY channelCountChanged            )
//    Q_PROPERTY( int         bitRate              READ bitRate                 WRITE setBitRate           NOTIFY bitRateChanged      )
//    Q_PROPERTY(QString   outputLocation READ outputLocation WRITE setOutputLocation NOTIFY outputLocationChanged )
////    Q_PROPERTY(int duration READ duration NOTIFY durationChanged)
////    Q_PROPERTY(qreal volume READ volume WRITE setVolume NOTIFY volumeChanged)
//    Q_PROPERTY(bool muted READ isMuted WRITE setMuted NOTIFY mutedChanged)
//    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
//    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
//    Q_PROPERTY(Error error READ error NOTIFY errorChanged)
//    Q_PROPERTY(RecordingState recordingState READ recordingState NOTIFY recordingStateChanged)
//    Q_PROPERTY( QString inputCard READ inputCard WRITE setInputCard )
//    Q_ENUMS(Status)
//    Q_ENUMS(Error)
//    Q_ENUMS(RecordingState)
//    Q_INTERFACES(QQmlParserStatus)

//public:
//    Q_INVOKABLE void getInputs() ;
//    Q_INVOKABLE void getSupportedAudioCodecs();
//    enum Status
//    {
//        UnavailbeStatus = QAudioRecorder::UnavailableStatus,
//        Unloaded       = QAudioRecorder::UnloadedStatus,
//        Loading       = QAudioRecorder::LoadingStatus,
//        Loaded        = QAudioRecorder::LoadedStatus,
//        Starting       = QAudioRecorder::StartingStatus,
//        Recording  = QAudioRecorder::RecordingStatus,
//        Paused      = QAudioRecorder::PausedStatus,
//        Encoding    = QAudioRecorder::FinalizingStatus
//    };

//    enum RecordingState
//    {
//        Stopped        = QAudioRecorder::StoppedState,
//        Record  = QAudioRecorder::RecordingState,
//        Pause    = QAudioRecorder::PausedState
//    };

//    enum Error
//    {
//        NoError = QAudioRecorder::NoError,
//        ResourceError = QAudioRecorder::ResourceError,
//        FormatError = QAudioRecorder::FormatError,
//        OutOfSpace = QAudioRecorder::OutOfSpaceError
//    };
//    enum Quality
//    {
//        VeryLow = QMultimedia::VeryLowQuality,
//        Low = QMultimedia::LowQuality,
//        Normal = QMultimedia::NormalQuality,
//        High = QMultimedia::HighQuality,
//        VeryHigh = QMultimedia::VeryHighQuality
//    };

//    QtRecorder(QObject *parent = 0);
//    ~QtRecorder();

//    RecordingState recordingState() const;

//    QString inputCard() const;
//    void setInputCard(const QString &inputCard);

//    QString codec() const;
//    void setRecoderCodec(const QString &codec);

//    int sampleRate() ;
//    void setSampleRate(int &sampleRate);

//    int channelCount();
//    void setChannelCount(int &channelCount);

//    int bitRate() ;
//    void setBitRate( int &bitRate);

//    QString outputLocation() const;
//    void setOutputLocation(const QString &outputLocation);

////    void classBegin();
//    void componentComplete();

////    int duration ();
////    qreal volume() const;
//    void setVolume(qreal &volume);
//    bool isMuted() const;
//    void setMuted(bool &muted);
//    Status status() const;
//    Error error() const;
//    QMultimedia::EncodingQuality quality();
//    QMultimedia::EncodingQuality setQuality(QMultimedia::EncodingQuality &quality);
//    QString errorString() const;

//public Q_SLOTS:
//    void run();
//    void pause();
//    void stop();
//Q_SIGNALS:
//    void paused();
//    void stopped();
//    void running();
//    void outputLocationChanged();
//    void codecChanged();
//    void sampleRateChanged();
//    void channelCountChanged();
//    void bitRateChanged();
//    void statusChanged();
//    void durationChanged();
//    void inputChanged();
////    void volumeChanged();
//    void mutedChanged();
//    void recordingStateChanged();
//    void qualityChanged();
//    void errorChanged();
//    void errorStringChanged();
//    void error(QtRecorder::Error error, const QString &errorString);

//private Q_SLOTS:
//    void _q_error(QMediaRecorder::Error);
//    void _q_statusChanged();

//private:
//    void setAudioSettings();
//    QAudioRecorder *m_recorder;
//    QStringList  m_model;

//    QString m_Codec;
//    int m_SampleRate;
//    int m_ChannelCount;
//    int  m_BitRate;
//    QString m_outputLocation;
////    int m_duration;
////    qreal m_volume;
////    QString m_input;
//    bool m_muted;
//    bool m_complete;
//    QString m_errorString;
//    QAudioRecorder::State m_recordingState;
//    QAudioRecorder::Status m_status;
//    QAudioRecorder::Error m_error;
//    QMultimedia::EncodingQuality m_quality;
//    QString m_Input;

//};
//QT_END_NAMESPACE
////QML_DECLARE_TYPE(QT_PREPEND_NAMESPACE(QtRecorder))
//#endif// QTRECORDER_H
