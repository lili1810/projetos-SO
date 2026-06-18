#include <bits/stdc++.h>
using namespace std;

#define endl '\n'

typedef long long ll;

struct Metricas {
    int conclusao;
    int primeiraExec = -1;
    int retorno;
    int resposta;
    int espera;
};

struct Processo {
    int id;
    int chegada;
    int duracao;    
    int tempoRestante;

    Metricas fcfs;
    Metricas rr;
    Metricas sjf;

    // Isso aqui é o construtor:
    Processo(int _id, int _chegada, int _duracao) {
        id = _id;
        chegada = _chegada;
        duracao = _duracao;
        tempoRestante = _duracao;
    }
};


void FCFS(vector<Processo>& p){
    int tempo = 0;
    double tempoRetorno = 0;
    double tempoResposta = 0;
    double tempoEspera = 0;
    for(int i = 0; i < p.size(); i++){
        if (tempo < p[i].chegada) {
            tempo = p[i].chegada; // A CPU fica ociosa até o processo chegar
        }
        p[i].fcfs.primeiraExec = tempo;
        tempo += p[i].duracao;
        p[i].fcfs.conclusao = tempo;

        p[i].fcfs.retorno = p[i].fcfs.conclusao - p[i].chegada;
        p[i].fcfs.resposta = p[i].fcfs.primeiraExec - p[i].chegada;
        p[i].fcfs.espera = p[i].fcfs.primeiraExec - p[i].chegada;

        tempoRetorno += p[i].fcfs.conclusao - p[i].chegada;

        tempoResposta += p[i].fcfs.primeiraExec - p[i].chegada;
        
        // ele vai rodar até o final, logo é só o tempo de espera da chegada
        tempoEspera += p[i].fcfs.primeiraExec - p[i].chegada;
    }

    double mediaRetorno = tempoRetorno / p.size();
    double mediaResposta = tempoResposta / p.size();
    double mediaEspera = tempoEspera / p.size();
    
    cout << fixed << setprecision(1);
    cout << "FCFS " << mediaRetorno << " " << mediaResposta << " " << mediaEspera << endl;
}

void RR(vector<Processo>& p){
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

    // aprendeu a lidar com o tempo de chegada não sendo constante
    // assim ele precisa saber lidar com a cpu ociosa
    while(!filaProntos.empty() || i < tamanho){
        // Se a fila esvaziou, mas ainda tem processo para chegar, avança o tempo
        if(filaProntos.empty()) {
            tempo = max(tempo, p[i].chegada);
            filaProntos.push(p[i]); // no SJF seria push_back
            i++;
        }

        Processo atual = filaProntos.front();
        filaProntos.pop();

        // Executa pelo quantum (2) ou pelo restante da duração
        if(atual.chegada <= tempo && atual.rr.primeiraExec == -1){
            atual.rr.primeiraExec = tempo;
        }
        
        int tempoExecutado = min(2, atual.tempoRestante);
        tempo += tempoExecutado;
        atual.tempoRestante -= tempoExecutado;

        // Verifica se novos processos chegaram enquanto o processo atual rodava
        // Se sim, entram na fila ANTES do atual voltar para o fim
        // ele tem prioridade o que chegou primeiro, mesmo que ao mesmo tempo
        while(i < tamanho && p[i].chegada <= tempo){
            filaProntos.push(p[i]);
            i++;
        }

        // Se ainda não terminou, volta para o final da fila
        if(atual.chegada <= tempo && atual.tempoRestante > 0){
            filaProntos.push(atual);
        }else{
            atual.rr.conclusao = tempo;

            atual.rr.retorno = atual.rr.conclusao - atual.chegada;
            atual.rr.resposta = atual.rr.primeiraExec - atual.chegada;
            atual.rr.espera = (atual.rr.conclusao - atual.chegada) - atual.duracao;

            tempoRetorno += atual.rr.conclusao - atual.chegada;
            tempoResposta += atual.rr.primeiraExec - atual.chegada;
            // tempo de espera é o tempo de retorno menos o a duração total que ele ficou
            // porque vai ser o tempo que ele passou executando, menos a duração total do retorno dele
            tempoEspera += (atual.rr.conclusao - atual.chegada) - atual.duracao;
        }
    }
    double mediaRetorno = tempoRetorno / tamanho;
    double mediaResposta = tempoResposta / tamanho;
    double mediaEspera = tempoEspera / tamanho; 

    cout << fixed << setprecision(1);
    cout << "RR " << mediaRetorno << " " << mediaResposta << " " << mediaEspera << endl;
}

struct ComparaSJF {
    bool operator()(const Processo& a, const Processo& b) {
        // Se a duração for igual, o que chegou primeiro tem prioridade
        if (a.duracao == b.duracao) {
            return a.chegada > b.chegada; 
        }
        // Caso contrário, o de menor duração fica no topo
        return a.duracao > b.duracao; 
    }
};

void SJF(vector<Processo>& p){
    int tempo = 0;
    int tamanho = p.size();

    double tempoRetorno = 0;
    double tempoResposta = 0;
    double tempoEspera = 0;

    int i = 0;
    // Substituindo o vector pela priority_queue com o nosso comparador
    priority_queue<Processo, vector<Processo>, ComparaSJF> filaProntos;

    while(!filaProntos.empty() || i < tamanho){
        // Se a fila esvaziou, mas ainda faltam processos chegarem no futuro,
        // adiantamos o tempo do sistema para o tempo de chegada do próximo processo.
        if (filaProntos.empty()) {
            tempo = max(tempo, p[i].chegada);
        }

        while (i < tamanho && p[i].chegada <= tempo) {
            filaProntos.push(p[i]);
            i++;
        }

        // Pega o processo com a menor duração (já sai ordenado do topo da heap)
        Processo atual = filaProntos.top();
        filaProntos.pop();

        // Como o SJF não é preemptivo, a primeira execução é o tempo atual
        atual.sjf.primeiraExec = tempo;

        // Executa o processo até o final
        tempo += atual.duracao;
        atual.sjf.conclusao = tempo;

        atual.sjf.retorno = atual.sjf.conclusao - atual.chegada;
        atual.sjf.resposta = atual.sjf.primeiraExec - atual.chegada;
        atual.sjf.espera = atual.sjf.primeiraExec - atual.chegada;

        // Como 'p' veio por referência e os IDs começam em 1, 
        // salvamos o objeto atualizado no índice correto do main.
        p[atual.id - 1] = atual;

        
        tempoRetorno += atual.sjf.conclusao - atual.chegada;
        tempoResposta += atual.sjf.primeiraExec - atual.chegada;
        // a primeira execução dele é quando ele tbm termina, então o tempo que ele espera é até a primeira execução
        // não tem preenpção
        tempoEspera += atual.sjf.primeiraExec - atual.chegada;
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