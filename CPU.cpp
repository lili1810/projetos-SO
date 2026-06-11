#include <bits/stdc++.h>
using namespace std;

#define endl '\n'

typedef long long ll;

typedef struct Processo{
    int id;
    int chegada;
    int duracao;
    int conclusao;
    int primeiraExec = -1;
    // foi criado para simplificar na contagem do RR
    int duracaoMenos;
}Processo;


void FCFS(vector<Processo> p){
    int tempo = 0;
    double tempoRetorno = 0;
    double tempoResposta = 0;
    double tempoEspera = 0;
    for(int i = 0; i < p.size(); i++){
        p[i].primeiraExec = tempo;
        tempo += p[i].duracao;
        p[i].conclusao = tempo;

        tempoRetorno += p[i].conclusao - p[i].chegada;

        tempoResposta += p[i].primeiraExec - p[i].chegada;
        
        // ele vai rodar até o final, logo é só o tempo de espera da chegada
        tempoEspera += p[i].primeiraExec - p[i].chegada;
    }

    double mediaRetorno = tempoRetorno / p.size();
    double mediaResposta = tempoResposta / p.size();
    double mediaEspera = tempoEspera / p.size();
    
    cout << fixed << setprecision(1);
    cout << mediaRetorno << " " << mediaResposta << " " << mediaEspera << endl;
}

void RR(vector<Processo> p){
    int tempo = 0;
    // vou usar esse tamanho, porque to tirando os processos da lista depois deles serem concluidos
    int tamanho = p.size();

    double tempoRetorno = 0;
    double tempoResposta = 0;
    double tempoEspera = 0;

    queue<Processo> filaProntos;

    // Copia os dados e guarda a duração original
    for(int i = 0; i < tamanho; i++){
        p[i].duracaoMenos = p[i].duracao;
        filaProntos.push(p[i]);
    }

    while(!filaProntos.empty()){
        Processo atual = filaProntos.front();
        filaProntos.pop();

        // Executa pelo quantum (2) ou pelo restante da duração
        if(atual.primeiraExec == -1){
            atual.primeiraExec = tempo;
        }
        
        int tempoExecutado = min(2, atual.duracaoMenos);
        tempo += tempoExecutado;
        atual.duracaoMenos -= tempoExecutado;

        // Se ainda não terminou, volta para o final da fila
        if(atual.duracaoMenos > 0){
            filaProntos.push(atual);
        }else{
            atual.conclusao = tempo;

            tempoRetorno += atual.conclusao - atual.chegada;
            tempoResposta += atual.primeiraExec - atual.chegada;
            // tempo de espera é o tempo de retorno menos o a duração total que ele ficou
            // porque vai ser o tempo que ele passou executando, menos a duração total do retorno dele
            tempoEspera += (atual.conclusao - atual.chegada) - atual.duracao;
        }
    }
    double mediaRetorno = tempoRetorno / tamanho;
    double mediaResposta = tempoResposta / tamanho;
    double mediaEspera = tempoEspera / tamanho; 

    cout << fixed << setprecision(1);
    cout << mediaRetorno << " " << mediaResposta << " " << mediaEspera << endl;
}

int main() {

    ifstream arquivo("dados.txt");

    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo!" << endl;
        return 1;
    }

    vector<Processo> processos;

    int tempo, pico;
    int id = 1;
    while (arquivo >> tempo >> pico){
        processos.push_back({id, tempo, pico});
        id++;
    }

    FCFS(processos);
    RR(processos);




    return 0;
}