#ifndef STRINGTEST_STRING_H
#define STRINGTEST_STRING_H

#include <algorithm>
#include <cstring>
#include <vector>

class String {

private:
    //длина строки
    size_t m_length = 0;
    //массив символов строки
    char* m_chars = nullptr;
    //выделенная память для подстрок, необходимо для удаления строк после удаления объекта String
    std::vector<char*> memories;
public:

    //Конструктор
    String();
    //Конструктор с параметром
    String(const char* t_chars);
    //Деструктор
    ~String();

    void Concat(const char* t_chars);

    //Получить подстроку
    const char* Substr(const int t_begin, const int t_end);
    //Получить символ по индексу
    char GetChar(const int index) const;
    //Получить длину строки
    const size_t GetLength() const;
    //Получить значение
    const char* GetValue() const;


    //Перегрузка оператора >> для поиска подстроки. По-умолчанию используется алгоритм Кнута-Морриса-Пратта
    int operator>>(const char* t_pattern);

    /*
     * Раздел статических функций, которые могут вызываться без создания класса
     * Они сделаны статическими, потому что являются универсальными
     */
    //Алгоритм Бойера-Мура (BM = Boyer-Moore),
    static const int FindBM(const char *t_chars, const char *t_pattern);

    //Алгоритм Кнута-Морриса-Пратта (KMP)
    static const int FindKMP(const char *t_chars, const char *t_pattern);

    //Алгоритм Префикс-Функции, возвращает длину наибольшего префикса строки
    static std::vector<int> GetPrefixFunction(const char *t_chars);

private:
    //Метод для реверса строки, GetReverse("Sofia") => вернет "aifoS"
    /*
     * Статическая потому, что копирует строку в куче(heap) и возвращает
     * Эта функция приводит к утечке памяти, так как не удаляет реверсивную строку в куче
     * Функция используется в методе FindBM, где реверсивное значение удаляется внутри
     * статического метода FindBM. Так обеспечивается безопасность
     */
    static const char* GetReverse(const char* t_chars);
};


#endif //STRINGTEST_STRING_H
