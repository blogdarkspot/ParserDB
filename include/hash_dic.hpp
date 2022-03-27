#ifndef __HASH_DIC__
#define __HASH_DIC__

#include <iostream>
#include <string>

#define UTF_8_SIZE 1000
#define MAX_WORD_SIZE 40

/**
A ideia é fazer uma matriz onde é garantido que a operaçao

que dado um conjunto  C  tal que C pertence aos numeros naturais, tem que ser
garantido que dado um conjunto D que pode conter N elementos dos que pertencem a C
e podendo ocorrer repeticoes, a soma de todas as permuntacoes tem que ser diferentes.

exemplo

dado o conjunto C com os elementos { a, b, c, d, e, f}

podemos montar uma matriz da seguinte maneira para as permuntaçoes do conjunto D
     1  2  3  4  5  6  7
a   x       x
b       x                   x
c               x
d                   x
e                       x
f

temos a seguinte sequencia  abacdef que a soma desses valores

a + b + a + c + d + e + f
*/
struct HASH_DIC
{
    wchar_t _table[UTF_8_SIZE][MAX_WORD_SIZE];

    void print_info()
    {
        std::cout << sizeof(wchar_t) << std::endl;
    }
};

#endif