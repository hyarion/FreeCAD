/**************************************************************************\
 * Copyright (c) Benjamin Nauck
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#ifdef _MSC_VER
#pragma warning(disable : 4267)
#endif

#include <QEvent>
#include <QKeyEvent>
#include <Inventor/events/SoTouchEvent.h>

#include "devices/TouchDevice.h"


namespace SIM { namespace Coin3D { namespace Quarter {

class TouchDeviceP {
public:
  explicit TouchDeviceP(TouchDevice * publ) {
    this->publ = publ;
    this->touch = new SoTouchEvent();
  }

  ~TouchDeviceP() {
    delete this->touch;
  }

  const SoTouchEvent * touchEvent(QTouchEvent * event, bool & isDone);

  SoTouchEvent * touch;

  TouchDevice * publ;
};
}}}

using namespace SIM::Coin3D::Quarter;

#define PRIVATE(obj) obj->pimpl
#define PUBLIC(obj) obj->publ

const SoTouchEvent *
TouchDeviceP::touchEvent(QTouchEvent * event, bool & isDone)
{
  PUBLIC(this)->setModifiers(this->touch, event);

  if (event != this->cachedEvent) {
    this->touchPoints = QList<QTouchEvent::TouchPoint>(event->points());
    if (this->touchPoints.empty()) {
      isDone = true;
      return nullptr;
    }
  }
  
  QTouchEvent::TouchPoint touchPoint(this->touchPoints.pop());
  isDone = this->touchPoints.empty();

  this->touch->setFingerId(event->id());

  QPointF & position(event->position());
  this->touch->setPosition(SbVec2f(position.x(), position.y()));

  QPointF & speed(event->velocity());
  this->touch->setSpeed(SbVec2f(speed.x(), speed.y()));

  return this->touch;
}

TouchDevice::TouchDevice(QuarterWidget* quarter) :
  InputDevice(quarter)
{
  PRIVATE(this) = new TouchDeviceP(this);
}

TouchDevice::~TouchDevice()
{
  delete PRIVATE(this);
}

/*! Translates from QKeyEvents to SoTouchDeviceEvents
 */
const SoEvent *
TouchDevice::translateEvent(QEvent * event, bool & isDone)
{
  switch (event->type()) {
  case QEvent::TouchBegin:
  case QEvent::TouchCancel:
  case QEvent::TouchEnd:
  case QEvent::TouchUpdate:
    return PRIVATE(this)->touchEvent((QTouchEvent *) event, isDone);
  default:
    return nullptr;
  }
}

#undef PRIVATE
