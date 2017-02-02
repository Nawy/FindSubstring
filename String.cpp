#include "String.h"
#include <iostream>
#include <map>

//что бы кучу раз не писать std::*
using namespace std;

//Пустой конструктор, что бы можно было сделать String my_string;
String::String() {
}

//Конструктор с одним параметром, что бы вызвать его String my_string("Sofia") или String* my_string = new String("Sofia");
String::String(const char *t_chars) {
    //не дублируем код из Concat(конкатенация), а просто вызываем свою же функцию
    this->Concat(t_chars);
}

//метод получения подстроки, например слово "привет", Substr(2,4) вернет => "иве"
const char *String::Substr(const int t_begin, const int t_end) {
    const size_t substr_length = t_end - t_begin; //получаем размер будущей подстроки

    //проверяем, что размер подстроки меньше или равен размеру строки(логично ведь?) =)
    if (substr_length <= m_length) {
        //выделяем будущую подстроку в куче
        char *sub_string = new char[substr_length];
        //копируем memcpy(КУДА, ОТКУДА, СКОЛЬКО)
        memcpy(sub_string, m_chars + t_begin, substr_length);
        memories.push_back(sub_string);
        return sub_string;
    } else {
        return 0;
    }
}

/*
 * Алгоритм Бойера-Мура базируется на трех основных составляющих
 * 1. Подставляется все слово и поиск осуществляется с последнего символа шаблона
 * 2. Правило стоп-символа
 * 3. Правило совпавшего суффикса
 * Метод возвращает -1 если не нашло ничего и номер позиции если подстрока найдена
 */
const int String::FindBM(const char *t_chars, const char *t_pattern) {
	//получаем длину строки и шаблона
    const size_t chars_length = strlen(t_chars);
    const size_t pattern_length = strlen(t_pattern);

	//если строка меньше чем шаблон, сразу уходим
    if (chars_length < pattern_length) {
        return -1;
    }

	//длина шаблоне не равна нулю
    if (!pattern_length) {
        return chars_length;
    }

    //переопределяем названия просто для удобства взаимодействия с интераторами ниже
    typedef map<char, int> TStopTable;
    typedef map<int, int> TSufficsTable;
    TStopTable stop_table;
    TSufficsTable suffics_table;

	//заполняем таблицу стоп-символами. например для (ABСBAB):
	// CAB  
	// 416
	// Последний символ не учитывается и смещение происходит за стоп-символ
    for (int i = 0; i < pattern_length; ++i) {
        stop_table[t_pattern[i]] = i;
    }

	//создание таблицы суффиксов
	//создаем реверсивный шаблон
    const char* reversed_pattern = String::GetReverse(t_pattern);
	//вызываем префикс-функцию для обоих шаблонов(нормального и реверсивного)
    vector<int> prefix_normal = String::GetPrefixFunction(t_pattern);
    vector<int> prefix_reversed = String::GetPrefixFunction(reversed_pattern);
	//заполнение таблицы суффиксов
    for (int i = 0; i < pattern_length + 1; ++i) {
        suffics_table[i] = pattern_length - prefix_normal.back();
    }
	
    for (int i = 1; i < pattern_length; ++i) {
        int j = prefix_reversed[i];
        suffics_table[j] = min(suffics_table[j], i - prefix_reversed[i] + 1);
    }
	
	//основной цикл поиска подстроки
    for (int shift = 0; shift <= chars_length - pattern_length;) {
        int position = pattern_length - 1;
		
		//shift заполняем в соответствии эвристическим правилам стоп-символа и хорошего суффикса
		// по другому эти правила называют плохой символ => значит хороший суффикс
		
        while (t_pattern[position] == t_chars[position + shift]) {
            if (position == 0) return shift;
            --position;
        }

        if (position == pattern_length - 1) {
            TStopTable::const_iterator stop_symbol = stop_table.find(t_chars[position + shift]);
            int stop_symbol_additional = position - (stop_symbol != stop_table.end() ? stop_symbol->second : -1);
            shift += stop_symbol_additional;
        } else {
            shift += suffics_table[pattern_length - position - 1];
        }
    }
	
	//удаляем реверсивное значение, так как память выделена в куче
    delete[] reversed_pattern;
    return -1;
}

/*
 * Алгоритм Кнута-Морриса-Пратта
 */
const int String::FindKMP(const char *t_chars, const char *t_pattern) {

    const size_t pattern_length = strlen(t_pattern);
    const size_t chars_length = strlen(t_chars);

	//вызываем префикс-функцию
    vector<int> prefixes = String::GetPrefixFunction(t_chars);

    for (int index = 0, i = 0; i < chars_length; ++i){
        while ((index>0) && (t_pattern[index] != t_chars[i]))
            index = prefixes[index-1];

        if (t_pattern[index] == t_chars[i])
            index++;

        if (index==pattern_length)
            return i - pattern_length + 1;//найдено первое совпадение
    }

    return -1;
}

//Префикс функция
//получает длину наибольшего префикса.
vector<int> String::GetPrefixFunction(const char *t_chars) {
    const size_t chars_length = strlen(t_chars);

    std::vector<int> prefixes(chars_length);
    prefixes[0] = 0;

    int index = 0;
    for (int i = 1; i < chars_length; i++) {

        while (index > 0 && (t_chars[index] != t_chars[i])) {
            index = prefixes[index - 1];
        }

        if (t_chars[index] == t_chars[i]) {
            ++index;
        }
        prefixes[i] = index;
    }

    return prefixes;
}

//конкатенация, или соединение двух строк
void String::Concat(const char *t_chars) {
    if (t_chars != nullptr) {
        const size_t old_length = m_length;
        const char *temp = m_chars;

        m_length += strlen(t_chars);
        m_chars = new char[m_length];

        if (temp != nullptr) {
            memcpy(m_chars, temp, old_length);
        }
        memcpy(m_chars + old_length, t_chars, m_length - old_length);
        delete[] temp;
    }
}

//длина строки
const size_t String::GetLength() const {
    return m_length;
}

//получаем значение строки
const char *String::GetValue() const {
    return m_chars;
}

//получаем любой символ
char String::GetChar(const int index) const {
    if (index < m_length && index >= 0) {
        return m_chars[index];
    } else {
        return 0;
    }
}

//переворачиваем слово
const char* String::GetReverse(const char* t_chars) {

    const size_t chars_length = strlen(t_chars);
    char* reverseValue = new char[chars_length];
    int reverseIndex = 0;
    for(int index = chars_length-1; index >= 0; index--) {
        reverseValue[reverseIndex] = t_chars[index];
        reverseIndex++;
    }
    return reverseValue;
}

//перегруженный оператор
//используем KMP - это самый асимптоматически эффективный алгоритм(значит что время работы линейно, чем больше входящих данных тем дольше)
int String::operator>>(const char* t_pattern) {
    return String::FindKMP(m_chars, t_pattern);
}

String::~String() {
    //удаляем строку из памяти
    delete[] m_chars;
    //удаляем выделенные подстроки
    for(char* value : memories) {
        delete[] value;
    }
}

/*
Префикс-функция, приер работы по шагам для abcdabscabcdabia:

'a'!='b' => π=0;
'a'!='c' => π=0;
'a'!='d' => π=0;
'a'=='a' => π=π+1=1;
'b'=='b' => π=π+1=2;
'c'!='s' => π=0;
'a'!='c' => π=0;
'a'=='a' => π=π+1=1;
'b'=='b' => π=π+1=2;
'c'=='c' => π=π+1=3;
'd'=='d' => π=π+1=4;
'a'=='a' => π=π+1=5;
'b'=='b' => π=π+1=6;
's'!='i' => π=0;
'a'=='a' => π=π+1=1;
*/