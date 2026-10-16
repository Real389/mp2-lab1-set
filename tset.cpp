// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField(-1)
{
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(-1)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(-1)
{
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
     if (this != &s)
    {
        BitField = s.BitField;
        MaxPower = s.MaxPower;
    }
    return *this;
}
}

int TSet::operator==(const TSet &s) const // сравнение
{
     return BitField == s.BitField;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return BitField != s.BitField;
}

TSet TSet::operator+(const TSet &s) // объединение
{
TBitField result = BitField | s.BitField;
    return TSet(result);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet result(*this);
    if (Elem >= 0 && Elem < GetMaxPower())
        result.InsElem(Elem);
    return result;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet result(*this);
    if (Elem >= 0 && Elem < GetMaxPower())
        result.DelElem(Elem);
    return result;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TBitField result = BitField & s.BitField;
    return TSet(result);
}

TSet TSet::operator~(void) // дополнение
{
    TBitField result = ~BitField;
    return TSet(result);
}
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    int n;
    istr >> n; // количество элементов для ввода
    for (int i = 0; i < n; i++)
    {
        int elem;
        istr >> elem;
        if (elem >= 0 && elem < s.GetMaxPower())
            s.InsElem(elem);
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    ostr << '{';
    bool first = true;
    for (int i = 0; i < s.MaxPower; ++i)
        if (s.IsMember(i))
        {
            if (!first) ostr << ", ";
            ostr << i;
            first = false;
        }
    return ostr << '}';

}
