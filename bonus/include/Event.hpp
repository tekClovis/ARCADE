/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** Event.hpp
*/

#pragma once

namespace Arc {
    enum Key {
        None,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        Underscore,
        Dash,
        Num1,
        Num2,
        Num3,
        Num4,
        Num5,
        Num6,
        Num7,
        Num8,
        Num9,
        Num0,
        Tab,
        Left,
        Right,
        Up,
        Down,
        Space,
        Enter,
        Escape,
        MouseLeft,
        MouseRight,
        Backspace,
    };

    enum Type {
        KeyPressed,
        KeyReleased,
        Quit
    };

    struct MousePressed
    {
        int x;
        int y;
    };
    
    struct Event {
        Type type;
        Key code;
        MousePressed mouse;
    };
}
