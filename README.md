# Хэш Таблицы

Проект исследования и оптимизаций хэш таблиц.

## Оглавление

- [Хэш Таблицы](#хэш-таблицы)
  - [Оглавление](#оглавление)
  - [Описание](#описание)
  - [Первая часть](#первая-часть)
    - [Функции](#функции)
    - [Гистограммы](#гистограммы)
    - [Однородность](#однородность)
  - [Вторая часть](#вторая-часть)
    - [Оптимизации](#оптимизации)
  - [Итоги и выводы](#итоги-и-выводы)

## Описание

В этой работе я исследовал различные хэш-функции на однородность и исследовал влияние разных оптимизаций на хэш таблицы. В своем исследовании я использовал простую реализацию хэш-таблицы с двусвязными списками, которые отвечают за обработку коллизий.

В первой части я брал различные функции и исследовал их на однородное распределение. База данных состоит из слов произведения: [Mikhail Bulgakov. The Master and Margarita](https://web.archive.org/web/20110606093139/http://lib.ru/BULGAKOW/master97_engl.txt).

```
int               hash_table_ctor   (hash_table *, size_t, size_t (*hash_function)(ht_key_t, ht_value_t));
int               hash_table_add    (hash_table *, ht_key_t, ht_value_t);
int               hash_table_dtor   (hash_table *);
TYPE_ELEMENT_LIST hash_table_search (hash_table *, ht_key_t, ht_value_t);
```

Во второй части я работал с профилировщиком и оптимизировал скорость работы хэш таблиц с помощью встройки, выравниваний, ассемблерных вставок и SIMD инструкций.

## Первая часть
Первым объектом нашего исследования будут хэш-функции. Я начну с простых (наивных) хэш-функций, а затем перейду к более сложным.

### Функции:

1. Хэш возвращает 0.
2. Хэш возвращает ascii код 1 буквы слова.
3. Хэш возвращает длину слова.
4. Хэш возвращает сумму ascii кодов слова.
5. Хэщ возвращает (сумму букв)/(длину слова).
6. ROL хэш. `hash[i] = ROL(hash[i-1]) xor str[i]`
7. ROR хэш. `hash[i] = ROR(hash[i-1]) xor str[i]`
8. [Elf Хэш](https://en.wikipedia.org/wiki/PJW_hash_function)
9. [CRC32 Хэш](https://ru.wikibooks.org/wiki/Реализации_алгоритмов/Циклический_избыточный_код).

Для изучения распределения я строил гистограммы `КоличествоКоллизий(ЗначениеХэша)`. Диаграммы показывают распределение числа коллизий по значениям хэш-функции, в некоторых из них есть пики, которые влияют на скорость работы хэш таблицы(линейный поиску по списку).

Также размер хэш таблицы фиксирован и является простым числом.

Теория по хэш таблицам бралась из [Википедии](https://en.wikipedia.org/wiki/Hash_function)

### Гистограммы

<p> <strong>

В 1-4 представлены простые хэш-функции, которые не используются в реальных проектах. У них нет одинакового размера, они рассматриваются в учебных целях.

В 5-9 представлены сложные хэш-функции, которые применяются в разработке. Их я сравнивал отдельно с одинаковым размером таблицы

1. <U> 0 Hash </U>: Размер: 7, макс. коллизия: 11550.
<img src="https://github.com/Andrelays/hash_table/blob/main/pictures/output1.png?raw=true" width = 100%>

2. <U> First Letter Hash </U>: Размер: 127, макс. коллизия: 1334.
<img src="https://github.com/Andrelays/hash_table/blob/main/pictures/output2.png?raw=true" width = 100%>

3. <U> Length Word Hash </U>: Размер 31, макс. коллизия: 1908.
<img src="https://github.com/Andrelays/hash_table/blob/main/pictures/output3.png?raw=true" width = 100%>

4. <U> Sum of letters Hash </U>: Размер 1501, макс. коллизия: 48.
<img src="https://github.com/Andrelays/hash_table/blob/main/pictures/output4.png?raw=true" width = 100%>

5. <U> (Sum of letters)/Length Hash </U>: Размер 179, макс. коллизия: 1615.
<img src="https://github.com/Andrelays/hash_table/blob/main/pictures/output5.png?raw=true" width = 100%>

6. <U> ROR Hash </U>: Размер 6007, макс. коллизия: 20.
<img src="https://github.com/Andrelays/hash_table/blob/main/pictures/output6.png?raw=true" width = 100%>

7. <U> ROL Hash </U>: Размер 6007, макс. коллизия: 11.
<img src="https://github.com/Andrelays/hash_table/blob/main/pictures/output7.png?raw=true" width = 100%>

8. <U> Elf Hash </U>: Размер 6007, макс. коллизия: 8.
<img src="https://github.com/Andrelays/hash_table/blob/main/pictures/output8.png?raw=true" width = 100%>

9.  <U> CRC32 Hash </U>: Размер 6007, макс. коллизия: 9.
<img src="https://github.com/Andrelays/hash_table/blob/main/pictures/output9.png?raw=true" width = 100%>

</strong></p>

### Однородность

Для численного описания хэш-функций я буду использовать дисперсию:

$$\text{Var(X)} = \frac{1}{n}\sum_{i=1}^n(x_i - \mu)^2, $$
где $\mu$ - среднее значение:

$$
\mu = \frac{1}{n} \sum_{i = 1}^n x_i
$$

Идеальная хэш-функция распределяет слова равномерно, так что дисперсия приближается к нулю.

#### Таблица дисперсий:


|Название хэш-функции               |Var(x) - величина дисперсии    |
|:----------------------------------|:-----------------------------:|
|0 Hash                             |  1.633 * 10^8               |
|First Letter Hash                  |  4.033 * 10^4               |
|Length Word Hash                   |  3.647 * 10^5               |
|Sum of letters Hash                |  9.636 * 10^1               |
|(Sum of letters)/Length Hash       |  6.491 * 10^4               |
|ROR Hash                           |  7.019 * 10^0               |
|ROL Hash                           |  2.523 * 10^0               |
|Elf Hash                           |  1.917 * 10^0               |
|CRC32 Hash                         |  1.861 * 10^0               |

Лучшие функции в однородности стали CRC32, ElfHash и ROL Hash.

## Вторая часть

*Система:*

- Linux Mint 21.3 Cinnamon
- AMD Ryzen 9 7845HX with Radeon Graphics × 12
- CPU Temperature: 55-65 $^\circ C$, нет троттлинга

Я создал стресс тест: загружал в таблицу текст "Мастера и Маргариты" и с помощью функции `stress_test()` искал каждое слово из книги 256 раз.

Далее с помощью профилировщика я нашел узкие места, которые замедляли скорость работы таблицы и оптимизировал их.

В качестве профилировщика использовался Perf и визуализировал с помощью HotSpot.

**Анализирование профилировщика**:

Далее я проводил тестирования, используя Хэш: CRC 32 и таблицу размером 6007 ячеек с флагами `g++ -fno-omit-frame-pointer -g -O3 -msse4.1 -msse4.2 -mavx2 -mavx`.

Я не оптимизировал функции инициализации `hash_table_init_item()` и `hash_table_ctor() / hash_table_dtor()`, потому что они специфичны и работают с файлами.

Я измерял время всей программы через функцию `__rdtsc()`, включая создание хэш таблицы, загрузка слов и стресс тест. Поэтому итоговые данные показывают изменение скорости работы программы в целом.

Первое контрольное время работы программы: $1.62 \cdot 10^10$ тиков.

### Оптимизации

Перед каждой новой оптимизацией я буду прикладывать скриншоты из программы Hotspot, отсортированные по убыванию времени, затраченного каждой функцией.
<img src="https://github.com/Andrelays/hash_table/blob/main/pictures/hotspot1.png?raw=true" width = 100%>

1. **Оптимизация хэш-функции:**

Как видно из программы профилировщика - функция хэширования занимает 90% работы программы, т.к. алгоритм функции CRC 32, которую я использовал довольно плох, поэтому я решил начать оптимизацию с неё.

Я заменил её на встроенную функцию SSE `_mm_crc32_u8 (crc, char)`.

```cpp
size_t crc32_hash_sse(const char* str, size_t length)
{
    uint_least32_t crc = 0;

    for (size_t i = 0; i < length; i++)
    {
        crc = _mm_crc32_u8 (crc, (unsigned char)str[i]);
    };

    return crc ^ 0xFFFFFFFFUL;
}
```

После этого я ещё раз провёл тестирование по времени. В этот раз время работы программы составило: $1.22 \cdot 10^9$ тиков.

<img src="https://github.com/Andrelays/hash_table/blob/main/pictures/hotspot2.png?raw=true" width = 100%>

2. **STRCMP Оптимизация:**

Cамый долгий процесс это `strcmp()`. Оптимизируем её.

В моих данных самое длинное слово размером 30 букв, поэтому я использовал 32 байтные вектора `__m256`. Давайте выделим 32 байт для каждого слова. Таким образом, сложность алгоритма для сравнения двух слов равна O(1).

```cpp
static bool check_equality_key_avx(TYPE_ELEMENT_LIST value_1, TYPE_ELEMENT_LIST value_2)
{
  __m256i str_1 = _mm256_load_si256((const __m256i *) (value_1));
  __m256i str_2 = _mm256_load_si256((const __m256i *) (value_2));

  return (_mm256_testz_si256(str_1, str_2));
}
```

> `_mm_load_si256()` требует выравнивания адреса по 32 байта, иначе это приведет к ошибке сегментации. Поэтому я также выравнил адресса по 32 байта используя функцию `void *aligned_alloc( size_t alignment, size_t size )`

Новое время - $7.25 \cdot 10^8$ тиков.

<img src="https://github.com/Andrelays/hash_table/blob/main/pictures/hotspot3.png?raw=true" width = 100%>

3. **Inline Оптимизация:**

Следующей функцией, которую я оптимизировал была `TYPE_ELEMENT_LIST hash_table_search(hash_table *table, ht_key_t key, ht_value_t value)`, внутри этой функции вызывалась функция поиска в списке `ssize_t find_elem_by_value(list *list_pointer, TYPE_ELEMENT_LIST value)`. На это тратиться время, несмотря на то что блок кода мал. Поэтому я поместил функции поиска по списку и сравнение в файл hash_table.cpp и использовал inline. Получилось, что `inline __attribute__ ((always_inline)) ssize_t find_elem_by_value(list *list_pointer, TYPE_ELEMENT_LIST value)` ускорил программу до $4.92 \cdot 10^8$ тиков.

<img src="https://github.com/Andrelays/hash_table/blob/main/pictures/hotspot4.png?raw=true" width = 100%>

А  `inline __attribute__((always_inline)) bool check_equality_key_avx(TYPE_ELEMENT_LIST value_1, TYPE_ELEMENT_LIST value_2)` до $3.93 \cdot 10^8$ тиков.

<img src="https://github.com/Andrelays/hash_table/blob/main/pictures/hotspot5.png?raw=true" width = 100%>

## Итоги и выводы

После всех оптимизаций (если не считать первую, т.к. алгоритм который я использовал для CRC 32 изначально был плох), то я ускорил работу программы примерно в 3,1 раза.

*Таблица результатов*:

|Оптимизация                              |Тики ($10^8$)      |Ускорение(в сравнении с началом) |
|:----------------------------------------|:-----------------:|:-------------------------------:|
|Векторизация CRC32                       | 12.2              |1.68x                            |
|Выравнивание и векторизация `strcmp()`   | 7.25              |1.20x                            |
|Inline поиска                            | 4.92              |1.47x                            |
|Inline `strcmp()`                        | 3.93              |1.25x                            |

В этом проекте я исследовал хэш-функции, работал с профилировщиком(Perf & HotSpot), ищя узкие места в работе программы и устранял их с помощью оптимизаций таких как: встройка, выравнивание, ассемблерные вставки.

