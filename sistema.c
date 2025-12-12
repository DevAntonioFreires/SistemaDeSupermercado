#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>  // sleep()

#define MAX_PRODUTOS 100
#define MAX_CARRINHO 100

typedef struct {
    char codigo[20];
    char nome[50];
    float preco;
} Produto;

typedef struct {
    Produto produto;
    int quantidade;
} ItemCarrinho;

Produto catalogo[MAX_PRODUTOS];
int qtdProdutos = 6;

ItemCarrinho carrinho[MAX_CARRINHO];
int qtdCarrinho = 0;

char CPF[20]; // CPF GLOBAL PARA TODA A COMPRA

int validarCodigo(const char *codigo) {
    for (int i = 0; codigo[i] != '\0'; i++)
        if (!isdigit((unsigned char)codigo[i]))
            return 0;
    return 1;
}

int buscarProdutoCatalogo(const char *codigo) {
    for (int i = 0; i < qtdProdutos; i++)
        if (strcmp(catalogo[i].codigo, codigo) == 0)
            return i;
    return -1;
}

int buscarProdutoCarrinho(const char *codigo) {
    for (int i = 0; i < qtdCarrinho; i++)
        if (strcmp(carrinho[i].produto.codigo, codigo) == 0)
            return i;
    return -1;
}

void listarCatalogo() {
    printf("\n========== BASE DE PRODUTOS ==========\n");
    for (int i = 0; i < qtdProdutos; i++) {
        printf("%d - %s | %s | R$ %.2f\n",
               i + 1,
               catalogo[i].codigo,
               catalogo[i].nome,
               catalogo[i].preco);
    }
    printf("======================================\n");
}

void cadastrarProduto() {
    if (qtdProdutos >= MAX_PRODUTOS) {
        printf("\nBase de dados cheia!\n");
        return;
    }

    Produto p;

    printf("\nDigite o código de barras (somente números): ");
    scanf("%s", p.codigo);

    if (!validarCodigo(p.codigo)) {
        printf("Código inválido!\n");
        return;
    }

    if (buscarProdutoCatalogo(p.codigo) != -1) {
        printf("Produto já existe!\n");
        return;
    }

    printf("Nome do produto: ");
    scanf(" %[^\n]", p.nome);

    printf("Preço: ");
    scanf("%f", &p.preco);

    catalogo[qtdProdutos++] = p;
    printf("Produto cadastrado com sucesso!\n");
}

void listarCarrinho() {
    printf("\n========== CARRINHO ==========\n");
    if (qtdCarrinho == 0) {
        printf("Carrinho vazio.\n");
        return;
    }

    float total = 0;
    for (int i = 0; i < qtdCarrinho; i++) {
        float sub = carrinho[i].produto.preco * carrinho[i].quantidade;
        total += sub;

        printf("%d) %s | %s | R$ %.2f | Qtd: %d | Subtotal: R$ %.2f\n",
               i + 1,
               carrinho[i].produto.codigo,
               carrinho[i].produto.nome,
               carrinho[i].produto.preco,
               carrinho[i].quantidade,
               sub);
    }

    printf("---------------------------------\n");
    printf("TOTAL: R$ %.2f\n", total);
    printf("=================================\n");
}

void adicionarCarrinho() {
    listarCatalogo();

    char codigo[20];
    printf("\nDigite o código do produto: ");
    scanf("%s", codigo);

    if (!validarCodigo(codigo)) {
        printf("Código inválido!\n");
        return;
    }

    int idxCatalogo = buscarProdutoCatalogo(codigo);
    if (idxCatalogo == -1) {
        printf("Produto não encontrado!\n");
        return;
    }

    int qtd;
    printf("Quantidade: ");
    scanf("%d", &qtd);

    if (qtd <= 0) {
        printf("Quantidade inválida!\n");
        return;
    }

    int idxCar = buscarProdutoCarrinho(codigo);
    if (idxCar != -1) {
        carrinho[idxCar].quantidade += qtd;
    } else {
        carrinho[qtdCarrinho].produto = catalogo[idxCatalogo];
        carrinho[qtdCarrinho].quantidade = qtd;
        qtdCarrinho++;
    }

    printf("Produto adicionado ao carrinho!\n");
}

void removerItem() {
    if (qtdCarrinho == 0) {
        printf("Carrinho vazio.\n");
        return;
    }

    listarCarrinho();

    char codigo[20];
    printf("\nDigite o código a remover: ");
    scanf("%s", codigo);

    int pos = buscarProdutoCarrinho(codigo);
    if (pos == -1) {
        printf("Item não encontrado!\n");
        return;
    }

    for (int i = pos; i < qtdCarrinho - 1; i++)
        carrinho[i] = carrinho[i + 1];

    qtdCarrinho--;
    printf("Item removido!\n");
}

void corrigirQuantidade() {
    if (qtdCarrinho == 0) {
        printf("Carrinho vazio.\n");
        return;
    }

    listarCarrinho();

    char codigo[20];
    printf("\nCódigo do produto: ");
    scanf("%s", codigo);

    int pos = buscarProdutoCarrinho(codigo);
    if (pos == -1) {
        printf("Produto não encontrado!\n");
        return;
    }

    int novaQtd;
    printf("Nova quantidade: ");
    scanf("%d", &novaQtd);

    if (novaQtd < 0) {
        printf("Quantidade inválida!\n");
        return;
    }

    if (novaQtd == 0) {
        for (int i = pos; i < qtdCarrinho - 1; i++)
            carrinho[i] = carrinho[i + 1];
        qtdCarrinho--;
        printf("Item removido!\n");
    } else {
        carrinho[pos].quantidade = novaQtd;
        printf("Quantidade atualizada!\n");
    }
}

void gerarNota() {
    printf("\n============= NOTA FISCAL =============\n");

    if (strcmp(CPF, "0") != 0)
        printf("CPF: %s\n", CPF);

    float total = 0;
    for (int i = 0; i < qtdCarrinho; i++) {
        float sub = carrinho[i].produto.preco * carrinho[i].quantidade;
        total += sub;

        printf("%s (x%d) - R$ %.2f\n",
               carrinho[i].produto.nome,
               carrinho[i].quantidade,
               sub);
    }

    printf("\nTOTAL: R$ %.2f\n", total);
    printf("========================================\n");
}

void limparCarrinho() {
    qtdCarrinho = 0;
}

int main() {

INICIO_COMPRA:

    printf("\nDigite o CPF do cliente (ou 0 para não incluir): ");
    scanf("%s", CPF);

    limparCarrinho();

    strcpy(catalogo[0].codigo, "111");
    strcpy(catalogo[0].nome, "Arroz 5kg");
    catalogo[0].preco = 22.90;

    strcpy(catalogo[1].codigo, "222");
    strcpy(catalogo[1].nome, "Feijão 1kg");
    catalogo[1].preco = 7.50;

    strcpy(catalogo[2].codigo, "333");
    strcpy(catalogo[2].nome, "Açúcar 1kg");
    catalogo[2].preco = 4.20;

    strcpy(catalogo[3].codigo, "444");
    strcpy(catalogo[3].nome, "Leite 1L");
    catalogo[3].preco = 5.80;

    strcpy(catalogo[4].codigo, "555");
    strcpy(catalogo[4].nome, "Óleo 900ml");
    catalogo[4].preco = 8.60;

    strcpy(catalogo[5].codigo, "666");
    strcpy(catalogo[5].nome, "Café 500g");
    catalogo[5].preco = 12.40;

    int opc;

MENU:

    do {
        printf("\n=========== CAIXA SUPERMERCADO ===========\n");
        printf("1 - Adicionar produto ao carrinho\n");
        printf("2 - Remover produto do carrinho\n");
        printf("3 - Corrigir quantidade\n");
        printf("4 - Listar itens do carrinho\n");
        printf("5 - Cadastrar novo produto\n");
        printf("6 - Finalizar compra (gerar nota)\n");
        printf("0 - Sair do sistema\n");
        printf("Escolha: ");
        scanf("%d", &opc);

        switch (opc) {
            case 1: adicionarCarrinho(); break;
            case 2: removerItem(); break;
            case 3: corrigirQuantidade(); break;
            case 4: listarCarrinho(); break;
            case 5: cadastrarProduto(); break;

            case 6:
                gerarNota();

PAGAMENTO:

                char resp;
                printf("\nPagamento foi realizado? (S/N): ");
                scanf(" %c", &resp);

                if (toupper(resp) == 'S') {
                    printf("\nPagamento confirmado!\n");
                    goto INICIO_COMPRA;
                } else {
                    printf("\nPagamento NÃO realizado. Tentando novamente em 10 segundos...\n");
                    sleep(10);
                    goto PAGAMENTO;
                }
                break;

            default:
                if (opc != 0) printf("Opção inválida!\n");
        }

    } while (opc != 0);

    return 0;
}
