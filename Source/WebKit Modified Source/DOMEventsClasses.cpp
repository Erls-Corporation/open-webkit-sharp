/*
 * Copyright (C) 2006, 2007 Apple Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "WebKitDLL.h"
#include <initguid.h>
#include "DOMEventsClasses.h"

#include <WebCore/BString.h>
#include <WebCore/COMPtr.h>
#include <WebCore/DOMWindow.h>
#include <WebCore/Event.h>
#include <WebCore/EventNames.h>
#include <WebCore/KeyboardEvent.h>
#include <WebCore/MouseEvent.h>
#include <WebCore/ScriptExecutionContext.h>
#include <WebCore/OverflowEvent.h>

using WebCore::BString;
using WebCore::MutationEvent;
using WebCore::WheelEvent;
using WebCore::OverflowEvent;

// DOMEventListener -----------------------------------------------------------

HRESULT STDMETHODCALLTYPE DOMEventListener::QueryInterface(const IID &riid, void** ppvObject)
{
    *ppvObject = 0;
    if (IsEqualGUID(riid, IID_IDOMEventListener))
        *ppvObject = static_cast<IDOMEventListener*>(this);
    else
        return DOMObject::QueryInterface(riid, ppvObject);

    AddRef();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMEventListener::handleEvent( 
    /* [in] */ IDOMEvent* /*evt*/)
{
    return E_NOTIMPL;
}

WebEventListener::WebEventListener(IDOMEventListener* i)
    : EventListener(CPPEventListenerType)
    , m_iDOMEventListener(i)
{
    m_iDOMEventListener->AddRef();
}

WebEventListener::~WebEventListener()
{
    m_iDOMEventListener->Release();
}

bool WebEventListener::operator==(const WebCore::EventListener& other)
{
    return (other.type() == CPPEventListenerType 
        && reinterpret_cast<const WebEventListener*>(&other)->m_iDOMEventListener == m_iDOMEventListener);
}

void WebEventListener::handleEvent(WebCore::ScriptExecutionContext* s, WebCore::Event* e)
{
    RefPtr<WebCore::Event> ePtr(e);
    COMPtr<IDOMEvent> domEvent = DOMEvent::createInstance(ePtr);
    m_iDOMEventListener->handleEvent(domEvent.get());
}

PassRefPtr<WebEventListener> WebEventListener::create(IDOMEventListener* d)
{
    return adoptRef(new WebEventListener(d));
}

// DOMEvent -------------------------------------------------------------------

DOMEvent::DOMEvent(PassRefPtr<WebCore::Event> e)
: m_event(0)
{
    m_event = e;
}

DOMEvent::~DOMEvent()
{
}

IDOMEvent* DOMEvent::createInstance(PassRefPtr<WebCore::Event> e)
{
    if (!e)
        return 0;

    HRESULT hr;
    IDOMEvent* domEvent = 0;

    if (e->isKeyboardEvent()) {
        DOMKeyboardEvent* newEvent = new DOMKeyboardEvent(e);
        hr = newEvent->QueryInterface(IID_IDOMKeyboardEvent, (void**)&domEvent);
    } else if (e->isMouseEvent()) {
        DOMMouseEvent* newEvent = new DOMMouseEvent(e);
        hr = newEvent->QueryInterface(IID_IDOMMouseEvent, (void**)&domEvent);
    } else if (e->hasInterface(WebCore::eventNames().interfaceForMutationEvent)) {
        DOMMutationEvent* newEvent = new DOMMutationEvent(e);
        hr = newEvent->QueryInterface(IID_IDOMMutationEvent, (void**)&domEvent);
    } else if (e->hasInterface(WebCore::eventNames().interfaceForOverflowEvent)) {
        DOMOverflowEvent* newEvent = new DOMOverflowEvent(e);
        hr = newEvent->QueryInterface(IID_IDOMOverflowEvent, (void**)&domEvent);
    } else if (e->hasInterface(WebCore::eventNames().interfaceForWheelEvent)) {
        DOMWheelEvent* newEvent = new DOMWheelEvent(e);
        hr = newEvent->QueryInterface(IID_IDOMWheelEvent, (void**)&domEvent);
    } else if (e->isUIEvent()) {
        DOMUIEvent* newEvent = new DOMUIEvent(e);
        hr = newEvent->QueryInterface(IID_IDOMUIEvent, (void**)&domEvent);
    } else {
        DOMEvent* newEvent = new DOMEvent(e);
        hr = newEvent->QueryInterface(IID_IDOMEvent, (void**)&domEvent);
    }

    if (FAILED(hr))
        return 0;

    return domEvent;
}

HRESULT STDMETHODCALLTYPE DOMEvent::QueryInterface(const IID &riid, void** ppvObject)
{
    *ppvObject = 0;
    if (IsEqualGUID(riid, IID_DOMEvent))
        *ppvObject = this;
    else if (IsEqualGUID(riid, IID_IDOMEvent))
        *ppvObject = static_cast<IDOMEvent*>(this);
    else
        return DOMObject::QueryInterface(riid, ppvObject);

    AddRef();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMEvent::type( 
    /* [retval][out] */ BSTR* result)
{
    *result = BString(m_event->type()).release();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMEvent::target( 
    /* [retval][out] */ IDOMEventTarget** result)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE DOMEvent::currentTarget( 
    /* [retval][out] */ IDOMEventTarget** /*result*/)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE DOMEvent::eventPhase( 
    /* [retval][out] */ unsigned short* result)
{
    *result = m_event->eventPhase();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMEvent::bubbles( 
    /* [retval][out] */ BOOL* result)
{
	*result = m_event->bubbles() ? TRUE : FALSE;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMEvent::cancelable( 
    /* [retval][out] */ BOOL* result)
{
    *result = m_event->cancelable() ? TRUE : FALSE;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMEvent::timeStamp( 
    /* [retval][out] */ DOMTimeStamp* /*result*/)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE DOMEvent::stopPropagation( void)
{
	m_event->stopPropagation();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMEvent::preventDefault( void)
{
    m_event->preventDefault();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMEvent::initEvent( 
    /* [in] */ BSTR /*eventTypeArg*/,
    /* [in] */ BOOL /*canBubbleArg*/,
    /* [in] */ BOOL /*cancelableArg*/)
{
    return E_NOTIMPL;
}

// DOMUIEvent -----------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DOMUIEvent::QueryInterface(REFIID riid, void** ppvObject)
{
    *ppvObject = 0;
    if (IsEqualGUID(riid, IID_IDOMUIEvent))
        *ppvObject = static_cast<IDOMUIEvent*>(this);
    else
        return DOMEvent::QueryInterface(riid, ppvObject);

    AddRef();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMUIEvent::view( 
    /* [retval][out] */ IDOMWindow** /*result*/)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE DOMUIEvent::detail( 
    /* [retval][out] */ long* result)
{
	WebCore::UIEvent eve = static_cast<WebCore::UIEvent*>(m_event.get());
	*result = eve->detail();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMUIEvent::initUIEvent( 
    /* [in] */ BSTR /*type*/,
    /* [in] */ BOOL /*canBubble*/,
    /* [in] */ BOOL /*cancelable*/,
    /* [in] */ IDOMWindow* /*view*/,
    /* [in] */ long /*detail*/)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE DOMUIEvent::keyCode( 
    /* [retval][out] */ long* result)
{
	WebCore::UIEvent eve = static_cast<WebCore::UIEvent*>(m_event.get());
	*result = eve->keyCode();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMUIEvent::charCode( 
    /* [retval][out] */ long* result)
{
    WebCore::UIEvent eve = static_cast<WebCore::UIEvent*>(m_event.get());
	*result = eve->charCode();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMUIEvent::unused1(
    /* [retval][out] */ long* /*result*/)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE DOMUIEvent::unused2(
    /* [retval][out] */ long* /*result*/)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE DOMUIEvent::pageX( 
    /* [retval][out] */ long* result)
{
    WebCore::UIEvent eve = static_cast<WebCore::UIEvent*>(m_event.get());
	*result = eve->pageX();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMUIEvent::pageY( 
    /* [retval][out] */ long* result)
{
    WebCore::UIEvent eve = static_cast<WebCore::UIEvent*>(m_event.get());
	*result = eve->pageY();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMUIEvent::which( 
    /* [retval][out] */ long* result)
{
    WebCore::UIEvent eve = static_cast<WebCore::UIEvent*>(m_event.get());
	*result = eve->which();
	return S_OK;
}

// DOMKeyboardEvent -----------------------------------------------------------

HRESULT STDMETHODCALLTYPE DOMKeyboardEvent::QueryInterface(REFIID riid, void** ppvObject)
{
    *ppvObject = 0;
    if (IsEqualGUID(riid, IID_IDOMKeyboardEvent))
        *ppvObject = static_cast<IDOMKeyboardEvent*>(this);
    else
        return DOMUIEvent::QueryInterface(riid, ppvObject);

    AddRef();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMKeyboardEvent::keyIdentifier( 
    /* [retval][out] */ BSTR* result)
{
	WebCore::KeyboardEvent eve = static_cast<WebCore::KeyboardEvent*>(m_event.get());
	*result = BString(eve->keyIdentifier()).release();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMKeyboardEvent::keyLocation( 
    /* [retval][out] */ unsigned long* result)
{
    WebCore::KeyboardEvent eve = static_cast<WebCore::KeyboardEvent*>(m_event.get());
	*result = eve->keyLocation();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMKeyboardEvent::ctrlKey( 
    /* [retval][out] */ BOOL* result)
{
    *result = FALSE;
    if (!m_event || !m_event->isKeyboardEvent())
        return E_FAIL;
    WebCore::KeyboardEvent* keyEvent = static_cast<WebCore::KeyboardEvent*>(m_event.get());

    *result = keyEvent->ctrlKey() ? TRUE : FALSE;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMKeyboardEvent::shiftKey( 
    /* [retval][out] */ BOOL* result)
{
    *result = FALSE;
    if (!m_event || !m_event->isKeyboardEvent())
        return E_FAIL;
    WebCore::KeyboardEvent* keyEvent = static_cast<WebCore::KeyboardEvent*>(m_event.get());

    *result = keyEvent->shiftKey() ? TRUE : FALSE;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMKeyboardEvent::altKey( 
    /* [retval][out] */ BOOL* result)
{
    *result = FALSE;
    if (!m_event || !m_event->isKeyboardEvent())
        return E_FAIL;
    WebCore::KeyboardEvent* keyEvent = static_cast<WebCore::KeyboardEvent*>(m_event.get());

    *result = keyEvent->altKey() ? TRUE : FALSE;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMKeyboardEvent::metaKey( 
    /* [retval][out] */ BOOL* result)
{
    *result = FALSE;
    if (!m_event || !m_event->isKeyboardEvent())
        return E_FAIL;
    WebCore::KeyboardEvent* keyEvent = static_cast<WebCore::KeyboardEvent*>(m_event.get());

    *result = keyEvent->metaKey() ? TRUE : FALSE;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMKeyboardEvent::altGraphKey( 
    /* [retval][out] */ BOOL* result)
{
    *result = FALSE;
    if (!m_event || !m_event->isKeyboardEvent())
        return E_FAIL;
    WebCore::KeyboardEvent* keyEvent = static_cast<WebCore::KeyboardEvent*>(m_event.get());

    *result = keyEvent->altGraphKey() ? TRUE : FALSE;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMKeyboardEvent::getModifierState( 
    /* [in] */ BSTR keyIdentifierArg,
    /* [retval][out] */ BOOL* result)
{
    WebCore::KeyboardEvent* keyEvent = static_cast<WebCore::KeyboardEvent*>(m_event.get());
	String keyId(keyIdentifierArg, SysStringLen(keyIdentifierArg));
	*result = keyEvent->getModifierState(keyId) ? TRUE : FALSE;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMKeyboardEvent::initKeyboardEvent( 
    /* [in] */ BSTR /*type*/,
    /* [in] */ BOOL /*canBubble*/,
    /* [in] */ BOOL /*cancelable*/,
    /* [in] */ IDOMWindow* /*view*/,
    /* [in] */ BSTR /*keyIdentifier*/,
    /* [in] */ unsigned long /*keyLocation*/,
    /* [in] */ BOOL /*ctrlKey*/,
    /* [in] */ BOOL /*altKey*/,
    /* [in] */ BOOL /*shiftKey*/,
    /* [in] */ BOOL /*metaKey*/,
    /* [in] */ BOOL /*graphKey*/)
{
    return E_NOTIMPL;
}

// DOMMouseEvent --------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DOMMouseEvent::QueryInterface(REFIID riid, void** ppvObject)
{
    *ppvObject = 0;
    if (IsEqualGUID(riid, IID_IDOMMouseEvent))
        *ppvObject = static_cast<IDOMMouseEvent*>(this);
    else
        return DOMUIEvent::QueryInterface(riid, ppvObject);

    AddRef();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMMouseEvent::screenX( 
    /* [retval][out] */ long* result)
{
    WebCore::MouseEvent* mEvent = static_cast<WebCore::MouseEvent*>(m_event.get());
	WebCore::MouseRelatedEvent* mEvent1 = static_cast<WebCore::MouseRelatedEvent*>(mEvent);
	*result = mEvent1->screenX();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMMouseEvent::screenY( 
    /* [retval][out] */ long* result)
{
    WebCore::MouseEvent* mEvent = static_cast<WebCore::MouseEvent*>(m_event.get());
	WebCore::MouseRelatedEvent* mEvent1 = static_cast<WebCore::MouseRelatedEvent*>(mEvent);
	*result = mEvent1->screenY();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMMouseEvent::clientX( 
    /* [retval][out] */ long* result)
{
    WebCore::MouseEvent* mEvent = static_cast<WebCore::MouseEvent*>(m_event.get());
	WebCore::MouseRelatedEvent* mEvent1 = static_cast<WebCore::MouseRelatedEvent*>(mEvent);
	*result = mEvent1->clientX();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMMouseEvent::clientY( 
    /* [retval][out] */ long* result)
{
    WebCore::MouseEvent* mEvent = static_cast<WebCore::MouseEvent*>(m_event.get());
	WebCore::MouseRelatedEvent* mEvent1 = static_cast<WebCore::MouseRelatedEvent*>(mEvent);
	*result = mEvent1->clientY();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMMouseEvent::ctrlKey( 
    /* [retval][out] */ BOOL* result)
{
    *result = FALSE;
    if (!m_event || !m_event->isMouseEvent())
        return E_FAIL;
    WebCore::MouseEvent* mouseEvent = static_cast<WebCore::MouseEvent*>(m_event.get());

    *result = mouseEvent->ctrlKey() ? TRUE : FALSE;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMMouseEvent::shiftKey( 
    /* [retval][out] */ BOOL* result)
{
    *result = FALSE;
    if (!m_event || !m_event->isMouseEvent())
        return E_FAIL;
    WebCore::MouseEvent* mouseEvent = static_cast<WebCore::MouseEvent*>(m_event.get());

    *result = mouseEvent->shiftKey() ? TRUE : FALSE;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMMouseEvent::altKey( 
    /* [retval][out] */ BOOL* result)
{
    *result = FALSE;
    if (!m_event || !m_event->isMouseEvent())
        return E_FAIL;
    WebCore::MouseEvent* mouseEvent = static_cast<WebCore::MouseEvent*>(m_event.get());

    *result = mouseEvent->altKey() ? TRUE : FALSE;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMMouseEvent::metaKey( 
    /* [retval][out] */ BOOL* result)
{
    *result = FALSE;
    if (!m_event || !m_event->isMouseEvent())
        return E_FAIL;
    WebCore::MouseEvent* mouseEvent = static_cast<WebCore::MouseEvent*>(m_event.get());

    *result = mouseEvent->metaKey() ? TRUE : FALSE;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMMouseEvent::button( 
    /* [retval][out] */ unsigned short* result)
{
    WebCore::MouseEvent* mEvent = static_cast<WebCore::MouseEvent*>(m_event.get());
	*result = mEvent->button();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMMouseEvent::relatedTarget( 
    /* [retval][out] */ IDOMEventTarget** /*result*/)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE DOMMouseEvent::initMouseEvent( 
    /* [in] */ BSTR /*type*/,
    /* [in] */ BOOL /*canBubble*/,
    /* [in] */ BOOL /*cancelable*/,
    /* [in] */ IDOMWindow* /*view*/,
    /* [in] */ long /*detail*/,
    /* [in] */ long /*screenX*/,
    /* [in] */ long /*screenY*/,
    /* [in] */ long /*clientX*/,
    /* [in] */ long /*clientY*/,
    /* [in] */ BOOL /*ctrlKey*/,
    /* [in] */ BOOL /*altKey*/,
    /* [in] */ BOOL /*shiftKey*/,
    /* [in] */ BOOL /*metaKey*/,
    /* [in] */ unsigned short /*button*/,
    /* [in] */ IDOMEventTarget* /*relatedTarget*/)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE DOMMouseEvent::offsetX( 
    /* [retval][out] */ long* result)
{
    WebCore::MouseEvent* mEvent = static_cast<WebCore::MouseEvent*>(m_event.get());
	WebCore::MouseRelatedEvent* mEvent1 = static_cast<WebCore::MouseRelatedEvent*>(mEvent);
	*result = mEvent1->offsetX();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMMouseEvent::offsetY( 
    /* [retval][out] */ long* result)
{
    WebCore::MouseEvent* mEvent = static_cast<WebCore::MouseEvent*>(m_event.get());
	WebCore::MouseRelatedEvent* mEvent1 = static_cast<WebCore::MouseRelatedEvent*>(mEvent);
	*result = mEvent1->offsetY();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMMouseEvent::x( 
    /* [retval][out] */ long* result)
{
    WebCore::MouseEvent* mEvent = static_cast<WebCore::MouseEvent*>(m_event.get());
	WebCore::MouseRelatedEvent* mEvent1 = static_cast<WebCore::MouseRelatedEvent*>(mEvent);
	*result = mEvent1->x();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMMouseEvent::y( 
    /* [retval][out] */ long* result)
{
    WebCore::MouseEvent* mEvent = static_cast<WebCore::MouseEvent*>(m_event.get());
	WebCore::MouseRelatedEvent* mEvent1 = static_cast<WebCore::MouseRelatedEvent*>(mEvent);
	*result = mEvent1->y();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMMouseEvent::fromElement( 
    /* [retval][out] */ IDOMNode** result)
{
    WebCore::MouseEvent* mEvent = static_cast<WebCore::MouseEvent*>(m_event.get());
	*result = DOMNode::createInstance(mEvent->fromElement());
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMMouseEvent::toElement( 
    /* [retval][out] */ IDOMNode** result)
{
    WebCore::MouseEvent* mEvent = static_cast<WebCore::MouseEvent*>(m_event.get());
	*result = DOMNode::createInstance(mEvent->toElement());
	return S_OK;
}

// DOMMutationEvent -----------------------------------------------------------

HRESULT STDMETHODCALLTYPE DOMMutationEvent::QueryInterface(REFIID riid, void** ppvObject)
{
    *ppvObject = 0;
    if (IsEqualGUID(riid, IID_IDOMMutationEvent))
        *ppvObject = static_cast<IDOMMutationEvent*>(this);
    else
        return DOMEvent::QueryInterface(riid, ppvObject);

    AddRef();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMMutationEvent::relatedNode( 
    /* [retval][out] */ IDOMNode** result)
{
	WebCore::MutationEvent* mEvent = static_cast<WebCore::MutationEvent*>(m_event.get());
	*result = DOMNode::createInstance(mEvent->relatedNode());
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMMutationEvent::prevValue( 
    /* [retval][out] */ BSTR* result)
{
    WebCore::MutationEvent* mEvent = static_cast<WebCore::MutationEvent*>(m_event.get());
	*result = BString(mEvent->prevValue()).release();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMMutationEvent::newValue( 
    /* [retval][out] */ BSTR* result)
{
    WebCore::MutationEvent* mEvent = static_cast<WebCore::MutationEvent*>(m_event.get());
	*result = BString(mEvent->newValue()).release();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMMutationEvent::attrName( 
    /* [retval][out] */ BSTR* result)
{
    WebCore::MutationEvent* mEvent = static_cast<WebCore::MutationEvent*>(m_event.get());
	*result = BString(mEvent->attrName()).release();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMMutationEvent::attrChange( 
    /* [retval][out] */ unsigned short* result)
{
    WebCore::MutationEvent* mEvent = static_cast<WebCore::MutationEvent*>(m_event.get());
	*result = mEvent->attrChange();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMMutationEvent::initMutationEvent(
    /* [in] */ BSTR /*type*/,
    /* [in] */ BOOL /*canBubble*/,
    /* [in] */ BOOL /*cancelable*/,
    /* [in] */ IDOMNode* /*relatedNode*/,
    /* [in] */ BSTR /*prevValue*/,
    /* [in] */ BSTR /*newValue*/,
    /* [in] */ BSTR /*attrName*/,
    /* [in] */ unsigned short /*attrChange*/)
{
    return E_NOTIMPL;
}

// DOMOverflowEvent -----------------------------------------------------------

HRESULT STDMETHODCALLTYPE DOMOverflowEvent::QueryInterface(REFIID riid, void** ppvObject)
{
    *ppvObject = 0;
    if (IsEqualGUID(riid, IID_IDOMOverflowEvent))
        *ppvObject = static_cast<IDOMOverflowEvent*>(this);
    else
        return DOMEvent::QueryInterface(riid, ppvObject);

    AddRef();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMOverflowEvent::orient( 
    /* [retval][out] */ unsigned short* result)
{
    WebCore::OverflowEvent* mEvent = static_cast<WebCore::OverflowEvent*>(m_event.get());
	*result = mEvent->orient();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMOverflowEvent::horizontalOverflow( 
    /* [retval][out] */ BOOL* result)
{
    WebCore::OverflowEvent* mEvent = static_cast<WebCore::OverflowEvent*>(m_event.get());
	*result = mEvent->horizontalOverflow() ? TRUE : FALSE
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMOverflowEvent::verticalOverflow( 
    /* [retval][out] */ BOOL* result)
{
    WebCore::OverflowEvent* mEvent = static_cast<WebCore::OverflowEvent*>(m_event.get());
	*result = mEvent->verticalOverflow() ? TRUE : FALSE
	return S_OK;
}

// DOMWheelEvent --------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DOMWheelEvent::QueryInterface(REFIID riid, void** ppvObject)
{
    *ppvObject = 0;
    if (IsEqualGUID(riid, IID_IDOMWheelEvent))
        *ppvObject = static_cast<IDOMWheelEvent*>(this);
    else
        return DOMUIEvent::QueryInterface(riid, ppvObject);

    AddRef();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMWheelEvent::screenX( 
    /* [retval][out] */ long* result)
{
    WebCore::WheelEvent* mEvent = static_cast<WebCore::WheelEvent*>(m_event.get());
	*result = mEvent->screenX();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMWheelEvent::screenY( 
    /* [retval][out] */ long* result)
{
    WebCore::WheelEvent* mEvent = static_cast<WebCore::WheelEvent*>(m_event.get());
	*result = mEvent->screenY();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMWheelEvent::clientX( 
    /* [retval][out] */ long* result)
{
    WebCore::WheelEvent* mEvent = static_cast<WebCore::WheelEvent*>(m_event.get());
	*result = mEvent->clientX();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMWheelEvent::clientY( 
    /* [retval][out] */ long* result)
{
    WebCore::WheelEvent* mEvent = static_cast<WebCore::WheelEvent*>(m_event.get());
	*result = mEvent->clientY();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMWheelEvent::ctrlKey( 
    /* [retval][out] */ BOOL* result)
{
    WebCore::WheelEvent* mEvent = static_cast<WebCore::WheelEvent*>(m_event.get());
	*result = mEvent->ctrlKey() ? TRUE : FALSE
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMWheelEvent::shiftKey( 
    /* [retval][out] */ BOOL* result)
{
    WebCore::WheelEvent* mEvent = static_cast<WebCore::WheelEvent*>(m_event.get());
	*result = mEvent->shiftKey() ? TRUE : FALSE
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMWheelEvent::altKey( 
    /* [retval][out] */ BOOL* result)
{
    WebCore::WheelEvent* mEvent = static_cast<WebCore::WheelEvent*>(m_event.get());
	*result = mEvent->altKey() ? TRUE : FALSE
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMWheelEvent::metaKey( 
    /* [retval][out] */ BOOL* result)
{
    WebCore::WheelEvent* mEvent = static_cast<WebCore::WheelEvent*>(m_event.get());
	*result = mEvent->metaKey() ? TRUE : FALSE
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMWheelEvent::wheelDelta( 
    /* [retval][out] */ long* result)
{
    WebCore::WheelEvent* mEvent = static_cast<WebCore::WheelEvent*>(m_event.get());
	*result = mEvent->clientX();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMWheelEvent::wheelDeltaX( 
    /* [retval][out] */ long* result)
{
    WebCore::WheelEvent* mEvent = static_cast<WebCore::WheelEvent*>(m_event.get());
	*result = mEvent->wheelDeltaX();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMWheelEvent::wheelDeltaY( 
    /* [retval][out] */ long* result)
{
    WebCore::WheelEvent* mEvent = static_cast<WebCore::WheelEvent*>(m_event.get());
	*result = mEvent->wheelDeltaY();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMWheelEvent::offsetX( 
    /* [retval][out] */ long* result)
{
    WebCore::WheelEvent* mEvent = static_cast<WebCore::WheelEvent*>(m_event.get());
	*result = mEvent->offsetX();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMWheelEvent::offsetY( 
    /* [retval][out] */ long* result)
{
    WebCore::WheelEvent* mEvent = static_cast<WebCore::WheelEvent*>(m_event.get());
	*result = mEvent->offsetY();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMWheelEvent::x( 
    /* [retval][out] */ long* result)
{
    WebCore::WheelEvent* mEvent = static_cast<WebCore::WheelEvent*>(m_event.get());
	*result = mEvent->x();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMWheelEvent::y( 
    /* [retval][out] */ long* result)
{
    WebCore::WheelEvent* mEvent = static_cast<WebCore::WheelEvent*>(m_event.get());
	*result = mEvent->y();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMWheelEvent::isHorizontal( 
    /* [retval][out] */ BOOL* result)
{
    WebCore::WheelEvent* mEvent = static_cast<WebCore::WheelEvent*>(m_event.get());
	*result = mEvent->isHorizontal() ? TRUE : FALSE
	return S_OK;
}

HRESULT STDMETHODCALLTYPE DOMWheelEvent::initWheelEvent( 
    /* [in] */ long /*wheelDeltaX*/,
    /* [in] */ long /*wheelDeltaY*/,
    /* [in] */ IDOMWindow* /*view*/,
    /* [in] */ long /*screenX*/,
    /* [in] */ long /*screenY*/,
    /* [in] */ long /*clientX*/,
    /* [in] */ long /*clientY*/,
    /* [in] */ BOOL /*ctrlKey*/,
    /* [in] */ BOOL /*altKey*/,
    /* [in] */ BOOL /*shiftKey*/,
    /* [in] */ BOOL /*metaKey*/)
{
    return E_NOTIMPL;
}
