// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
   if (len < 0)
        throw std::invalid_argument("Length must be non-negative");
    BitLen = len;
    MemLen = (len + ELEM_BITS - 1) / ELEM_BITS; // округление вверх
    if (MemLen == 0)
        MemLen = 1; // чтобы был хотя бы один элемент
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i<MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
    pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if ((0<= n) and (n <= BitLen-1)) {
        int index = n / (8*sizeof(TELEM));
        return index;
    }
    throw std::out_of_range("Индекс выходит за пределы поля");
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
     if ((0<= n) and (n <= BitLen-1)) {
        int j = n % (8*sizeof(TELEM));
        return ((TELEM)1 << j);
    }
    throw std::out_of_range("Индекс выходит за пределы поля");
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("Bit index out of range");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
  if (n < 0 || n >= BitLen)
        throw std::out_of_range("Bit index out of range");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  if ((0<= n) and (n <= BitLen-1)) {
        TELEM mask = GetMemMask(n);
        int val = GetMemIndex(n);
        if ((pMem[val] & mask) == 0)
            return 0;
        return 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf)
        return *this;
    if (MemLen != bf.MemLen)
    {
        delete[] pMem;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
    }
    BitLen = bf.BitLen;
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return 0;
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(maxLen);
    int minMem = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    for (int i = 0; i < minMem; i++)
        result.pMem[i] = pMem[i] | bf.pMem[i];
    // копируем оставшиеся элементы из более длинного операнда
    if (MemLen > bf.MemLen)
        for (int i = minMem; i < MemLen; i++)
            result.pMem[i] = pMem[i];
    else
        for (int i = minMem; i < bf.MemLen; i++)
            result.pMem[i] = bf.pMem[i];
    return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(maxLen);
    for (int i = 0; i < result.MemLen; i++)
    {
        TELEM a = (i < MemLen) ? pMem[i] : 0;
        TELEM b = (i < bf.MemLen) ? bf.pMem[i] : 0;
        result.pMem[i] = a & b;
    }
    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; i++)
        result.pMem[i] = ~pMem[i];
    // обнуляем "лишние" биты в последнем элементе
    int tail = BitLen % ELEM_BITS;
    if (tail != 0)
        result.pMem[MemLen - 1] &= ((TELEM)1 << tail) - 1;
    return result;
}
// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    TBitField result(bf.BitLen);
    for (int i = 0; i < bf.BitLen; ++i)
    {
        char ch;
        if (!(istr >> ch)) return istr;
        if (ch != '0' && ch != '1')
        {
            istr.setstate(ios::failbit);
            return istr;
        }
        if (ch == '1') result.SetBit(i);
    }
    bf = result;
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; ++i) ostr << bf.GetBit(i);
    return ostr;
}
