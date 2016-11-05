#include "RedeNeural.h"


void Mostra3(vector<vector<vector<double>>> vec3d)
{
    for(uint c=0; c<vec3d.size(); c++)
        for(uint n=0; n<vec3d[c].size(); n++)
        {
            for(uint e=0; e<vec3d[c][n].size(); e++)
            {
                cout << vec3d[c][n][e] << " ";
            }
            cout << endl;
        }
}
void Mostra2(vector<vector<double>> vec2d)
{
    for(uint n=0; n<vec2d.size(); n++)
    {
        for(uint e=0; e<vec2d[n].size(); e++)
        {
            cout << vec2d[n][e] << " ";
        }
        cout << endl;
    }
}
void Mostra1(vector<double> vec)
{
    for(uint e=0; e<vec.size(); e++)
    {
        cout << vec[e] << " ";
    }
}


RedeNeural::RedeNeural(string arquivo)
{
    srand(time(NULL));
    funcALista = {"Logistica", "Tanh", "Linear", "Atan"};
    funcA= {&RedeNeural::FaLogistica, &RedeNeural::FaTanh, &RedeNeural::FaLinear};
    dfuncA= {&RedeNeural::dFaLogistica, &RedeNeural::dFaTanh, &RedeNeural::dFaLinear};

    ifstream abre(arquivo.c_str());
    string txt; // texto qualquer

    abre >> txt >> numCam >> txt >> numEntradas >> txt >> numSaidas >> txt >> numIt >> txt >> pfa >> txt >> afa >> txt >> eta;
    cout << "Numero de camadas : " << numCam << endl
        << "Numero de entradas : " << numEntradas << endl
        << "Numero de saidas : " << numSaidas << endl
		<< "Numero de iteracoes : " << numIt << endl
        << "P funcao ativacao : " << pfa << endl
        << "A funcao ativacao : " << afa << endl
        << "Eta : " << eta << endl;
    Y.resize(numCam+1);
    Y[0].resize(numEntradas+1); // Nao precisa definir
    Y[0][0]= -1; // Nao precisa definir
    Yref.resize(numCam);
    Yref.back().resize(numSaidas); // Nao precisa definir se mudar if
    deltas.resize(numCam);
    pesos.resize(numCam);
    faCamada.resize(numCam);
    for(int c=0; c<numCam; c++)
    {
        short numN, numE, numfaCamada; // numN= n Neuronios por camada, numE= n Entradas por Neuronio na camada, nFa= n da funcao de ativacao
        abre >> txt >> numN >> txt >> numfaCamada;
        numE= (c==0)?(numEntradas+1):(pesos[c-1].size()+1);
        cout << "Numero de neuronios na camada " << c << " : " << numN << endl
             << "Numero de entradas nos neuronios na camada " << c << " : " << numE << endl
             << "Funcao de ativacao na camada " << c << " : " << funcALista[numfaCamada] << endl;
        faCamada[c]= numfaCamada;
        Y[c+1].resize(numN+1);
        Y[c+1][0]= -1;
        if(c<numCam-1) // if a ser mudado
            Yref[c].resize(numN);
        deltas[c].resize(numN);
        pesos[c].resize(numN);
        for(int n=0; n<numN; n++)
            pesos[c][n].resize(numE);
    }
    abre.close();
}
void RedeNeural::CarregaConjuntoEntradaSaida(string arquivo)
{
    entradas.clear();
    saidas.clear();
    ifstream abre(arquivo.c_str());
    string txt;
    abre >> txt;
    while(!abre.eof())
    {
        vector<double> en_aux, sa_aux;
        double aux;
        en_aux.push_back(-1); // BIAS
        for(int i=0; i<numEntradas; i++)
        {
            abre >> aux;
            en_aux.push_back(aux);
        }
        for(int i=0; i<numSaidas; i++)
        {
            abre >> aux;
            sa_aux.push_back(aux);
        }
        entradas.push_back(en_aux);
        saidas.push_back(sa_aux);
    }
    abre.close();
}

void RedeNeural::InicializaPesos()
{
    for(uint c=0; c<pesos.size(); c++)
        for(uint n=0; n<pesos[c].size(); n++)
            for(uint e=0; e<pesos[c][n].size(); e++)
                pesos[c][n][e]= rand()%10/10.0;
}
void RedeNeural::EmbaralhaEntradas()
{
    for(uint i=0; i<entradas.size(); i++)
    {
        int s= rand()%entradas.size();
        swap(entradas[i], entradas[s]);
        swap(saidas[i], saidas[s]);
    }
}
void RedeNeural::CalculaSaidasNeuronios()
{
    for(uint c=0; c<pesos.size(); c++)
        for(uint n=0; n<pesos[c].size(); n++)
        {
            double newY= 0;
            for(uint e=0; e<pesos[c][n].size(); e++)
                newY+= pesos[c][n][e]*Y[c][e];
            Y[c+1][n+1]= (this->*funcA[faCamada[c]])(newY, pfa, afa);
        }
}
void RedeNeural::CalculaDeltas()
{
    for(int c=pesos.size()-1; c>=0; c--)
    {
        for(uint n=0; n<pesos[c].size(); n++)
        {
            if(c == (int)pesos.size()-1)
            {
//                    erroQuadratico += abs(Yref[c][n]-Y[c+1][n+1])/(Yref[c][n]==0?1:Yref[c][n]);
                erroQuadratico += pow(Yref[c][n]-Y[c+1][n+1], 2);
                deltas[c][n]= ((Yref[c][n]-Y[c+1][n+1])*(this->*dfuncA[faCamada[c]])(Y[c+1][n+1], pfa, afa));
            }
            else
                deltas[c][n]= ((Yref[c][n])*(this->*dfuncA[faCamada[c]])(Y[c+1][n+1], pfa, afa));
        }
        if(c>0) // BackPropagation
        {
//                uint n;
            for(uint e=1; e<pesos[c][0].size(); e++)
            {
                Yref[c-1][e-1]= 0;
                for(uint n=0; n<pesos[c].size(); n++)
                    Yref[c-1][e-1]+=deltas[c][n]*pesos[c][n][e];
            }
        }
    }
}
void RedeNeural::CalculaPesos()
{
    for(uint c=0; c<pesos.size(); c++)
        for(uint n=0; n<pesos[c].size(); n++)
            for(uint e=0; e<pesos[c][n].size(); e++)
                pesos[c][n][e]= pesos[c][n][e]+eta*deltas[c][n]*Y[c][e];
}

void RedeNeural::Treina()
{
    InicializaPesos();
    cout << "\nPesos Iniciais\n";
    Mostra3(pesos);
    for(int it=0; it<numIt; it++)
    {
        erroQuadratico= 0;
        EmbaralhaEntradas();
        for(uint inputs=0; inputs<entradas.size(); inputs++)
        {
            Y[0]= entradas[inputs]; // define entradas
            Yref[numCam-1]= saidas[inputs]; // define saidas
            CalculaSaidasNeuronios();
            CalculaDeltas();
            CalculaPesos();
        }
        erroQuadratico=erroQuadratico/(entradas.size()*numSaidas);
//            cout << erroQuadratico << endl;
    }
}
vector<double> RedeNeural::AvaliaEntrada(vector<double> en)
{
    Y[0]= en;
    CalculaSaidasNeuronios();
    return Y.back();
}
void RedeNeural::Mostra()
{
    cout << "\nRede Neural " << endl;
    Mostra3(pesos);
    cout << endl;
    for(uint inputs=0; inputs<entradas.size(); inputs++)
    {
        cout << "Entrada ";
        Mostra1(entradas[inputs]);
        cout << endl;
        Y[0]= entradas[inputs];
        CalculaSaidasNeuronios();
        for(uint z=1; z<Y.back().size(); z++){
            cout << "Saida : " << Y.back()[z]
                 << " Esperado " << saidas[inputs][z-1]
                 << " Erro em % : " << (saidas[inputs][z-1]-Y.back()[z])/(saidas[inputs][z-1]==0?1:saidas[inputs][z-1])*100 << "%" << endl;
        }
    }
    cout << "Erro quadratico : " << erroQuadratico << endl;
}
