#include <iostream>
#include <assert.h>
#include "String.h"

using namespace std;

int main() {
    //функция assert выдаст ошибку если выражение внутри != true

    //Пример первый, поиска слова "kolokol" в "kolokprotokolokol"
    assert(String::FindBM("kolokprotokolokol", "kolokol") == 10); //Поиск по алгоритму Бойера-Мура
    assert(String::FindKMP("kolokprotokolokol", "kolokol") == 10); //Поиск по алгоритму Кнута-Морриса-Пратта

    //Пример второй, поиска слова "abbad" в "abeccaabadbabbad"
    assert(String::FindBM("abeccaabadbabbad", "abbad") == 11);
    assert(String::FindKMP("abeccaabadbabbad", "abbad") == 11);

    //Пример третий, поиска слова "kolokol" в "kolokol"
    assert(String::FindBM("kolokol", "kolokol") == 0);
    assert(String::FindKMP("kolokol", "kolokol") == 0);

    //Пример четвертый, поиска слова "kolokol" в "protokolprotokol" = -1(подстрока не найдена)
    assert(String::FindBM("protokolprotokol", "kolokol") == -1);
    assert(String::FindKMP("protokolprotokol", "kolokol") == -1);

    //Создания строки
    String example1("kolokprotokolokol");
    String example2("abeccaabadbabbad");

    //Тестируем перегруженный оператор ">>"
    assert(example1 >> "kolokol" == 10);
    assert(example2 >> "abbad" == 11);

    //Конкатенация
    example1.Concat("abbad");
    example2.Concat("kolokol");

    //Тестируем результаты конкатенации
    assert(example1 >> "abbad" == 17);
    assert(example2 >> "kolokol" == 16);

    cout << "example1 = " << example1.GetValue() << endl;
    cout << "example2 = " << example2.GetValue() << endl;

    cout << "example1(1,3) = " << example1.Substr(1, 3) << endl;
    cout << "example2(2,6) = " << example2.Substr(2, 6) << endl;
    return 0;
}