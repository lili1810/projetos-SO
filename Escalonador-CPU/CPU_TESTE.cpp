#include <bits/stdc++.h>
using namespace std;

#define endl '\n'

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
    Metricas srtf;

    Processo(int _id, int _chegada, int _duracao) {
        id = _id;
        chegada = _chegada;
        duracao = _duracao;
        tempoRestante = _duracao;
    }
};

// Função para desenhar o gráfico de Gantt no terminal
void imprimirGantt(const string& nomeAlgoritmo, const vector<int>& linhaDoTempo) {
    cout << "\n=======================================================\n";
    cout << "  Gráfico de Gantt: " << nomeAlgoritmo << "\n";
    cout << "=======================================================\n\n";
    
    // 1. Linha Superior dos Blocos
    for (size_t i = 0; i < linhaDoTempo.size(); i++) {
        cout << "+----";
    }
    cout << "+\n";

    // 2. Linha do Conteúdo (IDs dos Processos)
    for (int id : linhaDoTempo) {
        cout << "|";
        if (id == 0) {
            cout << " -- "; // CPU Ociosa
        } else {
            // Garante que o "P1", "P2" centralize ou alinhe bem em 4 espaços
            cout << " P" << id;
            if (id < 10) cout << " "; // Espaço extra para IDs de 1 dígito
        }
    }
    cout << "|\n";

    // 3. Linha Inferior dos Blocos
    for (size_t i = 0; i < linhaDoTempo.size(); i++) {
        cout << "+----";
    }
    cout << "+\n";

    // 4. Linha do Tempo (Segundos alinhados com o início de cada bloco)
    // Usamos setw(5) para que cada número ocupe exatamente o espaço de "+----"
    for (size_t t = 0; t <= linhaDoTempo.size(); t++) {
        cout << left << setw(5) << t;
    }
    cout << "\n\n";
}

// ------------------- FCFS -------------------
void FCFS(vector<Processo> p) {
    int tempo = 0;
    double tempoRetorno = 0, tempoResposta = 0, tempoEspera = 0;
    vector<int> linhaDoTempo;

    for(size_t i = 0; i < p.size(); i++){
        // MUDANÇA: CPU Ociosa registrada segundo a segundo
        while (tempo < p[i].chegada) {
            linhaDoTempo.push_back(0); 
            tempo++;
        }
        
        p[i].fcfs.primeiraExec = tempo;
        
        // MUDANÇA: Registro do processo na linha do tempo
        for(int t = 0; t < p[i].duracao; t++) {
            linhaDoTempo.push_back(p[i].id);
        }
        
        tempo += p[i].duracao;
        p[i].fcfs.conclusao = tempo;

        tempoRetorno += p[i].fcfs.conclusao - p[i].chegada;
        tempoResposta += p[i].fcfs.primeiraExec - p[i].chegada;
        tempoEspera += p[i].fcfs.primeiraExec - p[i].chegada;
    }

    cout << fixed << setprecision(1);
    cout << "FCFS " << (tempoRetorno / p.size()) << " " << (tempoResposta / p.size()) << " " << (tempoEspera / p.size()) << endl;
    imprimirGantt("FCFS", linhaDoTempo);
}

// ------------------- SJF (Não-Preemptivo) -------------------
struct ComparaSJF {
    bool operator()(const Processo& a, const Processo& b) {
        if (a.duracao == b.duracao) return a.chegada > b.chegada; 
        return a.duracao > b.duracao; 
    }
};

void SJF(vector<Processo> p) {
    int tempo = 0;
    size_t tamanho = p.size();
    double tempoRetorno = 0, tempoResposta = 0, tempoEspera = 0;
    vector<int> linhaDoTempo;
    priority_queue<Processo, vector<Processo>, ComparaSJF> filaProntos;

    size_t i = 0;
    while(!filaProntos.empty() || i < tamanho){
        // MUDANÇA: CPU Ociosa registrada segundo a segundo
        while (filaProntos.empty() && tempo < p[i].chegada) {
            linhaDoTempo.push_back(0);
            tempo++;
        }

        while (i < tamanho && p[i].chegada <= tempo) {
            filaProntos.push(p[i]);
            i++;
        }

        Processo atual = filaProntos.top();
        filaProntos.pop();

        p[atual.id - 1].sjf.primeiraExec = tempo;

        // MUDANÇA: Registro da execução na linha do tempo
        for(int t = 0; t < atual.duracao; t++) {
            linhaDoTempo.push_back(atual.id);
        }

        tempo += atual.duracao;
        p[atual.id - 1].sjf.conclusao = tempo;

        tempoRetorno += tempo - atual.chegada;
        tempoResposta += p[atual.id - 1].sjf.primeiraExec - atual.chegada;
        tempoEspera += p[atual.id - 1].sjf.primeiraExec - atual.chegada;
    }

    cout << fixed << setprecision(1);
    cout << "SJF " << (tempoRetorno / tamanho) << " " << (tempoResposta / tamanho) << " " << (tempoEspera / tamanho) << endl;
    imprimirGantt("SJF", linhaDoTempo);
}

// ------------------- SRTF (SJF Preemptivo) -------------------
struct ComparaSRTF {
    bool operator()(const Processo& a, const Processo& b) {
        if (a.tempoRestante == b.tempoRestante) return a.chegada > b.chegada; 
        return a.tempoRestante > b.tempoRestante; 
    }
};

void SRTF(vector<Processo> p) {
    int tempo = 0;
    int concluidos = 0;
    size_t tamanho = p.size();
    double tempoRetorno = 0, tempoResposta = 0, tempoEspera = 0;
    vector<int> linhaDoTempo;
    priority_queue<Processo, vector<Processo>, ComparaSRTF> filaProntos;
    
    size_t i = 0;
    vector<int> primeiraExecucao(tamanho + 1, -1);

    while (concluidos < (int)tamanho) {
        while (i < tamanho && p[i].chegada <= tempo) {
            filaProntos.push(p[i]);
            i++;
        }

        if (filaProntos.empty()) {
            linhaDoTempo.push_back(0);
            tempo++;
            continue;
        }

        Processo atual = filaProntos.top();
        filaProntos.pop();

        if (primeiraExecucao[atual.id] == -1) {
            primeiraExecucao[atual.id] = tempo;
            tempoResposta += (tempo - atual.chegada);
        }

        linhaDoTempo.push_back(atual.id);
        atual.tempoRestante--;
        tempo++;

        if (atual.tempoRestante == 0) {
            concluidos++;
            tempoRetorno += (tempo - atual.chegada);
            tempoEspera += (tempo - atual.chegada) - atual.duracao;
        } else {
            filaProntos.push(atual);
        }
    }

    cout << fixed << setprecision(1);
    cout << "SRTF " << (tempoRetorno / tamanho) << " " << (tempoResposta / tamanho) << " " << (tempoEspera / tamanho) << endl;
    imprimirGantt("SRTF (Preemptivo)", linhaDoTempo);
}

// ------------------- ROUND ROBIN (RR) -------------------
void RR(vector<Processo> p) {
    int tempo = 0;
    size_t tamanho = p.size();
    double tempoRetorno = 0, tempoResposta = 0, tempoEspera = 0;
    vector<int> linhaDoTempo;
    queue<Processo> filaProntos;

    size_t i = 0;
    vector<int> primeiraExecucao(tamanho + 1, -1);

    filaProntos.push(p[i]);
    i++;

    while(!filaProntos.empty() || i < tamanho){
        // MUDANÇA: CPU Ociosa tratada de 1 em 1 segundo
        while(filaProntos.empty() && i < tamanho) {
            if (tempo < p[i].chegada) {
                linhaDoTempo.push_back(0);
                tempo++;
            } else {
                filaProntos.push(p[i]);
                i++;
            }
        }

        if(filaProntos.empty()) break;

        Processo atual = filaProntos.front();
        filaProntos.pop();

        if (primeiraExecucao[atual.id] == -1) {
            primeiraExecucao[atual.id] = tempo;
            tempoResposta += (tempo - atual.chegada);
        }
        
        int quantum = 2;
        int tempoExecutado = min(quantum, atual.tempoRestante);

        // MUDANÇA: Substituição do salto direto por processamento segundo a segundo
        for(int t = 0; t < tempoExecutado; t++) {
            linhaDoTempo.push_back(atual.id);
            tempo++;
            atual.tempoRestante--;

            // MUDANÇA CRÍTICA: Novos processos chegam ENQUANTO o quantum está rodando
            while(i < tamanho && p[i].chegada <= tempo){
                filaProntos.push(p[i]);
                i++;
            }
        }

        if(atual.tempoRestante > 0){
            filaProntos.push(atual);
        } else {
            tempoRetorno += (tempo - atual.chegada);
            tempoEspera += (tempo - atual.chegada) - atual.duracao;
        }
    }

    cout << fixed << setprecision(1);
    cout << "RR " << (tempoRetorno / tamanho) << " " << (tempoResposta / tamanho) << " " << (tempoEspera / tamanho) << endl;
    imprimirGantt("Round Robin", linhaDoTempo);
}

int main() {
    vector<Processo> processos = {
        Processo(1, 0, 8),
        Processo(2, 1, 4),
        Processo(3, 2, 9),
        Processo(4, 3, 5)
    };

    // Ordena por tempo de chegada
    sort(processos.begin(), processos.end(), [](const Processo& a, const Processo& b) {
        return a.chegada < b.chegada;
    });

    FCFS(processos);
    SJF(processos);
    SRTF(processos);
    RR(processos);

    return 0;
}