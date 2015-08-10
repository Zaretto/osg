/* -*-c++-*- OpenSceneGraph - Copyright (C) 2009 Wang Rui
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
*/

#include <osg/DeleteHandler>
#include <osgQt/GraphicsWindowQt5>
#include <osgViewer/ViewerBase>
#include <QInputEvent>

#include <QDebug>
#include <QThread>

#include <QCoreApplication>
#include <QAbstractEventDispatcher>
#include <QOpenGLContext>
#include <QSurfaceFormat>

/*
#if (QT_VERSION>=QT_VERSION_CHECK(4, 6, 0))
# define USE_GESTURES
# include <QGestureEvent>
# include <QGesture>
#endif
*/

using namespace osgQt;


class QtKeyboardMap
{

public:
    QtKeyboardMap()
    {
        mKeyMap[Qt::Key_Escape     ] = osgGA::GUIEventAdapter::KEY_Escape;
        mKeyMap[Qt::Key_Delete   ] = osgGA::GUIEventAdapter::KEY_Delete;
        mKeyMap[Qt::Key_Home       ] = osgGA::GUIEventAdapter::KEY_Home;
        mKeyMap[Qt::Key_Enter      ] = osgGA::GUIEventAdapter::KEY_KP_Enter;
        mKeyMap[Qt::Key_End        ] = osgGA::GUIEventAdapter::KEY_End;
        mKeyMap[Qt::Key_Return     ] = osgGA::GUIEventAdapter::KEY_Return;
        mKeyMap[Qt::Key_PageUp     ] = osgGA::GUIEventAdapter::KEY_Page_Up;
        mKeyMap[Qt::Key_PageDown   ] = osgGA::GUIEventAdapter::KEY_Page_Down;
        mKeyMap[Qt::Key_Left       ] = osgGA::GUIEventAdapter::KEY_Left;
        mKeyMap[Qt::Key_Right      ] = osgGA::GUIEventAdapter::KEY_Right;
        mKeyMap[Qt::Key_Up         ] = osgGA::GUIEventAdapter::KEY_Up;
        mKeyMap[Qt::Key_Down       ] = osgGA::GUIEventAdapter::KEY_Down;
        mKeyMap[Qt::Key_Backspace  ] = osgGA::GUIEventAdapter::KEY_BackSpace;
        mKeyMap[Qt::Key_Tab        ] = osgGA::GUIEventAdapter::KEY_Tab;
        mKeyMap[Qt::Key_Space      ] = osgGA::GUIEventAdapter::KEY_Space;
        mKeyMap[Qt::Key_Delete     ] = osgGA::GUIEventAdapter::KEY_Delete;
        mKeyMap[Qt::Key_Alt      ] = osgGA::GUIEventAdapter::KEY_Alt_L;
        mKeyMap[Qt::Key_Shift    ] = osgGA::GUIEventAdapter::KEY_Shift_L;
        mKeyMap[Qt::Key_Control  ] = osgGA::GUIEventAdapter::KEY_Control_L;
        mKeyMap[Qt::Key_Meta     ] = osgGA::GUIEventAdapter::KEY_Meta_L;

        mKeyMap[Qt::Key_F1             ] = osgGA::GUIEventAdapter::KEY_F1;
        mKeyMap[Qt::Key_F2             ] = osgGA::GUIEventAdapter::KEY_F2;
        mKeyMap[Qt::Key_F3             ] = osgGA::GUIEventAdapter::KEY_F3;
        mKeyMap[Qt::Key_F4             ] = osgGA::GUIEventAdapter::KEY_F4;
        mKeyMap[Qt::Key_F5             ] = osgGA::GUIEventAdapter::KEY_F5;
        mKeyMap[Qt::Key_F6             ] = osgGA::GUIEventAdapter::KEY_F6;
        mKeyMap[Qt::Key_F7             ] = osgGA::GUIEventAdapter::KEY_F7;
        mKeyMap[Qt::Key_F8             ] = osgGA::GUIEventAdapter::KEY_F8;
        mKeyMap[Qt::Key_F9             ] = osgGA::GUIEventAdapter::KEY_F9;
        mKeyMap[Qt::Key_F10            ] = osgGA::GUIEventAdapter::KEY_F10;
        mKeyMap[Qt::Key_F11            ] = osgGA::GUIEventAdapter::KEY_F11;
        mKeyMap[Qt::Key_F12            ] = osgGA::GUIEventAdapter::KEY_F12;
        mKeyMap[Qt::Key_F13            ] = osgGA::GUIEventAdapter::KEY_F13;
        mKeyMap[Qt::Key_F14            ] = osgGA::GUIEventAdapter::KEY_F14;
        mKeyMap[Qt::Key_F15            ] = osgGA::GUIEventAdapter::KEY_F15;
        mKeyMap[Qt::Key_F16            ] = osgGA::GUIEventAdapter::KEY_F16;
        mKeyMap[Qt::Key_F17            ] = osgGA::GUIEventAdapter::KEY_F17;
        mKeyMap[Qt::Key_F18            ] = osgGA::GUIEventAdapter::KEY_F18;
        mKeyMap[Qt::Key_F19            ] = osgGA::GUIEventAdapter::KEY_F19;
        mKeyMap[Qt::Key_F20            ] = osgGA::GUIEventAdapter::KEY_F20;

        mKeyMap[Qt::Key_hyphen         ] = '-';
        mKeyMap[Qt::Key_Equal         ] = '=';

        mKeyMap[Qt::Key_division      ] = osgGA::GUIEventAdapter::KEY_KP_Divide;
        mKeyMap[Qt::Key_multiply      ] = osgGA::GUIEventAdapter::KEY_KP_Multiply;
        mKeyMap[Qt::Key_Minus         ] = '-';
        mKeyMap[Qt::Key_Plus          ] = '+';
        //mKeyMap[Qt::Key_H              ] = osgGA::GUIEventAdapter::KEY_KP_Home;
        //mKeyMap[Qt::Key_                    ] = osgGA::GUIEventAdapter::KEY_KP_Up;
        //mKeyMap[92                    ] = osgGA::GUIEventAdapter::KEY_KP_Page_Up;
        //mKeyMap[86                    ] = osgGA::GUIEventAdapter::KEY_KP_Left;
        //mKeyMap[87                    ] = osgGA::GUIEventAdapter::KEY_KP_Begin;
        //mKeyMap[88                    ] = osgGA::GUIEventAdapter::KEY_KP_Right;
        //mKeyMap[83                    ] = osgGA::GUIEventAdapter::KEY_KP_End;
        //mKeyMap[84                    ] = osgGA::GUIEventAdapter::KEY_KP_Down;
        //mKeyMap[85                    ] = osgGA::GUIEventAdapter::KEY_KP_Page_Down;
        mKeyMap[Qt::Key_Insert        ] = osgGA::GUIEventAdapter::KEY_KP_Insert;
        //mKeyMap[Qt::Key_Delete        ] = osgGA::GUIEventAdapter::KEY_KP_Delete;
    }

    ~QtKeyboardMap()
    {
    }

    int remapKey(QKeyEvent* event)
    {
        KeyMap::iterator itr = mKeyMap.find(event->key());
        if (itr == mKeyMap.end())
        {
            return int(*(event->text().toLatin1().data()));
        }
        else
            return itr->second;
    }

    private:
    typedef std::map<unsigned int, int> KeyMap;
    KeyMap mKeyMap;
};

static QtKeyboardMap s_QtKeyboardMap;


/// The object responsible for the scene re-rendering.
class HeartBeat : public QObject
{
public:
  int _timerId;
  osg::Timer _lastFrameStartTime;
  osg::observer_ptr< osgViewer::ViewerBase > _viewer;

  HeartBeat();
  virtual ~HeartBeat();
  void init( osgViewer::ViewerBase *viewer );
  void stopTimer();
  void timerEvent( QTimerEvent *event );
};

static HeartBeat heartBeat;

#if (QT_VERSION < QT_VERSION_CHECK(5, 2, 0))
    #define GETDEVICEPIXELRATIO() 1.0
#else
    #define GETDEVICEPIXELRATIO() devicePixelRatio()
#endif

GLWindow::GLWindow( )
: QWindow(),
_gw( NULL ),
_touchEventsEnabled( false ),
_forwardKeyEvents( false )
{
    _devicePixelRatio = GETDEVICEPIXELRATIO();
}

#if 0
GLWidget::GLWidget( QGLContext* context, QWidget* parent, const QGLWidget* shareWidget, Qt::WindowFlags f,
                    bool forwardKeyEvents )
: QGLWidget(context, parent, shareWidget, f),
_gw( NULL ),
_touchEventsEnabled( false ),
_forwardKeyEvents( forwardKeyEvents )
{
    _devicePixelRatio = GETDEVICEPIXELRATIO();
}

GLWidget::GLWidget( const QGLFormat& format, QWidget* parent, const QGLWidget* shareWidget, Qt::WindowFlags f,
                    bool forwardKeyEvents )
: QGLWidget(format, parent, shareWidget, f),
_gw( NULL ),
_touchEventsEnabled( false ),
_forwardKeyEvents( forwardKeyEvents )
{
    _devicePixelRatio = GETDEVICEPIXELRATIO();
}
#endif

GLWindow::~GLWindow()
{
    // close GraphicsWindowQt and remove the reference to us
    if( _gw )
    {
        _gw->close();
        _gw->_window = NULL;
        _gw = NULL;
    }
}

void GLWindow::setTouchEventsEnabled(bool e)
{
#ifdef USE_GESTURES
    if (e==_touchEventsEnabled)
        return;

    _touchEventsEnabled = e;

    if (_touchEventsEnabled)
    {
        grabGesture(Qt::PinchGesture);
    }
    else
    {
        ungrabGesture(Qt::PinchGesture);
    }
#endif
}

void GLWindow::processDeferredEvents()
{
    QQueue<QEvent::Type> deferredEventQueueCopy;
    {
        QMutexLocker lock(&_deferredEventQueueMutex);
        deferredEventQueueCopy = _deferredEventQueue;
        _eventCompressor.clear();
        _deferredEventQueue.clear();
    }

    while (!deferredEventQueueCopy.isEmpty())
    {
        QEvent event(deferredEventQueueCopy.dequeue());
        QWindow::event(&event);
    }
}

bool GLWindow::event( QEvent* event )
{
#ifdef USE_GESTURES
    if ( event->type()==QEvent::Gesture )
        return gestureEvent(static_cast<QGestureEvent*>(event));
#endif

    // QEvent::Hide
    //
    // workaround "Qt-workaround" that does glFinish before hiding the widget
    // (the Qt workaround was seen at least in Qt 4.6.3 and 4.7.0)
    //
    // Qt makes the context current, performs glFinish, and releases the context.
    // This makes the problem in OSG multithreaded environment as the context
    // is active in another thread, thus it can not be made current for the purpose
    // of glFinish in this thread.

    // QEvent::ParentChange
    //
    // Reparenting GLWidget may create a new underlying window and a new GL context.
    // Qt will then call doneCurrent on the GL context about to be deleted. The thread
    // where old GL context was current has no longer current context to render to and
    // we cannot make new GL context current in this thread.

    // We workaround above problems by deferring execution of problematic event requests.
    // These events has to be enqueue and executed later in a main GUI thread (GUI operations
    // outside the main thread are not allowed) just before makeCurrent is called from the
    // right thread. The good place for doing that is right after swap in a swapBuffersImplementation.

    if (event->type() == QEvent::Hide)
    {
        // enqueue only the last of QEvent::Hide and QEvent::Show
        enqueueDeferredEvent(QEvent::Hide, QEvent::Show);
        return true;
    }
    else if (event->type() == QEvent::Show)
    {
        // enqueue only the last of QEvent::Show or QEvent::Hide
        enqueueDeferredEvent(QEvent::Show, QEvent::Hide);
        return true;
    }
    else if (event->type() == QEvent::ParentChange)
    {
        // enqueue only the last QEvent::ParentChange
        enqueueDeferredEvent(QEvent::ParentChange);
        return true;
    }

    // perform regular event handling
    return QWindow::event( event );
}

void GLWindow::setKeyboardModifiers( QInputEvent* event )
{
    int modkey = event->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier);
    unsigned int mask = 0;
    if ( modkey & Qt::ShiftModifier ) mask |= osgGA::GUIEventAdapter::MODKEY_SHIFT;
    if ( modkey & Qt::ControlModifier ) mask |= osgGA::GUIEventAdapter::MODKEY_CTRL;
    if ( modkey & Qt::AltModifier ) mask |= osgGA::GUIEventAdapter::MODKEY_ALT;
    _gw->getEventQueue()->getCurrentEventState()->setModKeyMask( mask );
}

void GLWindow::resizeEvent( QResizeEvent* event )
{
    const QSize& size = event->size();
    int scaled_width = static_cast<int>(size.width()*_devicePixelRatio);
    int scaled_height = static_cast<int>(size.height()*_devicePixelRatio);
    _gw->resized( x(), y(), scaled_width,  scaled_height);
    _gw->getEventQueue()->windowResize( x(), y(), scaled_width, scaled_height );
    _gw->requestRedraw();
}

void GLWindow::moveEvent( QMoveEvent* event )
{
    const QPoint& pos = event->pos();
    int scaled_width = static_cast<int>(width()*_devicePixelRatio);
    int scaled_height = static_cast<int>(height()*_devicePixelRatio);
    _gw->resized( pos.x(), pos.y(), scaled_width,  scaled_height );
    _gw->getEventQueue()->windowResize( pos.x(), pos.y(), scaled_width,  scaled_height );
}

void GLWindow::keyPressEvent( QKeyEvent* event )
{
    setKeyboardModifiers( event );
    int value = s_QtKeyboardMap.remapKey( event );
    _gw->getEventQueue()->keyPress( value );

    // this passes the event to the regular Qt key event processing,
    // among others, it closes popup windows on ESC and forwards the event to the parent widgets
    if( _forwardKeyEvents )
        inherited::keyPressEvent( event );
}

void GLWindow::keyReleaseEvent( QKeyEvent* event )
{
    if( event->isAutoRepeat() )
    {
        event->ignore();
    }
    else
    {
        setKeyboardModifiers( event );
        int value = s_QtKeyboardMap.remapKey( event );
        _gw->getEventQueue()->keyRelease( value );
    }

    // this passes the event to the regular Qt key event processing,
    // among others, it closes popup windows on ESC and forwards the event to the parent widgets
    if( _forwardKeyEvents )
        inherited::keyReleaseEvent( event );
}

void GLWindow::mousePressEvent( QMouseEvent* event )
{
    int button = 0;
    switch ( event->button() )
    {
        case Qt::LeftButton: button = 1; break;
        case Qt::MidButton: button = 2; break;
        case Qt::RightButton: button = 3; break;
        case Qt::NoButton: button = 0; break;
        default: button = 0; break;
    }
    setKeyboardModifiers( event );
    _gw->getEventQueue()->mouseButtonPress( event->x()*_devicePixelRatio, event->y()*_devicePixelRatio, button );
}

void GLWindow::mouseReleaseEvent( QMouseEvent* event )
{
    int button = 0;
    switch ( event->button() )
    {
        case Qt::LeftButton: button = 1; break;
        case Qt::MidButton: button = 2; break;
        case Qt::RightButton: button = 3; break;
        case Qt::NoButton: button = 0; break;
        default: button = 0; break;
    }
    setKeyboardModifiers( event );
    _gw->getEventQueue()->mouseButtonRelease( event->x()*_devicePixelRatio, event->y()*_devicePixelRatio, button );
}

void GLWindow::mouseDoubleClickEvent( QMouseEvent* event )
{
    int button = 0;
    switch ( event->button() )
    {
        case Qt::LeftButton: button = 1; break;
        case Qt::MidButton: button = 2; break;
        case Qt::RightButton: button = 3; break;
        case Qt::NoButton: button = 0; break;
        default: button = 0; break;
    }
    setKeyboardModifiers( event );
    _gw->getEventQueue()->mouseDoubleButtonPress( event->x()*_devicePixelRatio, event->y()*_devicePixelRatio, button );
}

void GLWindow::mouseMoveEvent( QMouseEvent* event )
{
    setKeyboardModifiers( event );
    _gw->getEventQueue()->mouseMotion( event->x()*_devicePixelRatio, event->y()*_devicePixelRatio );
}

void GLWindow::wheelEvent( QWheelEvent* event )
{
    setKeyboardModifiers( event );
    _gw->getEventQueue()->mouseScroll(
        event->orientation() == Qt::Vertical ?
            (event->delta()>0 ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN) :
            (event->delta()>0 ? osgGA::GUIEventAdapter::SCROLL_LEFT : osgGA::GUIEventAdapter::SCROLL_RIGHT) );
}

#ifdef USE_GESTURES
static osgGA::GUIEventAdapter::TouchPhase translateQtGestureState( Qt::GestureState state )
{
    osgGA::GUIEventAdapter::TouchPhase touchPhase;
    switch ( state )
    {
        case Qt::GestureStarted:
            touchPhase = osgGA::GUIEventAdapter::TOUCH_BEGAN;
            break;
        case Qt::GestureUpdated:
            touchPhase = osgGA::GUIEventAdapter::TOUCH_MOVED;
            break;
        case Qt::GestureFinished:
        case Qt::GestureCanceled:
            touchPhase = osgGA::GUIEventAdapter::TOUCH_ENDED;
            break;
        default:
            touchPhase = osgGA::GUIEventAdapter::TOUCH_UNKNOWN;
    };

    return touchPhase;
}
#endif


bool GLWindow::gestureEvent( QGestureEvent* qevent )
{
#ifndef USE_GESTURES
    return false;
#else

    bool accept = false;

    if ( QPinchGesture* pinch = static_cast<QPinchGesture *>(qevent->gesture(Qt::PinchGesture) ) )
    {
    const QPointF qcenterf = pinch->centerPoint();
    const float angle = pinch->totalRotationAngle();
    const float scale = pinch->totalScaleFactor();

    const QPoint pinchCenterQt = mapFromGlobal(qcenterf.toPoint());
    const osg::Vec2 pinchCenter( pinchCenterQt.x(), pinchCenterQt.y() );

        //We don't have absolute positions of the two touches, only a scale and rotation
        //Hence we create pseudo-coordinates which are reasonable, and centered around the
        //real position
        const float radius = (width()+height())/4;
        const osg::Vec2 vector( scale*cos(angle)*radius, scale*sin(angle)*radius);
        const osg::Vec2 p0 = pinchCenter+vector;
        const osg::Vec2 p1 = pinchCenter-vector;

        osg::ref_ptr<osgGA::GUIEventAdapter> event = 0;
        const osgGA::GUIEventAdapter::TouchPhase touchPhase = translateQtGestureState( pinch->state() );
        if ( touchPhase==osgGA::GUIEventAdapter::TOUCH_BEGAN )
        {
            event = _gw->getEventQueue()->touchBegan(0 , touchPhase, p0[0], p0[1] );
        }
        else if ( touchPhase==osgGA::GUIEventAdapter::TOUCH_MOVED )
        {
            event = _gw->getEventQueue()->touchMoved( 0, touchPhase, p0[0], p0[1] );
        }
        else
        {
            event = _gw->getEventQueue()->touchEnded( 0, touchPhase, p0[0], p0[1], 1 );
        }

        if ( event )
        {
            event->addTouchPoint( 1, touchPhase, p1[0], p1[1] );
            accept = true;
        }
    }

    if ( accept )
        qevent->accept();

    return accept;
#endif
}



GraphicsWindowQt5::GraphicsWindowQt5(osg::GraphicsContext::Traits* traits)
:   _realized(false)
{
    _window = NULL;
    _context = NULL;
    _traits = traits;
    init(0);
}

GraphicsWindowQt5::~GraphicsWindowQt5()
{
    qDebug() << "Destroying graphics window, ho hum";
    close();

    // remove reference from GLWindow
    if ( _window )
        _window->_gw = NULL;
}

bool GraphicsWindowQt5::init( Qt::WindowFlags f )
{
    // update _widget and parent by WindowData
    WindowData* windowData = _traits.get() ? dynamic_cast<WindowData*>(_traits->inheritedWindowData.get()) : 0;
    if ( !_window )
        _window = windowData ? windowData->_window : NULL;

    // create widget if it does not exist
    _ownsWidget = (_window == NULL);
    if ( !_window )
    {
        // WindowFlags
        Qt::WindowFlags flags = f | Qt::Window | Qt::CustomizeWindowHint;
        if ( true || _traits->windowDecoration )
            flags |= Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowSystemMenuHint
#if (QT_VERSION_CHECK(4, 5, 0) <= QT_VERSION)
                | Qt::WindowCloseButtonHint
#endif
                ;

        // create widget
        _window = new GLWindow();
        _window->setFlags(flags);
        _window->setSurfaceType(QSurface::OpenGLSurface);
        _window->setFormat(traits2qSurfaceFormat(_traits.get()));
        _window->create();
    }

    // set widget name and position
    // (do not set it when we inherited the widget)
    if ( _ownsWidget )
    {
        _window->setTitle( _traits->windowName.c_str() );
        _window->setPosition( _traits->x, _traits->y );
        QSize sz(_traits->width, _traits->height);
        if ( !_traits->supportsResize ) {
          _window->setMinimumSize( sz );
          _window->setMaximumSize( sz );
        } else {
          _window->resize( sz );
        }
    }

    // initialize widget properties
//    _window->setMouseTracking( true );
//    _window->setFocusPolicy( Qt::WheelFocus );
    _window->setGraphicsWindow( this );
    useCursor( _traits->useCursor );

    // initialize State
    setState( new osg::State );
    getState()->setGraphicsContext(this);

    // initialize contextID
    if ( _traits.valid() && _traits->sharedContext.valid() )
    {
        getState()->setContextID( _traits->sharedContext->getState()->getContextID() );
        incrementContextIDUsageCount( getState()->getContextID() );
    }
    else
    {
        getState()->setContextID( osg::GraphicsContext::createNewContextID() );
    }

    // make sure the event queue has the correct window rectangle size and input range
    getEventQueue()->syncWindowRectangleWithGraphicsContext();

    return true;
}

QSurfaceFormat GraphicsWindowQt5::traits2qSurfaceFormat( const osg::GraphicsContext::Traits* traits )
{
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);

    format.setAlphaBufferSize( traits->alpha );
    format.setRedBufferSize( traits->red );
    format.setGreenBufferSize( traits->green );
    format.setBlueBufferSize( traits->blue );
    format.setDepthBufferSize( traits->depth );
    format.setStencilBufferSize( traits->stencil );
  //  format.setSampleBuffers( traits->sampleBuffers );
    format.setSamples( traits->samples );

    format.setAlphaBufferSize( traits->alpha>0 );
    format.setDepthBufferSize( traits->depth );

    format.setSwapBehavior( traits->doubleBuffer ?
        QSurfaceFormat::DoubleBuffer :
        QSurfaceFormat::DefaultSwapBehavior);
    format.setSwapInterval( traits->vsync ? 1 : 0 );
    format.setStereo( traits->quadBufferStereo ? 1 : 0 );

    return format;
}

void GraphicsWindowQt5::qSurfaceFormat2traits( const QSurfaceFormat& format, osg::GraphicsContext::Traits* traits )
{
    traits->red = format.redBufferSize();
    traits->green = format.greenBufferSize();
    traits->blue = format.blueBufferSize();
    traits->alpha = format.alphaBufferSize();
    traits->depth = format.depthBufferSize();
    traits->stencil = format.stencilBufferSize();
    traits->samples = format.samples();

    traits->quadBufferStereo = format.stereo();
    traits->doubleBuffer = (format.swapBehavior() == QSurfaceFormat::DoubleBuffer);

    traits->vsync = format.swapInterval() >= 1;
}

osg::GraphicsContext::Traits* GraphicsWindowQt5::createTraits( const QWindow* window )
{
    osg::GraphicsContext::Traits *traits = new osg::GraphicsContext::Traits;

    qSurfaceFormat2traits( window->format(), traits );

    QRect r = window->geometry();
    traits->x = r.x();
    traits->y = r.y();
    traits->width = r.width();
    traits->height = r.height();

    traits->windowName = window->title().toLocal8Bit().data();
    Qt::WindowFlags f = window->flags();
    traits->windowDecoration = ( f & Qt::WindowTitleHint ) &&
                            ( f & Qt::WindowMinMaxButtonsHint ) &&
                            ( f & Qt::WindowSystemMenuHint );

    traits->supportsResize = true;

  /*
    QSizePolicy sp = window->sizePolicy();
    traits->supportsResize = sp.horizontalPolicy() != QSizePolicy::Fixed ||
                            sp.verticalPolicy() != QSizePolicy::Fixed;
*/
    return traits;
}

bool GraphicsWindowQt5::setWindowRectangleImplementation( int x, int y, int width, int height )
{
    if ( _window == NULL )
        return false;

    qDebug() << "setWRI window geometry to " << x << y << width << height;
    _window->setGeometry( x, y, width, height );
    return true;
}

void GraphicsWindowQt5::getWindowRectangle( int& x, int& y, int& width, int& height )
{
    if ( _window )
    {
        const QRect& geom = _window->geometry();
        x = geom.x();
        y = geom.y();
        width = geom.width();
        height = geom.height();
    }
}

bool GraphicsWindowQt5::setWindowDecorationImplementation( bool windowDecoration )
{
    Qt::WindowFlags flags = Qt::Window|Qt::CustomizeWindowHint;//|Qt::WindowStaysOnTopHint;
    if ( windowDecoration )
        flags |= Qt::WindowTitleHint|Qt::WindowMinMaxButtonsHint|Qt::WindowSystemMenuHint;
    _traits->windowDecoration = windowDecoration;

    if ( _window )
    {
        qDebug() << "setWindowDecorationImpl" << windowDecoration << flags;
        _window->setFlags( flags );

        return true;
    }

    return false;
}

bool GraphicsWindowQt5::getWindowDecoration() const
{
    return _traits->windowDecoration;
}

void GraphicsWindowQt5::grabFocus()
{
    if ( _window )
        _window->requestActivate();
}

void GraphicsWindowQt5::grabFocusIfPointerInWindow()
{
  #if 0
    if ( _widget->underMouse() )
        _widget->setFocus( Qt::ActiveWindowFocusReason );
    #endif
}

void GraphicsWindowQt5::raiseWindow()
{
    if ( _window )
        _window->raise();
}

void GraphicsWindowQt5::setWindowName( const std::string& name )
{
    if ( _window )
        _window->setTitle( QString::fromUtf8(name.c_str()) );
}

std::string GraphicsWindowQt5::getWindowName()
{
    return _window ? _window->title().toStdString() : "";
}

void GraphicsWindowQt5::useCursor( bool cursorOn )
{
    if ( _window )
    {
        _traits->useCursor = cursorOn;
        if ( !cursorOn ) _window->setCursor( Qt::BlankCursor );
        else _window->setCursor( _currentCursor );
    }
}

void GraphicsWindowQt5::setCursor( MouseCursor cursor )
{
    if ( cursor==InheritCursor && _window )
    {
        _window->unsetCursor();
    }

    switch ( cursor )
    {
    case NoCursor: _currentCursor = Qt::BlankCursor; break;
    case RightArrowCursor: case LeftArrowCursor: _currentCursor = Qt::ArrowCursor; break;
    case InfoCursor: _currentCursor = Qt::SizeAllCursor; break;
    case DestroyCursor: _currentCursor = Qt::ForbiddenCursor; break;
    case HelpCursor: _currentCursor = Qt::WhatsThisCursor; break;
    case CycleCursor: _currentCursor = Qt::ForbiddenCursor; break;
    case SprayCursor: _currentCursor = Qt::SizeAllCursor; break;
    case WaitCursor: _currentCursor = Qt::WaitCursor; break;
    case TextCursor: _currentCursor = Qt::IBeamCursor; break;
    case CrosshairCursor: _currentCursor = Qt::CrossCursor; break;
    case HandCursor: _currentCursor = Qt::OpenHandCursor; break;
    case UpDownCursor: _currentCursor = Qt::SizeVerCursor; break;
    case LeftRightCursor: _currentCursor = Qt::SizeHorCursor; break;
    case TopSideCursor: case BottomSideCursor: _currentCursor = Qt::UpArrowCursor; break;
    case LeftSideCursor: case RightSideCursor: _currentCursor = Qt::SizeHorCursor; break;
    case TopLeftCorner: _currentCursor = Qt::SizeBDiagCursor; break;
    case TopRightCorner: _currentCursor = Qt::SizeFDiagCursor; break;
    case BottomRightCorner: _currentCursor = Qt::SizeBDiagCursor; break;
    case BottomLeftCorner: _currentCursor = Qt::SizeFDiagCursor; break;
    default: break;
    };
    if ( _window ) _window->setCursor( _currentCursor );
}

bool GraphicsWindowQt5::valid() const
{
    return _window;
}

bool GraphicsWindowQt5::realizeImplementation()
{
    qDebug() << "realizeImplementation" << "on thread" << QThread::currentThread();

    _window->show();

    // initialize GL context for the widget
    // defer this to makeCurrent which happens on the rendering thread

    _realized = true;

    // make sure the event queue has the correct window rectangle size and input range
    getEventQueue()->syncWindowRectangleWithGraphicsContext();
    return true;
}

bool GraphicsWindowQt5::isRealizedImplementation() const
{
    return _realized;
}

void GraphicsWindowQt5::closeImplementation()
{
    if ( _window )
        _window->close();

    delete _context;
    _realized = false;
}

void GraphicsWindowQt5::runOperations()
{
    // While in graphics thread this is last chance to do something useful before
    // graphics thread will execute its operations.
    if (_window->getNumDeferredEvents() > 0)
        _window->processDeferredEvents();

    if (QOpenGLContext::currentContext() != _context)
        makeCurrent();

    GraphicsWindow::runOperations();
}

bool GraphicsWindowQt5::makeCurrentImplementation()
{
    if (_window->getNumDeferredEvents() > 0)
        _window->processDeferredEvents();

    if (!_context) {
      qDebug() << "lazy context creation on thread:" << QThread::currentThread();

      _context = new QOpenGLContext();
      _context->setFormat(_window->format());
      bool result = _context->create();
      if (!result)
      {
        OSG_WARN << "GraphicsWindowQt5::makeCurrentImplementation: Can't create QOpenGLContext'" << std::endl;
        return false;
      }
    }

    _context->makeCurrent(_window);

    return true;
}

bool GraphicsWindowQt5::releaseContextImplementation()
{
    _context->doneCurrent();
    return true;
}

void GraphicsWindowQt5::swapBuffersImplementation()
{
    _context->swapBuffers(_window);
#if 0
    // We need to call makeCurrent here to restore our previously current context
    // which may be changed by the processDeferredEvents function.
    if (QGLContext::currentContext() != _widget->context())
        _widget->makeCurrent();
#endif
}

void GraphicsWindowQt5::requestWarpPointer( float x, float y )
{
    if ( _window )
        QCursor::setPos( _window->mapToGlobal(QPoint((int)x,(int)y)) );
}

bool GraphicsWindowQt5::checkEvents()
{
    if (_window->getNumDeferredEvents() > 0)
      _window->processDeferredEvents();
    QCoreApplication::processEvents(QEventLoop::AllEvents);
    return true;
}

class Qt5WindowingSystem : public osg::GraphicsContext::WindowingSystemInterface
{
public:

    Qt5WindowingSystem()
    {
        OSG_INFO << "QtWindowingSystemInterface()" << std::endl;
    }

    ~Qt5WindowingSystem()
    {
        if (osg::Referenced::getDeleteHandler())
        {
            osg::Referenced::getDeleteHandler()->setNumFramesToRetainObjects(0);
            osg::Referenced::getDeleteHandler()->flushAll();
        }
    }

    // Access the Qt windowing system through this singleton class.
    static Qt5WindowingSystem* getInterface()
    {
        static Qt5WindowingSystem* qtInterface = new Qt5WindowingSystem;
        return qtInterface;
    }

    // Return the number of screens present in the system
    virtual unsigned int getNumScreens( const osg::GraphicsContext::ScreenIdentifier& /*si*/ )
    {
        OSG_WARN << "osgQt: getNumScreens() not implemented yet." << std::endl;
        return 0;
    }

    // Return the resolution of specified screen
    // (0,0) is returned if screen is unknown
    virtual void getScreenSettings( const osg::GraphicsContext::ScreenIdentifier& /*si*/, osg::GraphicsContext::ScreenSettings & /*resolution*/ )
    {
        OSG_WARN << "osgQt: getScreenSettings() not implemented yet." << std::endl;
    }

    // Set the resolution for given screen
    virtual bool setScreenSettings( const osg::GraphicsContext::ScreenIdentifier& /*si*/, const osg::GraphicsContext::ScreenSettings & /*resolution*/ )
    {
        OSG_WARN << "osgQt: setScreenSettings() not implemented yet." << std::endl;
        return false;
    }

    // Enumerates available resolutions
    virtual void enumerateScreenSettings( const osg::GraphicsContext::ScreenIdentifier& /*screenIdentifier*/, osg::GraphicsContext::ScreenSettingsList & /*resolution*/ )
    {
        OSG_WARN << "osgQt: enumerateScreenSettings() not implemented yet." << std::endl;
    }

    // Create a graphics context with given traits
    virtual osg::GraphicsContext* createGraphicsContext( osg::GraphicsContext::Traits* traits )
    {
        if (traits->pbuffer)
        {
            OSG_WARN << "osgQt: createGraphicsContext - pbuffer not implemented yet." << std::endl;
            return NULL;
        }
        else
        {
            osg::ref_ptr< GraphicsWindowQt5 > window = new GraphicsWindowQt5( traits );
            if (window->valid()) {
              return window.release();
            }
            else {
              qDebug() << "window is not valid";
              return NULL;
            }
        }
    }

private:

    // No implementation for these
    Qt5WindowingSystem( const Qt5WindowingSystem& );
    Qt5WindowingSystem& operator=( const Qt5WindowingSystem& );
};


// declare C entry point for static compilation.
extern "C" void OSGQT_EXPORT graphicswindow_Qt5(void)
{
    osg::GraphicsContext::setWindowingSystemInterface(Qt5WindowingSystem::getInterface());
}


void osgQt::initQtWindowingSystem()
{
    graphicswindow_Qt5();
}



void osgQt::setViewer( osgViewer::ViewerBase *viewer )
{
    heartBeat.init( viewer );
}


/// Constructor. Must be called from main thread.
HeartBeat::HeartBeat() : _timerId( 0 )
{
  qDebug() << "creating heart-beat";
}


/// Destructor. Must be called from main thread.
HeartBeat::~HeartBeat()
{
    stopTimer();
}


void HeartBeat::stopTimer()
{
    if ( _timerId != 0 )
    {
        killTimer( _timerId );
        _timerId = 0;
    }
}


/// Initializes the loop for viewer. Must be called from main thread.
void HeartBeat::init( osgViewer::ViewerBase *viewer )
{
  qDebug() << "init heartbeat with viewer";
    if( _viewer == viewer )
        return;

    stopTimer();

    _viewer = viewer;

    if( viewer )
    {
        _timerId = startTimer( 0 );
        _lastFrameStartTime.setStartTick( 0 );
    }
}


void HeartBeat::timerEvent( QTimerEvent */*event*/ )
{
  qDebug() << "heart beating";

    osg::ref_ptr< osgViewer::ViewerBase > viewer;
    if( !_viewer.lock( viewer ) )
    {
        // viewer has been deleted -> stop timer
        stopTimer();
        qDebug() << "viewer is gone, stopping";
        return;
    }

    // limit the frame rate
    if( viewer->getRunMaxFrameRate() > 0.0)
    {
      qDebug() << "capping frame rate";
        double dt = _lastFrameStartTime.time_s();
        double minFrameTime = 1.0 / viewer->getRunMaxFrameRate();
        if (dt < minFrameTime)
            OpenThreads::Thread::microSleep(static_cast<unsigned int>(1000000.0*(minFrameTime-dt)));
    }
    else
    {
        // avoid excessive CPU loading when no frame is required in ON_DEMAND mode
        if( viewer->getRunFrameScheme() == osgViewer::ViewerBase::ON_DEMAND )
        {
          qDebug() << "sleeping for on-demand";
            double dt = _lastFrameStartTime.time_s();
            if (dt < 0.01)
                OpenThreads::Thread::microSleep(static_cast<unsigned int>(1000000.0*(0.01-dt)));
        }

        // record start frame time
        _lastFrameStartTime.setStartTick();

        // make frame
        if( viewer->getRunFrameScheme() == osgViewer::ViewerBase::ON_DEMAND )
        {
            if( viewer->checkNeedToDoFrame() )
            {
                viewer->frame();
            }
        }
        else
        {
          qDebug() << "requesting a frame";
            viewer->frame();
        }
    }
}
