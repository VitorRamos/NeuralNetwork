#include <iostream>
#include "RedeNeural.h"

using namespace std;

int main()
{
    RedeNeural rede("config.txt");
    rede.CarregaConjuntoEntradaSaida("training.txt");
    rede.Treina();
//    rede.CarregaConjuntoEntradaSaida("evaluation.txt");
    rede.Mostra();
	//cin.get();
    return 0;
}
