#include "events/key_event.h"

#include <format>

#include "key.h"
#include "utils/formatter.h"

namespace ufps
{

KeyEvent::KeyEvent(Key key, KeyState state)
    : key_{key}
    , state_{state}
{
}

auto KeyEvent::key() const -> Key
{
    return key_;
}

auto KeyEvent::state() const -> KeyState
{
    return state_;
}

auto KeyEvent::to_string() const -> std::string
{
    return std::format("\u0421\u043e\u0431\u044b\u0442\u0438\u0435\u041a\u043b\u0430\u0432\u0438\u0448\u0438 {} {}", key_, state_);
}

}
