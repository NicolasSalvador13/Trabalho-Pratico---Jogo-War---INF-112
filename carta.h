#ifndef CARTA_H
#define CARTA_H

#include <string>

class Carta {
    private:
        std::string _objetivo;
    public:
        Carta(const std::string& objetivo);
        std::string getObjetivo() const;
        void setObjetivo(const std::string& objetivo);
};  

#endif