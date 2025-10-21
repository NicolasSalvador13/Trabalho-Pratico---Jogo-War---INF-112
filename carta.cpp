#include "carta.h"

#include <iostream>
#include <string>


Carta::Carta (std::string objetivo) :
    _objetivo(objetivo) {}


std::string Carta::getObjetivo() const {
    return _objetivo;
}


void Carta::setObjetivo(std::string objetivo) {
    _objetivo = objetivo;
}