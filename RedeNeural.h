
#pragma once

#include <iostream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <fstream>
#include <math.h>
#include <algorithm>

using namespace std;

typedef unsigned int uint;

void Mostra3(vector<vector<vector<double>>> vec3d);
void Mostra2(vector<vector<double>> vec2d);
void Mostra1(vector<double> vec);

class RedeNeural
{
    vector<vector<vector<double>>> pesos;
    vector<vector<double>> entradas, saidas, Y, Yref, deltas;
    vector<double(RedeNeural::*)(double, double, double)> funcA, dfuncA;
    vector<double> faCamada;
    vector<string> funcALista;
    double pfa= 1, afa= 1, eta= 0.1, erroQuadratico;
    short numSaidas, numEntradas, numCam, numIt;

    double FaLogistica(double x, double p, double a){ return a*1.0/(1+exp(-p*x)); }
    double dFaLogistica(double y, double p, double a) { return a*p*y*(1-y); }
    double FaTanh(double x, double p, double a) { return a*tanh(p*x); }
    double dFaTanh(double y, double p, double a) { return a*p*(1-y*y); }
    double FaLinear(double x, double p, double a) { return a*p*x; }
    double dFaLinear(double y, double p, double a) { return a*p; }
public:
    RedeNeural(string arquivo);
    void CarregaConjuntoEntradaSaida(string arquivo);
private:
    void InicializaPesos();
    void EmbaralhaEntradas();
    void CalculaSaidasNeuronios();
    void CalculaDeltas();
    void CalculaPesos();
public:
    void Treina();
    vector<double> AvaliaEntrada(vector<double> en);
    void Mostra();
};
