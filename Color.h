#pragma once
#include <istream>

class Color {
  public:
    Color();
    Color(double red, double green, double blue);
    double red() const;
    double green() const;
    double blue() const;

  private:
    double r{};
    double g{};
    double b{};
};

inline std::istream& operator>>(std::istream& in, Color& c) {
    double r, g, b;
    if (in >> r >> g >> b) {
        c = Color(r, g, b);
    }
    return in;
}
