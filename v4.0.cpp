#include <iostream>
#include <stdio.h>
#include <string>

// Estrutura para armazenar os dados dos participantes
struct Participante {
    int id;
    std::string nome;
    int semestre;
    int anoIngresso;
    std::string curso; // "DSM", "SI" ou "GE"
    Participante* proximo; // Ponteiro para o próximo participante
};

// Estrutura para armazenar os dados das contribuições
struct Contribuicao {
    int idParticipante;
    int mes;
    int ano;
    double valor;
    Contribuicao* proximo; // Ponteiro para a próxima contribuição
};

// Ponteiros para o início das listas encadeadas
Participante* participantes = NULL;
Contribuicao* contribuicoes = NULL;

// Função para mostrar o menu principal
void mostrarMenu() {
    std::cout << "1. Inserir Participante\n";
    std::cout << "2. Editar Participante\n";
    std::cout << "3. Carregar Participantes de Arquivo\n";
    std::cout << "4. Salvar Participantes em Arquivo\n";
    std::cout << "5. Cadastrar Contribuicao\n";
    std::cout << "6. Salvar Contribuicoes em Arquivo\n";
    std::cout << "7. Salvar Contribuicoes por Curso\n";
    std::cout << "8. Sair\n";
    std::cout << "Escolha uma opcao: ";
}

// Função para gerar o próximo ID disponível para um novo participante
int gerarProximoId() {
    int maxId = 0;
    Participante* temp = participantes;
    while (temp != NULL) {
        if (temp->id > maxId) {
            maxId = temp->id;
        }
        temp = temp->proximo;
    }
    return maxId + 1;
}

// Função para inserir um novo participante
void inserirParticipante() {
    Participante* novoParticipante = new Participante;
    novoParticipante->id = gerarProximoId();

    std::cout << "Nome: ";
    std::cin >> novoParticipante->nome;
    std::cout << "Semestre: ";
    std::cin >> novoParticipante->semestre;
    std::cout << "Ano de Ingresso: ";
    std::cin >> novoParticipante->anoIngresso;
    std::cout << "Curso (DSM/SI/GE): ";
    std::cin >> novoParticipante->curso;

    novoParticipante->proximo = participantes;
    participantes = novoParticipante;
}

// Função para mostrar todos os IDs dos participantes
void mostrarIdsParticipantes() {
    std::cout << "IDs dos Participantes:\n";
    Participante* temp = participantes;
    while (temp != NULL) {
        std::cout << temp->id << ": " << temp->nome << "\n";
        temp = temp->proximo;
    }
}

// Função para editar os dados de um participante
void editarParticipante() {
    mostrarIdsParticipantes();

    int id;
    std::cout << "ID do Participante: ";
    std::cin >> id;

    Participante* temp = participantes;
    while (temp != NULL && temp->id != id) {
        temp = temp->proximo;
    }

    if (temp == NULL) {
        std::cout << "Participante não encontrado.\n";
    } else {
        std::cout << "Nome: ";
        std::cin >> temp->nome;
        std::cout << "Semestre: ";
        std::cin >> temp->semestre;
        std::cout << "Ano de Ingresso: ";
        std::cin >> temp->anoIngresso;
        std::cout << "Curso (DSM/SI/GE): ";
        std::cin >> temp->curso;
    }
}

// Função para carregar os participantes de um arquivo
void carregarParticipantes() {
    FILE* file = fopen("participantes.txt", "r");
    if (!file) {
        std::cout << "Erro ao abrir o arquivo.\n";
        return;
    }

    while (!feof(file)) {
        Participante* novoParticipante = new Participante;
        char nome[100], curso[100];

        if (fscanf(file, "%d %s %d %d %s", &novoParticipante->id, nome, &novoParticipante->semestre, &novoParticipante->anoIngresso, curso) == 5) {
            novoParticipante->nome = nome;
            novoParticipante->curso = curso;
            novoParticipante->proximo = participantes;
            participantes = novoParticipante;
        } else {
            delete novoParticipante;
        }
    }
    fclose(file);
}

// Função para salvar os participantes em um arquivo
void salvarParticipantes() {
    FILE* file = fopen("participantes.txt", "w");
    if (!file) {
        std::cout << "Erro ao abrir o arquivo.\n";
        return;
    }

    Participante* temp = participantes;
    while (temp != NULL) {
        fprintf(file, "%d %s %d %d %s\n", temp->id, temp->nome.c_str(), temp->semestre, temp->anoIngresso, temp->curso.c_str());
        temp = temp->proximo;
    }
    fclose(file);
}

// Função para cadastrar uma nova contribuição
void cadastrarContribuicao() {
    int id;
    std::cout << "ID do Participante: ";
    std::cin >> id;

    Participante* tempP = participantes;
    while (tempP != NULL && tempP->id != id) {
        tempP = tempP->proximo;
    }

    if (tempP == NULL) {
        std::cout << "Participante não encontrado.\n";
        return;
    }

    Contribuicao* novaContribuicao = new Contribuicao;
    novaContribuicao->idParticipante = id;
    std::cout << "Mes: ";
    std::cin >> novaContribuicao->mes;
    std::cout << "Ano: ";
    std::cin >> novaContribuicao->ano;
    std::cout << "Valor: ";
    std::cin >> novaContribuicao->valor;

    novaContribuicao->proximo = contribuicoes;
    contribuicoes = novaContribuicao;
}

// Função para salvar as contribuições em um arquivo
void salvarContribuicoes() {
    FILE* file = fopen("contribuintes.txt", "w");
    if (!file) {
        std::cout << "Erro ao abrir o arquivo.\n";
        return;
    }

    Contribuicao* temp = contribuicoes;
    while (temp != NULL) {
        fprintf(file, "%d %d %d %lf\n", temp->idParticipante, temp->mes, temp->ano, temp->valor);
        temp = temp->proximo;
    }
    fclose(file);
}

// Função para salvar as contribuições separadas por curso em arquivos diferentes
void salvarContribuicoesPorCurso() {
    FILE* dsmFile = fopen("contribuintes_DSM.txt", "w");
    FILE* siFile = fopen("contribuintes_SI.txt", "w");
    FILE* geFile = fopen("contribuintes_GE.txt", "w");

    if (!dsmFile || !siFile || !geFile) {
        std::cout << "Erro ao abrir um dos arquivos.\n";
        if (dsmFile) fclose(dsmFile);
        if (siFile) fclose(siFile);
        if (geFile) fclose(geFile);
        return;
    }

    Contribuicao* tempC = contribuicoes;
    while (tempC != NULL) {
        Participante* tempP = participantes;
        while (tempP != NULL) {
            if (tempP->id == tempC->idParticipante) {
                FILE* file;
                if (tempP->curso == "DSM") {
                    file = dsmFile;
                } else if (tempP->curso == "SI") {
                    file = siFile;
                } else if (tempP->curso == "GE") {
                    file = geFile;
                }
                fprintf(file, "%d %d %d %lf\n", tempC->idParticipante, tempC->mes, tempC->ano, tempC->valor);
                break;
            }
            tempP = tempP->proximo;
        }
        tempC = tempC->proximo;
    }

    fclose(dsmFile);
    fclose(siFile);
    fclose(geFile);
}

// Função principal
int main() {
    int opcao;
    do {
        mostrarMenu();
        std::cin >> opcao;

        switch(opcao) {
            case 1:
                inserirParticipante();
                break;
            case 2:
                editarParticipante();
                break;
            case 3:
                carregarParticipantes();
                break;
            case 4:
                salvarParticipantes();
                break;
            case 5:
                cadastrarContribuicao();
                break;
            case 6:
                salvarContribuicoes();
                break;
            case 7:
                salvarContribuicoesPorCurso();
                break;
            case 8:
                std::cout << "Saindo...\n";
                break;
            default:
                std::cout << "Opcao invalida. Tente novamente.\n";
                break;
        }
    } while(opcao != 8);

    // Liberar memória alocada
    while (participantes != NULL) {
        Participante* temp = participantes;
        participantes = participantes->proximo;
        delete temp;
    }

    while (contribuicoes != NULL) {
        Contribuicao* temp = contribuicoes;
        contribuicoes = contribuicoes->proximo;
        delete temp;
    }
    
    return 0;
}
