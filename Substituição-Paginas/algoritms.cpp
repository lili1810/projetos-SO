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
int simularOtimo(int totalQuadros, const vector<int>& referencias) {
    vector<int> memoria;
    vector<int> distancias(totalQuadros, 0);//guarda as distancias da pagina 

    int tam = totalQuadros;
    int faltasPagina = 0;   
    int indiceSubstituicao = 0;

    for (int i = 0; i < referencias.size(); i++){
        auto it = find(memoria.begin(), memoria.end(), referencias[i]);
            
        if(it == memoria.end()){//se nao achou a pagina
            faltasPagina++;

            if(memoria.size() < tam){//pagina vazia
                memoria.push_back(referencias[i]);

            }else{//pagina ocupada
                for(int j = 0; j < tam; j++){//percorre a memoria
                    auto ind = find(referencias.begin()+i+1, referencias.end(), memoria[j]);
                    if(ind != referencias.end()){ 
                        distancias[j] = distance(referencias.begin() + i, ind);
                    }else{ 
                        distancias[j] = 999999; //se nao estiver na memoria recebe uma distancia muito grande
                    }
                }
                auto itMaior = max_element(distancias.begin(), distancias.end());
                indiceSubstituicao = distance(distancias.begin(), itMaior);

                memoria[indiceSubstituicao] = referencias[i]; 
            }

        }else{
            continue;//achou na memoria
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
    int faltaOtimo = simularOtimo(totalQuadros, referencias);    

    cout << "FIFO " << faltasFIFO << "\n";
    cout << "Otimo " << faltaOtimo << "\n";

    return 0;
}