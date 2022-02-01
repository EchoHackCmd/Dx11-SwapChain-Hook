#ifndef UTILS_UTILS
#define UTILS_UTILS

#include <Windows.h>
#include <fstream>
#include <chrono>
#include <string>
#include <thread>

#include <MinHook.h>

class Utils {
public:
    static auto logToDebug(std::string) -> void;
    static auto getAppDataDir() -> std::string;
};

template<typename T>
class Vec2 {
public:
    T x = 0;
    T y = 0;

    Vec2(T x = 0, T y = 0) {
        this->x = x;
        this->y = y;
    };
};

template<typename T>
class Vec3 : public Vec2<T> {
public:
    T z = 0;

    Vec3(T x = 0, T y = 0, T z = 0) : Vec2<T>(x, y) {
        this->z = z;
    };
};

template<typename T>
class Vec4 : public Vec3<T> {
public:
    T w = 0;

    Vec4(T x = 0, T y = 0, T z = 0, T w = 0) : Vec3<T>(x, y, z) {
        this->w = w;
    };
};

class Color {
public:
    float r, g, b, a;
public:
    Color(float r = 0, float g = 0, float b = 0, float a = 1) {
        this->r = (r / 255.f);
        this->g = (g / 255.f);
        this->b = (b / 255.f);
        this->a = (a * 255.f);
    };
};

#endif /* UTILS_UTILS */