#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
ROOT_DIR=$(cd "${SCRIPT_DIR}/.." && pwd)

cd "${ROOT_DIR}"

cat <<'EOF'
Запуск unit-тестов (через Docker/Mingw) — ниже краткая расшифровка наборов:

- vector3.*      — проверка конструктора, арифметики и форматирования векторов.
- matrix4.*      — проверки матриц (создание, умножение, look-at, перспективы).
- multi_frame.*  — запись в мульти-буферный объект (разные режимы).
- formatter.*    — тесты строковых утилит, чтобы форматирование оставалось стабильным.
- error.*        — проверки макросов `expect/ensure`, включая трассировки.
- auto_release.* — уверены, что `AutoRelease` корректно освобождает ресурсы.

В любой момент можно переустановить сборку параллельно: сначала `make build`, потом `./build/tests/Debug/unit_tests.exe --gtest_filter=...`.
EOF

make tests
