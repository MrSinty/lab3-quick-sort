#include <iostream>
#include <utility>
#include <iterator>
#include <random>
#include <fstream>
#include "pch.h"
#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"

template <typename T>
void swap(T& a, T& b)
{
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}

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

template <typename T, typename Compare>
T* medianOfThree(T* first, T* middle, T* last, Compare comp) {
    if (comp(*middle, *first)) swap(*middle, *first);
    if (comp(*last, *first)) swap(*last, *first);
    if (comp(*last, *middle)) swap(*last, *middle);
    return middle;
}

template <typename T, typename Compare>
void quickSort(T* first, T* last, Compare comp) {

    while (std::distance(first, last) > 0) {
        T* middle = first + (last - first) / 2;
        T* pivot = medianOfThree(first, middle, last - 1, comp);

        swap(*pivot, *(last - 1));

        T* left = first;
        T* right = last - 2;
        while (true) {
            while (comp(*left, *(last - 1))) ++left;
            while (comp(*(last - 1), *right)) --right;
            if (left >= right) break;
            swap(*left, *right);
            ++left;
            --right;
        }
        swap(*left, *(last - 1));

        if (std::distance(first, left) < std::distance(left + 1, last)) {
            quickSort(first, left, comp);
            first = left + 1;
        }
        else {
            quickSort(left + 1, last, comp);
            last = left;
        }
    }
}

template <typename T, typename Compare>
void sort(T* first, T* last, Compare comp, int threshold) {
    const int INSERTION_SORT_THRESHOLD = threshold;

    while (std::distance(first, last) > INSERTION_SORT_THRESHOLD) {
        T* middle = first + (last - first) / 2;
        T* pivot = medianOfThree(first, middle, last - 1, comp);

        swap(*pivot, *(last - 1));

        T* left = first;
        T* right = last - 2;
        while (true) {
            while (comp(*left, *(last - 1))) ++left;
            while (comp(*(last - 1), *right)) --right;
            if (left >= right) break;
            swap(*left, *right);
            ++left;
            --right;
        }
        swap(*left, *(last - 1));

        if (std::distance(first, left) < std::distance(left + 1, last)) {
            sort(first, left, comp);
            first = left + 1;
        }
        else {
            sort(left + 1, last, comp);
            last = left;
        }
    }

    insertionSort(first, last, comp);
}

int a[50000];
int b[50000];

int main(int argc, char* argv)
{
    srand(time(0));

    //for (int i = 0; i < 100; ++i) {
    //    std::cout << a[i] << " ";
    //}
    //std::cout << "\n";

    std::ofstream fout1("quicksort.json");
    std::ofstream fout2("insertsort.json");

    const int benchIterations[] = {2, 4, 8, 16, 32, 64, 128, 256, 1000, 1500, 2500, 5000, 10000, 50000};
    int j = 0;
    for each (const int iterations in benchIterations) {
    
        for (int i = 0; i < iterations; ++i) {
            a[i] = rand() % 1000;
            b[i] = rand() % 1000;
        }

        std::string sortname1 = "qsort" + std::to_string(j);
        std::string sortname2 = "isort" + std::to_string(j);

        std::ofstream fout1(sortname1 + ".json");
        std::ofstream fout2(sortname2 + ".json");

        ankerl::nanobench::Bench()
            .epochIterations(1000)
            .run("simple qsort, size: " + std::to_string(iterations),
                [&]() {
                    quickSort(a, a + iterations, [](int a, int b) { return a < b; });
                })
            .render(ankerl::nanobench::templates::pyperf(), fout1);

        ankerl::nanobench::Bench()
            .epochIterations(1000)
            .run("insertion sort, size: " + std::to_string(iterations),
                [&]() {
                    insertionSort(b, b + iterations, [](int a, int b) { return a < b; });
                })
            .render(ankerl::nanobench::templates::pyperf(), fout2);

        std::cout << "\n";
        ++j;
    }

    //for (int i = 0; i < 100; ++i) {
    //    std::cout << b[i] << " ";
    //}
    return 0;
}