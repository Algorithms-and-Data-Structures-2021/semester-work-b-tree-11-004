# B-tree

[![CMake](https://github.com/Algorithms-and-Data-Structures-2021/semester-work-b-tree-11-004/actions/workflows/cmake.yml/badge.svg)](https://github.com/Algorithms-and-Data-Structures-2021/semester-work-template/actions/workflows/cmake.yml)

## Краткое описание семестрового проекта:

- **B-дерево** - это особый тип самобалансирующегося дерева поиска, в котором каждый узел может содержать более одного ключа и
  может иметь более двух дочерних элементов.
- Потребность в **B-дереве** возникла вместе с ростом потребности в меньшем времени доступа к физическим носителям
  информации, таким как жесткий диск. Вторичные запоминающие устройства работают медленнее и имеют большую емкость.
  Существовала потребность в таких типах структур данных, которые **минимизировали бы доступ** к диску.
- Другие структуры данных, такие как **бинарное дерево поиска**, **avl-дерево**, **красно-черное дерево** и т.д., могут хранить
  только один ключ в одном узле. Если нужно хранить большое количество ключей, то высота таких деревьев становится очень
  большой и **время доступа увеличивается**.
- **B-дерево** может хранить **множество** ключей в одном узле и иметь **несколько** дочерних узлов. Это значительно уменьшает
  высоту, позволяя **ускорить** доступ к диску.

**Основные свойства B-tree**:

1. Для каждого узла `x` ключи хранятся в порядке возрастания.
2. В каждом узле есть логическое значение `x.leaf`, которое истинно, если `x` это лист.
3. Если `n` это порядок дерева, то каждый внутренний узел может содержать не более `n - 1` ключей вместе с указателем на
   каждого дочернего.
4. Каждый узел, кроме корня, может иметь не более `n` дочерних элементов и не менее `n/2` дочерних.
5. Все листья имеют одинаковую глубину (то есть высоту - h дерева).
6. Корень имеет как минимум 2 дочерних элемента и содержит как минимум 1 ключ.
7. Если `n ≥ 1`, то для любого n-ключа B-дерева высоты h и минимальной степени `t ≥ 2` ,`h ≥ log<sub>t</sub> (n+1)/2`. 
   
**Основные операции**: `поиск`, `вставка`, `удаление`.

- **Теоретическая сложность операций:**
    - **Операция поиска** выполняется за время O(t log<sub>t</sub> n).
    - **Операция вставки** выполняется за время O(t log<sub>t</sub> n).
    - **Операция удаления** выполняется за время O(t log<sub>t</sub> n).

## Команда "Lost in space"

| Фамилия Имя | Вклад (%) | Прозвище | Роль |
| :---: | :---: | :---: | :---: |
| Шкалин Вячеслав | _90_ | _Человек-full_ | _Код, реализация, бенчмарки, readme_ |
| Гайсова Ольга | _10_ | _nullptr_ | _Презентация_ |

**Девиз команды**
> _Как-нибудь справимся!_

## Структура проекта

Проект состоит из следующих частей:

- [`src`](src)/[`include`](include) - реализация структуры данных (исходный код и заголовочные файлы);
- [`benchmark`](benchmark) - контрольные тесты производительности структуры данных (операции добавления, удаления, поиска);
- [`dataset`](dataset) - наборы данных для запуска контрольных тестов и их генерация;

## Требования

Рекомендуемые требования:

1. С++ компилятор c поддержкой стандарта C++17 (например, _GNU GCC 8.1.x_ и выше).
2. Система автоматизации сборки _CMake_ (версия _3.12.x_ и выше).
3. Интерпретатор _Python_ (версия _3.7.x_ и выше).
4. Рекомендуемый объем оперативной памяти - не менее 6 ГБ.
5. Свободное дисковое пространство объемом ~ 1,5 ГБ (набор данных для контрольных тестов).

## Сборка и запуск

_Инструкция по сборке проекта, генерации тестовых данных, запуска контрольных тестов и примеров работы._

### Пример (Windows)

#### Сборка проекта

Склонируйте проект к себе на устройство через [Git for Windows](https://gitforwindows.org/) (либо используйте
возможности IDE):

```shell
git clone https://github.com/Algorithms-and-Data-Structures-2021/semester-work-b-tree-11-004.git
```

#### Генерация тестовых данных

Генерация тестового набора данных в
формате [comma-seperated values (CSV)](https://en.wikipedia.org/wiki/Comma-separated_values):

Инструкция по запуску скрипта:

| Номер шага                                                                            | Папки и файлы                  |
| :---                                                                                  | :---                           |
| 1) Перейдите в папку генерации набора данных.                                         | `dataset`                      |
| 2) Откройте файл.                                                                     | `generate_csv_dataset.cpp`     |
| 3) Запустите метод.                                                                   | `main()`                       |
| 4) В папке dataset есть папка data, в ней еще 3 папки (remove, find, insert).         | `data`                         |
| 5) После запуска скрипта, в этих папках появятся файлы для контрольного тестирования. | `remove`, `find`, `insert`     |

По названию директории `/dataset/data/insert` можно понять, что здесь хранятся наборы данных для контрольных тестов по
**добавлению** элементов в структуру данных. Названия файлов `100.csv`. `5000000.csv` и т.д. хранят информацию о размере
набора данных (т.е. количество элементов).

#### Контрольные тесты (benchmarks)

Для запуска контрольных тестов необходимо предварительно сгенерировать или скачать готовый набор тестовых данных (в случае скачивания, заменить исходную папку `data` на скачанную).

**Примечание**. Если вы не хотите "захламлять" проект большим объёмом данных, вы можете скачать наборы данных для
контрольного тестирования, перейдя по ссылке
на [Google Drive](https://drive.google.com/drive/folders/1nqzZ9Z37lQ3YWiczp7nUHf3JC0opvjkM?usp=sharing).

##### Список контрольных тестов

| Название           | Описание                                | Метрики |
| :---               | ---                                     | :---    |
| `find_benchmark`   | поиск элементов в структуре             | _время_ |
| `insert_benchmark` | добавление элементов в структуру данных | _время_ |
| `remove_benchmark` | удаление всех элементов из структуры    | _время_ |

##### Инструкция по запуску контрольных тестов:

| Номер шага                                                                                        | Папки и файлы                  |
| :---                                                                                              | :---                           |
| 1) Перейдите в папку с контрольными тестами.                                                      | `benchmark`                    |
| 2) В папке есть 3 файла с контрольными тестами, по названию понятно, какой метод они тестируют. Откройте один из них.   | `remove_benchmark.cpp`, `find_benchmark.cpp`, `insert_benchmark.cpp` |
| 3) Запустите метод                                                                                | `main()`. Из-за большого объема данных, метод выполняется довольно долго, пожалуйста, подождите какое-то время. |
| 4) В папке result есть 3 файла с метриками. После прогона одного из контрольных тестов, файл, который привязан к определенному бенчмарку, отобразит результаты тестирования. | `result`, `eraseResults.csv`, `findResults.csv`, `insertResults.csv` |  

_В файлы записываются как 3 числа через запятую:_

1) Количество данных в наборе (от 100 до 5 млн)
2) Номер прогона на наборе данных (от 1 до 10)
3) Затраченное время (измеряется в наносекундах)

## Источники

* [Википедия (RU)](https://ru.wikipedia.org/wiki/B-дерево)
* [Википедия (EN)](https://en.wikipedia.org/wiki/B-tree)
* [Хабр](https://habr.com/ru/post/114154/)
* [Видео для базового объяснения](https://www.youtube.com/watch?v=WXXetwePSRk)
* [Университет ИТМО Викиконспекты](https://neerc.ifmo.ru/wiki/index.php?title=B-дерево)
* [Англоязычная статья с описанием основных операций](https://www.programiz.com/dsa/b-tree)
