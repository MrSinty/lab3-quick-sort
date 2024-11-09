#include <iostream>
#include <utility>
#include <iterator>
#include <random>
#include <fstream>
#include "pch.h"
#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"

// Функция для сортировки вставками
template <typename T, typename Compare>
void insertionSort(T* first, T* last, Compare comp) {
    for (T* i = first + 1; i < last; ++i) {
        T temp = std::move(*i);
        T* j = i;
        while (j > first && comp(temp, *(j - 1))) {
            *j = std::move(*(j - 1));
            --j;
        }
        *j = std::move(temp);
    }
}

// Функция для выбора опорного элемента (медиана из первого, среднего и последнего элементов)
template <typename T, typename Compare>
T* medianOfThree(T* first, T* middle, T* last, Compare comp) {
    if (comp(*middle, *first)) std::swap(*middle, *first);
    if (comp(*last, *first)) std::swap(*last, *first);
    if (comp(*last, *middle)) std::swap(*last, *middle);
    return middle;
}

// Основная функция быстрой сортировки с оптимизациями
template <typename T, typename Compare>
void sort(T* first, T* last, Compare comp) {
    const int INSERTION_SORT_THRESHOLD = 10; // Экспериментальное значение для вставки

    while (std::distance(first, last) > INSERTION_SORT_THRESHOLD) {
        T* middle = first + (last - first) / 2;
        T* pivot = medianOfThree(first, middle, last - 1, comp);

        std::swap(*pivot, *(last - 1)); // Перемещение опорного элемента в конец

        // Разбиение массива
        T* left = first;
        T* right = last - 2;
        while (true) {
            while (comp(*left, *(last - 1))) ++left;
            while (comp(*(last - 1), *right)) --right;
            if (left >= right) break;
            std::swap(*left, *right);
            ++left;
            --right;
        }
        std::swap(*left, *(last - 1)); // Возвращение опорного элемента на место

        // Рекурсивная сортировка меньшего интервала
        if (std::distance(first, left) < std::distance(left + 1, last)) {
            sort(first, left, comp);
            first = left + 1;
        }
        else {
            sort(left + 1, last, comp);
            last = left;
        }
    }

    // Завершение сортировки вставками на небольших интервалах
    insertionSort(first, last, comp);
}

int main(int argc, char* argv)
{
    srand(time(0));

    const int N = 1000;

    int a[N];
    int b[N];
    // Заполнение массива случайными значениями для примера
    for (int i = 0; i < N; ++i) {
        a[i] = rand() % 10000;
        b[i] = rand() % 10000;
    }

    //for (int i = 0; i < 100; ++i) {
    //    std::cout << a[i] << " ";
    //}
    //std::cout << "\n";

    std::ofstream fout1("pyperf_qtest.json");

    ankerl::nanobench::Bench()
        .minEpochIterations(10000)
        .run("qsort", 
            [&]() {
                sort(a, a + N, [](int a, int b) { return a < b; });
            })
        .render(ankerl::nanobench::templates::pyperf(), fout1);

    std::cout << "\n";
    std::ofstream fout2("pyperf_itest.json");

    ankerl::nanobench::Bench()
        .minEpochIterations(10000)
        .run("isort",
            [&]() {
                insertionSort(b, b + N, [](int a, int b) { return a < b; });
            })
        .render(ankerl::nanobench::templates::pyperf(), fout2);

     //Вывод отсортированного массива
    //for (int i = 0; i < 100; ++i) {
    //    std::cout << b[i] << " ";
    //}
    return 0;
}