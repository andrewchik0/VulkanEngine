#pragma once

#include "../engine/types.h"



namespace VKEngine {
  typedef uint32_t Key;

  enum KeyCodes : Key
  {
#ifdef PLATFORM_WINDOWS

    // From WinUser.h
    MouseLeftButton    = VK_LBUTTON,
    MouseRightButton   = VK_RBUTTON,
    MouseMiddleButton  = VK_MBUTTON,

    Backspace     = VK_BACK,
    Tab           = VK_TAB,
    Clear         = VK_CLEAR,
    Enter         = VK_RETURN,
    Shift         = VK_SHIFT,
    Control       = VK_CONTROL,
    Alt           = VK_MENU,
    Pause         = VK_PAUSE,
    CapsLock      = VK_CAPITAL,
    Escape        = VK_ESCAPE,
    Space         = VK_SPACE,
    PageUp        = VK_PRIOR,
    PageDown      = VK_NEXT,
    End           = VK_END,
    Home          = VK_HOME,
    Left          = VK_LEFT,
    Up            = VK_UP,
    Right         = VK_RIGHT,
    Down          = VK_DOWN,
    PrintScreen   = VK_SNAPSHOT,
    Insert        = VK_INSERT,
    Delete        = VK_DELETE,

    Key0 = 48,
    Key1 = 49,
    Key2 = 50,
    Key3 = 51,
    Key4 = 52,
    Key5 = 53,
    Key6 = 54,
    Key7 = 55,
    Key8 = 56,
    Key9 = 57,

    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,

    Numpad0     = VK_NUMPAD0,
    Numpad1     = VK_NUMPAD1,
    Numpad2     = VK_NUMPAD2,
    Numpad3     = VK_NUMPAD3,
    Numpad4     = VK_NUMPAD4,
    Numpad5     = VK_NUMPAD5,
    Numpad6     = VK_NUMPAD6,
    Numpad7     = VK_NUMPAD7,
    Numpad8     = VK_NUMPAD8,
    Numpad9     = VK_NUMPAD9,
    Multiply    = VK_MULTIPLY,
    Add         = VK_ADD,
    Separator   = VK_SEPARATOR,
    Substract   = VK_SUBTRACT,
    Decimal     = VK_DECIMAL,
    Divide      = VK_DIVIDE,

    F1   = VK_F1,
    F2   = VK_F2,
    F3   = VK_F3,
    F4   = VK_F4,
    F5   = VK_F5,
    F6   = VK_F6,
    F7   = VK_F7,
    F8   = VK_F8,
    F9   = VK_F9,
    F10  = VK_F10,
    F11  = VK_F11,
    F12  = VK_F12,
    F13  = VK_F13,
    F14  = VK_F14,
    F15  = VK_F15,
    F16  = VK_F16,
    F17  = VK_F17,
    F18  = VK_F18,
    F19  = VK_F19,
    F20  = VK_F20,
    F21  = VK_F21,
    F22  = VK_F22,
    F23  = VK_F23,
    F24  = VK_F24,

    LeftShift     = VK_LSHIFT,
    RightShift    = VK_RSHIFT,
    LeftControl   = VK_LCONTROL,
    RightControl  = VK_RCONTROL,
    LeftAlt       = VK_LMENU,
    RightAlt      = VK_RMENU,

#elif defined(USE_SDL)

#endif
  };
}

