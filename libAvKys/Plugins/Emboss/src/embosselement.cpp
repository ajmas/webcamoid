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

#include <QImage>
#include <QQmlContext>
#include <QtMath>
#include <akfrac.h>
#include <akpacket.h>
#include <akvideocaps.h>
#include <akvideoconverter.h>
#include <akvideopacket.h>

#include "embosselement.h"

class EmbossElementPrivate
{
    public:
        qreal m_factor {1.0};
        qreal m_bias {128.0};
        AkVideoConverter m_videoConverter {{AkVideoCaps::Format_gray8, 0, 0, {}}};
};

EmbossElement::EmbossElement(): AkElement()
{
    this->d = new EmbossElementPrivate;
}

EmbossElement::~EmbossElement()
{
    delete this->d;
}

qreal EmbossElement::factor() const
{
    return this->d->m_factor;
}

qreal EmbossElement::bias() const
{
    return this->d->m_bias;
}

QString EmbossElement::controlInterfaceProvide(const QString &controlId) const
{
    Q_UNUSED(controlId)

    return QString("qrc:/Emboss/share/qml/main.qml");
}

void EmbossElement::controlInterfaceConfigure(QQmlContext *context,
                                              const QString &controlId) const
{
    Q_UNUSED(controlId)

    context->setContextProperty("Emboss", const_cast<QObject *>(qobject_cast<const QObject *>(this)));
    context->setContextProperty("controlId", this->objectName());
}

AkPacket EmbossElement::iVideoStream(const AkVideoPacket &packet)
{
    auto src = this->d->m_videoConverter.convertToImage(packet);

    if (src.isNull())
        return AkPacket();

    QImage oFrame(src.size(), src.format());

    for (int y = 0; y < src.height(); y++) {
        int y_m1 = y - 1;
        int y_p1 = y + 1;

        if (y_m1 < 0)
            y_m1 = 0;

        if (y_p1 >= src.height())
            y_p1 = src.height() - 1;

        auto srcLine_m1 = src.constScanLine(y_m1);
        auto srcLine = src.constScanLine(y);
        auto srcLine_p1 = src.constScanLine(y_p1);
        auto dstLine = oFrame.scanLine(y);

        for (int x = 0; x < src.width(); x++) {
            int x_m1 = x - 1;
            int x_p1 = x + 1;

            if (x_m1 < 0)
                x_m1 = 0;

            if (x_p1 >= src.width())
                x_p1 = src.width() - 1;

            int gray = srcLine_m1[x_m1] * 2
                     + srcLine_m1[x]
                     + srcLine[x_m1]
                     - srcLine[x_p1]
                     - srcLine_p1[x]
                     - srcLine_p1[x_p1] * 2;

            gray = qRound(this->d->m_factor * gray + this->d->m_bias);
            dstLine[x] = quint8(qBound(0, gray, 255));
        }
    }

    auto oPacket = this->d->m_videoConverter.convert(oFrame, packet);

    if (oPacket)
        emit this->oStream(oPacket);

    return oPacket;
}

void EmbossElement::setFactor(qreal factor)
{
    if (qFuzzyCompare(this->d->m_factor, factor))
        return;

    this->d->m_factor = factor;
    emit this->factorChanged(factor);
}

void EmbossElement::setBias(qreal bias)
{
    if (qFuzzyCompare(this->d->m_bias, bias))
        return;

    this->d->m_bias = bias;
    emit this->biasChanged(bias);
}

void EmbossElement::resetFactor()
{
    this->setFactor(1);
}

void EmbossElement::resetBias()
{
    this->setBias(128);
}

#include "moc_embosselement.cpp"
