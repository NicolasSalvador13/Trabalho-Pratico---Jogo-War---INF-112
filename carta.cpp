#include "carta.h"

#include <iostream>
#include <string>


// Construtor
Carta::Carta (const std::string& objetivo) :
    _objetivo(objetivo) {}


// Retorna objetivo
std::string Carta::getObjetivo() const {
    return _objetivo;
}


// Atualiza objetivo da carta
void Carta::setObjetivo(const std::string& objetivo) {
    _objetivo = objetivo;
}
