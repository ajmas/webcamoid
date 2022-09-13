/* Webcamoid, webcam capture application.
 * Copyright (C) 2016  Gonzalo Exequiel Pedone
 *
 * Webcamoid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Webcamoid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Webcamoid. If not, see <http://www.gnu.org/licenses/>.
 *
 * Web-Site: http://webcamoid.github.io/
 */

#ifndef MEDIASOURCEFFMPEG_H
#define MEDIASOURCEFFMPEG_H

#include "mediasource.h"

class MediaSourceFFmpegPrivate;

class MediaSourceFFmpeg: public MediaSource
{
    Q_OBJECT

    public:
        MediaSourceFFmpeg(QObject *parent=nullptr);
        ~MediaSourceFFmpeg();

        Q_INVOKABLE QStringList medias() const;
        Q_INVOKABLE QString media() const;
        Q_INVOKABLE QList<int> streams() const;
        Q_INVOKABLE QList<int> listTracks(AkCaps::CapsType type);
        Q_INVOKABLE QString streamLanguage(int stream);
        Q_INVOKABLE bool loop() const;
        Q_INVOKABLE bool sync() const;
        Q_INVOKABLE int defaultStream(AkCaps::CapsType type);
        Q_INVOKABLE QString description(const QString &media) const;
        Q_INVOKABLE AkCaps caps(int stream);
        Q_INVOKABLE qint64 durationMSecs();
        Q_INVOKABLE qint64 currentTimeMSecs();
        Q_INVOKABLE qint64 maxPacketQueueSize() const;
        Q_INVOKABLE bool showLog() const;
        Q_INVOKABLE AkElement::ElementState state() const;

    private:
        MediaSourceFFmpegPrivate *d;

    public slots:
        void seek(qint64 mSecs, MediaSource::SeekPosition position);
        void setMedia(const QString &media);
        void setStreams(const QList<int> &streams);
        void setMaxPacketQueueSize(qint64 maxPacketQueueSize);
        void setShowLog(bool showLog);
        void setLoop(bool loop);
        void setSync(bool sync);
        void resetMedia();
        void resetStreams();
        void resetMaxPacketQueueSize();
        void resetShowLog();
        void resetLoop();
        void resetSync();
        bool setState(AkElement::ElementState state);

    private slots:
        void doLoop();
        void packetConsumed();
        void log();
        bool initContext();
};

#endif // MEDIASOURCEFFMPEG_H
