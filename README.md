# μfps / FPSGameEngine

Небольшой шутер от первого лица, написанный на современном C++ и OpenGL с нуля. Всю разработку можно наблюдать в прямом эфире, но можно и просто собрать локально, используя Docker и WSL.

## Быстрый старт

1. Обновите WSL и запустите выбранный дистрибутив (например, Ubuntu):
   ```bash
   wsl --update
   wsl -d Ubuntu
   ```
2. Внутри WSL убедитесь, что есть `make`, `cmake` и `docker` (через интеграцию Docker Desktop):
   ```bash
   sudo apt update
   sudo apt install -y build-essential cmake ninja
   ```
3. Перейдите в проект, соберите образ и настройте его:
   ```bash
   cd /mnt/c/MyGames/FPSGameEngine
   make docker-image    # выполняется один раз на машине
   make config          # генерирует CMake + Ninja
   ```
4. Соберите и запустите игру:
   ```bash
   make run             # сборка Debug и запуск ufps.exe
   ```
5. Для проверки можно отдельно собирать или запускать тесты:
   ```bash
   make build
   make tests
   ```

Если хотите контролировать сборки из PowerShell, используйте `wsl` или `bash` и запускайте те же команды внутри `/mnt/c/MyGames/FPSGameEngine`.

## Структура проекта

- `docker-image` — настраивает контейнер с Mingw-w64 тулчейнами и зависимостями (Imgui/ImGuizmo, OpenGL).  
- `make config` (`make` → `config`) создаёт `build/` и подключает `mingw_toolchain.cmake`.  
- `src/main.cpp` инициализирует окно, красит лог, добавляет два куба и слушает клавиатуру/мышь через `events`.  
- `src/graphics/debug_ui.*` рендерит ImGui/ImGuizmo: выбор сущностей, цвет, gizmo для трансформов, обработка клика.  
- Логирование из `ufps::log` теперь выводит информацию/ошибки на русском языке и пишет системную информацию.

## Управление и взаимодействие

- **W/A/S/D** — движение камеры вперёд, влево, назад, вправо (Q/E поднимаются/опускаются).  
- **Мышь** — вращает камеру по горизонтали/вертикали (когда `debug_mode == false`).  
- **F1** — переключает режим отладки: появляется ImGui-панель (справа от игры), где можно редактировать цвет и использовать ImGuizmo для перемещения, масштабирования и вращения кубов.  
- **ESC** — закрыть игру.  
- Когда `debug_mode` включён, курсор фиксируется на UI и события мыши передаются в ImGui (`DebugUI::add_mouse_event`).

## Запись и логирование

- `utils/system_info.cpp` собирает строку с ОС, видеокартой, драйвером и памятью и выводит её через `ufps::log::info`.  
- `graphics/window.cpp` логирует DPI/размеры окна и данные OpenGL.  
- `graphics/utils.h` пишет про перераспределение GPU-буферов.  
- `debug_ui.cpp` выводит также цвет выбранного пикселя `glReadPixels` (для отладки попаданий по кубам).

## Готово? Что дальше

1. Нажмите `make run` и в окне нажмите `F1`, чтобы открыть ImGui и настроить кубы.  
2. Если нужно, измените или добавьте новые сущности в `src/main.cpp` (функция `cube()` создаёт геометрию).  
3. После изменений `git add . && git commit -m "описание"` и `git push` (напоминаю, для пуша нужны PAT с `repo` и `workflow`, если изменялись `.github/workflows`).  

## Как запускать тесты с пояснениями

Если хочется видеть русские описания наборов тестов прямо перед запуском, используйте скрипт `scripts/run_tests.sh`. Он выводит перевод каждой группы (vector3, matrix4, error и т.д.), а затем вызывает `make tests` так же, как раньше:

```bash
./scripts/run_tests.sh
```

Скрипт работает в том же Docker-окружении и отражает статус всех `unit_tests`. Дополнительно вы можете запускать отдельные фильтры напрямую (`./build/tests/Debug/unit_tests.exe --gtest_filter=vector3.*`), если нужно сузить проверку.

Если появятся вопросы по запуску, настройке Docker/WSL или ImGui — просто напиши, я помогу пройти каждый шаг.

## Описание основных файлов и утилит

- **Движок**  
  - `src/main.cpp` — точка входа: инициализация COM/логгера/окна, загрузка сцены (два куба), обработка `ufps::Event`, движение камеры и рендер с `DebugUI`.  
  - `src/core/camera.*` — камера от первого лица: матрицы вида/проекции, yaw/pitch, управление движением и данные для GPU.  
  - `src/events/*` — события: `Event`-вариант, клавиатура (состояния `KeyState`, перевод в строки) и мышь, переводящие Win32-колбэки в `main`.  
  - `src/graphics/` — рендеринг: буферы OpenGL (`buffer.*`, `persistent_buffer.*`, `multi_buffer.h`), manager-ы мешей/материалов, командный буфер, GLSL-шейдеры, сцена/сущности, окно/DebugUI с ImGui/ImGuizmo.  
  - `src/maths/` — векторы, матрицы, кватернионы и `Transform`↔`Matrix4` преобразования.  
  - `src/utils/` — общие утилиты: RAII (`AutoRelease`, `ComRelease`), `expect/ensure`, форматтер, логирование (консоль/файл), системная информация (WMI), перекодировка строк, `config.h`.

- **Скрипты и сборка**  
  - `scripts/run_tests.sh` — печатает русское описание наборов и вызывает `make tests`.  
  - `Makefile`/`mingw_toolchain.cmake`/`Dockerfile` — строят Docker-образ с Mingw/GCC, конфигурируют CMake+Ninja, собирают/пакуют `ufps`, запускают `unit_tests`.  
  - `.github/workflows/*` — CI: кросс-компиляция, Windows-тесты, bump версии, релизы с Discord-уведомлением.  
  - `.github/actions/discord-notify/action.yaml` — вспомогательный Action, отправляющий уведомления в Discord по webhook.  

- **Тесты**  
  - `tests/CMakeLists.txt` собирает `unit_tests`, которые покрывают:
    * `tests/vector3_tests.cpp`
    * `tests/matrix4_tests.cpp`
    * `tests/multi_buffer_tests.cpp`
    * `tests/formatter_tests.cpp`
    * `tests/error_tests.cpp`
    * `tests/auto_release_tests.cpp`

  Эту секцию можно использовать как справочник, когда нужно понять, за что отвечает каждый модуль и где искать пример его использования.
