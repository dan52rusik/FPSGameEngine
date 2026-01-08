#include "events/mouse_button_event.h"

#include <format>

#include "utils/formatter.h"

namespace ufps
{

MouseButtonEvent::MouseButtonEvent(float x, float y, MouseButtonState state)
    : x_{x}
    , y_{y}
    , state_{state}
{
}

auto MouseButtonEvent::x() const -> float
{
    return x_;
}

auto MouseButtonEvent::y() const -> float
{
    return y_;
}

auto MouseButtonEvent::state() const -> MouseButtonState
{
    return state_;
}

auto MouseButtonEvent::to_string() const -> std::string
{
    return std::format(
        "\u0421\u043e\u0431\u044b\u0442\u0438\u0435\u041a\u043d\u043e\u043f\u043a\u0438\u041c\u044b\u0448\u0438 {} {} {}",
        state(),
        x(),
        y());
}

auto to_string(MouseButtonState obj) -> std::string
{
    switch (obj)
    {
        using enum ufps::MouseButtonState;
        case UP: return "\u041e\u0422\u041f\u0423\u0429\u0415\u041d\u0410";
        case DOWN: return "\u041d\u0410\u0416\u0410\u0422\u0410";
        default: return "\u041d\u0415\u0418\u0417\u0412\u0415\u0421\u0422\u041d\u041e";
    }
}

}
