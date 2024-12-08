# `sudoku_solver`

**PURPOSE:** Demonstrates **profiling**/usage of atomics/**data generation during compilation**/CRTP on the same task - "sudoku solution"

- generation of arrays during compilations: see [`generated_indexes.h`] and [`generated_indexes.cpp`]
- CRTP: see [`classes_to_test.h`]
- sample of **atomics** usage in multithreading: see [`bs3_threads_atomics.h`]
- profiling for every approach included in CRTP: see [`report_generator.h`] and [`report_generator.cpp`]

## Sequence with threads - atomics
|Step Main Thread|Step other Thread|
|:-|:-|
|prepare all `struct ThreadRelated`. Set pointer to `struct SFIELD`||
|switch `workMode` atomic to **modeDoWork** (for every thread)||
||Wait `workMode` atomic becoming **modeDoWork**|
|Do the work for own range of cells|Do the work for own range of cells|
||switch `workMode` atomic to **modeWorkDone**|
|wait for every thread `workMode` atomic become  **modeWorkDone**||
|Repeat for the new *guess field*||

## Contact

mailto: **Zaytsev.Lesha@gmail.com**

![Donate:USDT:Tron](./usdt_tron_donate.jpg)
## Copyright

[MIT License](https://opensource.org/license/mit/) Copyright <2024> [Alexey Zaytsev](https://www.linkedin.com/in/zaytsevalexey/)



[`generated_indexes.h`]:./srcsolver/generated_indexes.h
[`generated_indexes.cpp`]:./srcsolver/generated_indexes.cpp
[`classes_to_test.h`]:./srcsolver/classes_to_test.h
[`bs3_threads_atomics.h`]:./srcsolver/bs3_threads_atomics.h
[`report_generator.h`]:./srcsolver/report_generator.h
[`report_generator.cpp`]:./srcsolver/report_generator.cpp