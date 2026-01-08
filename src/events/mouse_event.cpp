#include "events/mouse_event.h"

#include <format>

namespace ufps
{

MouseEvent::MouseEvent(float delta_x, float delta_y)
    : delta_x_{delta_x}
    , delta_y_{delta_y}
{
}

auto MouseEvent::delta_x() const -> float
{
    return delta_x_;
}

auto MouseEvent::delta_y() const -> float
{
    return delta_y_;
}

auto MouseEvent::to_string() const -> std::string
{
    return std::format("\u0421\u043e\u0431\u044b\u0442\u0438\u0435\u041c\u044b\u0448\u0438 {} {}", delta_x(), delta_y());
}

}
