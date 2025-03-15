#pragma once

namespace ymwm::events {
  struct AbstractKeyCode {
    using Type = const unsigned int;
    static Type Unknown{ 0u };

    static Type A;
    static Type B;
    static Type C;
    static Type D;
    static Type E;
    static Type F;
    static Type G;
    static Type H;
    static Type I;
    static Type J;
    static Type K;
    static Type L;
    static Type M;
    static Type N;
    static Type O;
    static Type P;
    static Type Q;
    static Type R;
    static Type S;
    static Type T;
    static Type U;
    static Type V;
    static Type W;
    static Type X;
    static Type Y;
    static Type Z;

    static Type Enter;
    static Type Space;
    static Type Titulus;
    static Type Backspace;
    static Type Period;
    static Type Comma;
    static Type Del;
    static Type Home;
    static Type End;
    static Type PgUp;
    static Type PgDn;
  };
} // namespace ymwm::events
