#include <bits/stdc++.h>
using namespace std;

#define endl '\n'

typedef long long ll;

typedef struct Processo {
    int id;
    int chegada;
    int duracao;
    int conclusao;
    int primeiraExec = -1;
    int duracaoMenos;

    // Isso aqui é o construtor:
    Processo(int _id, int _chegada, int _duracao) {
        id = _id;
        chegada = _chegada;
        duracao = _duracao;
        duracaoMenos = _duracao;
    }
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
    cout << "FCFS " << mediaRetorno << " " << mediaResposta << " " << mediaEspera << endl;
}

void RR(vector<Processo> p){
    int tempo = 0;
    // vou usar esse tamanho, porque to tirando os processos da lista depois deles serem concluidos
    int tamanho = p.size();

    double tempoRetorno = 0;
    double tempoResposta = 0;
    double tempoEspera = 0;

    queue<Processo> filaProntos;

    int i = 0;
    filaProntos.push(p[i]);
    i++;

    while(!filaProntos.empty()){
        Processo atual = filaProntos.front();
        filaProntos.pop();

        // Executa pelo quantum (2) ou pelo restante da duração
        if(atual.chegada <= tempo && atual.primeiraExec == -1){
            atual.primeiraExec = tempo;
        }
        
        int tempoExecutado = min(2, atual.duracaoMenos);
        tempo += tempoExecutado;
        atual.duracaoMenos -= tempoExecutado;

        // Verifica se novos processos chegaram enquanto o processo atual rodava
        // Se sim, entram na fila ANTES do atual voltar para o fim
        // ele tem prioridade o que chegou primeiro, mesmo que ao mesmo tempo
        while(i < tamanho && p[i].chegada <= tempo){
            filaProntos.push(p[i]);
            i++;
        }

        // Se ainda não terminou, volta para o final da fila
        if(atual.chegada <= tempo && atual.duracaoMenos > 0){
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
    cout << "RR "<< mediaRetorno << " " << mediaResposta << " " << mediaEspera << endl;
}

// função auxiliar de sort
bool comparaPorDuracao(const Processo& a, const Processo& b) {
    return a.duracao < b.duracao;
}

void SJF(vector<Processo> p){
    int tempo = 0;
    int tamanho = p.size();

    double tempoRetorno = 0;
    double tempoResposta = 0;
    double tempoEspera = 0;

    int i = 0;
    // queria ter usado priority queue mas não funcionaria em struct
    vector<Processo> filaProntos;
    for(i = 0; i < tamanho; i++){
        if(p[i].chegada == 0){
            filaProntos.push_back(p[i]);
        }else{
            break;
        }
    }

    while(!filaProntos.empty()){
        // não dá para dar sortttt
        sort(filaProntos.begin(), filaProntos.end(), comparaPorDuracao);
        Processo atual = filaProntos[0];
        filaProntos.erase(filaProntos.begin());

        if(atual.chegada <= tempo && atual.primeiraExec == -1){
            atual.primeiraExec = tempo;
        }

        // não é necessário esse variavel, mas quis por para seguir a função de RR
        int tempoExecutado = atual.duracao;
        tempo += tempoExecutado;
        // depois de executar ele conclui
        atual.conclusao = tempo;

        // Verifica se novos processos chegaram enquanto o processo atual rodava
        // Se sim, entram na fila ANTES do atual voltar para o fim
        // ele tem prioridade o que chegou primeiro, mesmo que ao mesmo tempo
        while(i < tamanho && p[i].chegada <= tempo){
            filaProntos.push_back(p[i]);
            i++;
        }

        tempoRetorno += atual.conclusao - atual.chegada;
        tempoResposta += atual.primeiraExec - atual.chegada;
        // a primeira execução dele é quando ele tbm termina, então o tempo que ele espera é até a primeira execução
        // não tem preenpção
        tempoEspera += atual.primeiraExec - atual.chegada;
    }
    double mediaRetorno = tempoRetorno / tamanho;
    double mediaResposta = tempoResposta / tamanho;
    double mediaEspera = tempoEspera / tamanho; 

    cout << fixed << setprecision(1);
    cout << "SJF " << mediaRetorno << " " << mediaResposta << " " << mediaEspera << endl;
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
    while (arquivo >> tempo >> pico) {
        processos.push_back(Processo(id, tempo, pico));
        id++;
    }

    FCFS(processos);
    SJF(processos);
    RR(processos);

    return 0;
}