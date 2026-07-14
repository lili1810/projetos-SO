#include <bits/stdc++.h>

using namespace std;

#define endl '\n'

typedef long long ll;

int simularFIFO(int totalQuadros, const vector<int>& referencias) {
    vector<int> memoria;
    int faltasPagina = 0;
    int indiceSubstituicao = 0;

    for (int pagina : referencias) {
        auto it = find(memoria.begin(), memoria.end(), pagina);
        
        if (it == memoria.end()) {
            faltasPagina++;

            if (memoria.size() < totalQuadros) {
                memoria.push_back(pagina);
            } else {
                memoria[indiceSubstituicao] = pagina;
                indiceSubstituicao = (indiceSubstituicao + 1) % totalQuadros;
            }
        }
    }

    return faltasPagina;
}

int main() {
    ifstream arquivo("dados.txt");

    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo!" << endl;
        return 1;
    }

    int totalQuadros;
    arquivo >> totalQuadros;

    vector<int> referencias;
    int pagina;

    while (arquivo >> pagina) {
        referencias.push_back(pagina);
    }

    int faltasFIFO = simularFIFO(totalQuadros, referencias);

    cout << "FIFO " << faltasFIFO << "\n";

    return 0;
}