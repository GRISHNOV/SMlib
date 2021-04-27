# SMlib
State machine prototype

```
gcc -g -O0 -fshort-enums -Wall -Wpedantic SMlib.c SMlibExternalAction.c main.c
./a.out
```

# Project files

- main.c – Основной код проекта, из которого будет использоваться библиотека SMlib.
- SMlib.h – Подключается к коду основного проекта. Содержит весь необходимый интерфейс для взаимодействия с библиотекой.
- SMlibExternalAction.h – Описание функций, которые будут вызываться при смене состояний.
- SMlibDeviceConfig.h – Конфигурационный файл конечного автомата устройства.

# Output

```
START

Current state code is => 0

Format [stateFrom][stateTo] => isEdgeExist
[0][0] => FALSE
[0][1] => TRUE
[0][2] => FALSE
[1][0] => FALSE
[1][1] => TRUE
[1][2] => TRUE
[2][0] => FALSE
[2][1] => TRUE
[2][2] => FALSE


Format [state][lun] => [RW=0, RO=1, HI=2, AO=3]
[0][0] => 1
[0][1] => 2
[0][2] => 2
[0][3] => 2
[1][0] => 1
[1][1] => 0
[1][2] => 1
[1][3] => 3
[2][0] => 0
[2][1] => 0
[2][2] => 0
[2][3] => 0


EXTERNAL FUNCTION: Switching from 0 to 1

SUCCESS: now state is 1

Size 258 bytes

END
```
