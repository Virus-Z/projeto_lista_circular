#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar um item da loja
typedef struct Item {
    int id;
    char *name; // Ponteiro para o nome, que será alocado dinamicamente
    float value;
    struct Item *next;
} Item;

// Protótipos das funções
void add_item(Item **last, int id, const char* name, float value);
int rm_item(Item **last, int id);
void buscar_por_nome(Item *last, const char* name);
int contar_itens(Item *last);
void print_lista(Item *last);
void clear_lista(Item **last);

/*A função principal serve como interface para o usuário, exibindo um menu e chamando as funções da lista.*/
int main() {
    Item *last = NULL; // A lista começa vazia. Usei um ponteiro para o último elemento da lista.
    int opcao, id, total;
    char nome_buffer[100];
    float value;

    do {
        printf("\n--- MENU - GERENCIADOR DE ITENS DA LOJA ---\n");
        printf("1. Adicionar Item\n");
        printf("2. Remover Item (por ID)\n");
        printf("3. Buscar Item (por Nome)\n");
        printf("4. Contar Itens\n");
        printf("5. Imprimir Todos os Itens\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        // Limpa o buffer de entrada para evitar problemas com scanf
        while(getchar() != '\n'); 

        switch (opcao) {
            case 1:
                printf("Digite o ID do item: ");
                scanf("%d", &id);
                while(getchar() != '\n');
                printf("Digite o name do item: ");
                fgets(nome_buffer, sizeof(nome_buffer), stdin);
                nome_buffer[strcspn(nome_buffer, "\n")] = 0;
                printf("Digite o value do item: ");
                scanf("%f", &value);
                while(getchar() != '\n');
                add_item(&last, id, nome_buffer, value);
                printf("Item adicionado com sucesso!\n");
                break;
            case 2:
                printf("Digite o ID do item a ser removido: ");
                scanf("%d", &id);
                if (rm_item(&last, id)) {
                    printf("Item removido com sucesso!\n");
                } else {
                    printf("Erro: Item com ID %d nao encontrado.\n", id);
                }
                break;
            case 3:
                printf("Digite o name do item a ser buscado: ");
                fgets(nome_buffer, sizeof(nome_buffer), stdin);
                nome_buffer[strcspn(nome_buffer, "\n")] = 0;
                buscar_por_nome(last, nome_buffer);
                break;
            case 4:
                total = contar_itens(last);
                printf("A lista contem %d item(ns).\n", total);
                break;
            case 5:
                print_lista(last);
                break;
            case 0:
                printf("Saindo do programa...\n");
                clear_lista(&last);
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

//Função para adicionar um item à lista
void add_item(Item **last, int id, const char* name, float value) {
    // 1. Aloca memória para o novo nó (item)
    Item *novo_item = (Item*) malloc(sizeof(Item));
    if (!novo_item) {
        perror("Falha na alocacao de memoria para o item");
        exit(EXIT_FAILURE);
    }

    // 2. Aloca memória para o nome e copia o conteúdo
    novo_item->name = (char*) malloc(strlen(name) + 1); // +1 para o caractere nulo
    if (!novo_item->name) {
        perror("Falha na alocacao de memoria para o name");
        free(novo_item);
        exit(EXIT_FAILURE);
    }
    strcpy(novo_item->name, name);

    // 3. Preenche os outros campos
    novo_item->id = id;
    novo_item->value = value;

    // 4. Insere o item na lista
    if (*last == NULL) { // Se a lista está vazia
        *last = novo_item;
        novo_item->next = novo_item; // O único item aponta para si mesmo
    } else { // Se a lista já tem itens
        novo_item->next = (*last)->next; // O novo item aponta para o antigo primeiro
        (*last)->next = novo_item; // O último item agora aponta para o novo
        *last = novo_item; // O novo item se torna o último
    }
}

//Função para remover um item pelo ID
int rm_item(Item **last, int id) {
    if (*last == NULL) return 0; // Lista vazia

    Item *atual = (*last)->next;
    Item *anterior = *last;

    // Percorre a lista para encontrar o item
    do {
        if (atual->id == id) {
            // Caso 1: O item a ser removido é o único na lista
            if (atual == *last && atual->next == *last) {
                *last = NULL;
            } 
            // Caso 2: O item a ser removido é o último da lista
            else if (atual == *last) {
                anterior->next = atual->next;
                *last = anterior;
            } 
            // Caso 3: Item no meio ou no início da lista
            else {
                anterior->next = atual->next;
            }

            // Libera a memória alocada
            free(atual->name);
            free(atual);
            return 1; // Remoção bem-sucedida
        }
        anterior = atual;
        atual = atual->next;
    } while (atual != (*last)->next);

    return 0; // Item não encontrado
}

//Função para buscar itens pelo nome
void buscar_por_nome(Item *last, const char* name) {
    if (last == NULL) {
        printf("A lista esta vazia. Nao ha itens para buscar.\n");
        return;
    }

    Item *atual = last->next;
    
    int encontrado = 0;

    do {
        if (strcmp(atual->name, name) == 0) {
            printf("----------------------------\n");
            printf("----- Item Encontrado: -----\n");
            printf("  ID: %d\n", atual->id);
            printf("  Nome: %s\n", atual->name);
            printf("  Preco: R$ %.2f\n", atual->value);
            printf("----------------------------\n");
            encontrado = 1;
        }
        atual = atual->next;
    } while (atual != last->next);

    if (!encontrado) {
        printf("Nenhum item com o name '%s' foi encontrado.\n", name);
    }
}

//Função para contar o número de itens na lista
int contar_itens(Item *last) {
    if (last == NULL) {
        return 0;
    }

    int contador = 0;
    Item *atual = last->next; // Começa do primeiro item

    do {
        contador++;
        atual = atual->next;
    } while (atual != last->next); // Para quando der a volta completa

    return contador;
}

//Função para imprimir todos os itens da lista em formato de tabela
void print_lista(Item *last) {
    if (last == NULL) {
        printf("A lista esta vazia.\n");
        return;
    }

    Item *atual = last->next;

    // Usei larguras fixas para criar colunas. O '-' alinha o texto à esquerda.
    printf("\n------------------------------------------------------\n");
    printf("| %-10s | %-25s | %10s |\n", "ID", "Nome do Item", "Preco");
    printf("------------------------------------------------------\n");

    do {
        // Imprime cada item, garantindo o mesmo alinhamento do cabeçalho
        printf("| %-10d | %-25s | R$ %8.2f|\n", atual->id, atual->name, atual->value);
        atual = atual->next;
    } while (atual != last->next);
    printf("------------------------------------------------------\n");
}

//Função para limpar toda a lista e liberar a memória alocada
void clear_lista(Item **last) {
    if (*last == NULL) return;
    Item *atual = (*last)->next;
    Item *proximo_item;
    // Quebra o ciclo para transformar em uma lista linear e facilitar a liberação
    (*last)->next = NULL;  
    while (atual != NULL) {
        proximo_item = atual->next;
        free(atual->name); // Libera o nome
        free(atual);       // Libera o nó
        atual = proximo_item;
    }
    *last = NULL; // Garante que a lista seja considerada vazia
}
