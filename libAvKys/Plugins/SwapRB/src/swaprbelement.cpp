/* Webcamoid, webcam capture application.
 * Copyright (C) 2020  Gonzalo Exequiel Pedone
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

#include <akfrac.h>
#include <akpacket.h>
#include <akvideocaps.h>
#include <akvideoconverter.h>
#include <akvideopacket.h>

#include "swaprbelement.h"

class SwapRBElementPrivate
{
    public:
        AkVideoConverter m_videoConverter {{AkVideoCaps::Format_argb, 0, 0, {}}};
};

SwapRBElement::SwapRBElement(): AkElement()
{
}

SwapRBElement::~SwapRBElement()
{
}

AkPacket SwapRBElement::iVideoStream(const AkVideoPacket &packet)
{
    auto src = this->d->m_videoConverter.convertToImage(packet);

    if (src.isNull())
        return {};

    auto oFrame = src.rgbSwapped();
    auto oPacket = this->d->m_videoConverter.convert(oFrame, packet);

    if (oPacket)
        emit this->oStream(oPacket);

    return oPacket;
}

#include "moc_swaprbelement.cpp"
