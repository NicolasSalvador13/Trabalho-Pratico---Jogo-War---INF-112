#ifndef CARTA_H
#define CARTA_H

#include <string>

class Carta {
    private:
        std::string _objetivo;
    public:
        Carta(std::string objetivo);
        std::string getObjetivo() const;
        void setObjetivo(std::string objetivo);
};  

#endif