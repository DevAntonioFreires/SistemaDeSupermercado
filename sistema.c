#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

int validarCodigo(const char *codigo) {
    for (int i = 0; codigo[i] != '\0'; i++) {
        if (!isdigit((unsigned char)codigo[i])) return 0;
    }
    return 1;
}

int buscarProdutoCatalogo(const char *codigo) {
    for (int i = 0; i < qtdProdutos; i++)
        if (strcmp(catalogo[i].codigo, codigo) == 0) return i;
    return -1;
}

int buscarProdutoCarrinho(const char *codigo) {
    for (int i = 0; i < qtdCarrinho; i++)
        if (strcmp(carrinho[i].produto.codigo, codigo) == 0) return i;
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

    listarCatalogo();

    Produto p;
    printf("\nDigite o codigo de barras (somente numeros): ");
    scanf("%s", p.codigo);

    if (!validarCodigo(p.codigo)) {
        printf("Codigo invalido!\n");
        return;
    }

    if (buscarProdutoCatalogo(p.codigo) != -1) {
        printf("Produto ja existe!\n");
        return;
    }

    printf("Nome do produto: ");
    scanf(" %[^\n]", p.nome);

    printf("Preco: ");
    scanf("%f", &p.preco);

    catalogo[qtdProdutos++] = p;
    printf("Produto cadastrado com sucesso!\n");
}

void listarCarrinho() {
    printf("\n========== ITENS NO CARRINHO ==========\n");
    if (qtdCarrinho == 0) {
        printf("Carrinho vazio.\n");
        printf("======================================\n");
        return;
    }

    float total = 0.0f;
    for (int i = 0; i < qtdCarrinho; i++) {
        float subtotal = carrinho[i].produto.preco * carrinho[i].quantidade;
        total += subtotal;
        printf("%d) Codigo: %s | %s | R$ %.2f | qtd: %d | Sub: R$ %.2f\n",
               i + 1,
               carrinho[i].produto.codigo,
               carrinho[i].produto.nome,
               carrinho[i].produto.preco,
               carrinho[i].quantidade,
               subtotal);
    }
    printf("----------------------------------------\n");
    printf("TOTAL: R$ %.2f\n", total);
    printf("========================================\n");
}

void adicionarCarrinho() {
    listarCatalogo();

    char codigo[20];
    printf("\nDigite o codigo de barras: ");
    scanf("%s", codigo);

    if (!validarCodigo(codigo)) {
        printf("Codigo invalido!\n");
        return;
    }

    int idxCat = buscarProdutoCatalogo(codigo);
    if (idxCat == -1) {
        printf("Produto nao encontrado no catalogo!\n");
        return;
    }

    int qtd;
    printf("Quantidade: ");
    scanf("%d", &qtd);
    if (qtd <= 0) {
        printf("Quantidade invalida.\n");
        return;
    }

    int idxCar = buscarProdutoCarrinho(codigo);
    if (idxCar != -1) {
        carrinho[idxCar].quantidade += qtd;
    } else {
        carrinho[qtdCarrinho].produto = catalogo[idxCat];
        carrinho[qtdCarrinho].quantidade = qtd;
        qtdCarrinho++;
    }

    printf("Produto adicionado/atualizado no carrinho!\n");
}

void removerItem() {
    if (qtdCarrinho == 0) {
        printf("\nCarrinho vazio.\n");
        return;
    }

    listarCarrinho();
    char codigo[20];
    printf("\nDigite o codigo do produto a remover: ");
    scanf("%s", codigo);

    int pos = buscarProdutoCarrinho(codigo);
    if (pos == -1) {
        printf("Item nao encontrado no carrinho.\n");
        return;
    }

    for (int j = pos; j < qtdCarrinho - 1; j++) carrinho[j] = carrinho[j + 1];
    qtdCarrinho--;
    printf("Item removido!\n");
}

void corrigirQuantidade() {
    if (qtdCarrinho == 0) {
        printf("\nCarrinho vazio.\n");
        return;
    }

    listarCarrinho();
    char codigo[20];
    printf("\nDigite o codigo do produto para corrigir a quantidade: ");
    scanf("%s", codigo);

    int pos = buscarProdutoCarrinho(codigo);
    if (pos == -1) {
        printf("Produto nao encontrado no carrinho.\n");
        return;
    }

    int novaQtd;
    printf("Quantidade atual: %d\n", carrinho[pos].quantidade);
    printf("Digite a nova quantidade: ");
    scanf("%d", &novaQtd);
    if (novaQtd < 0) {
        printf("Quantidade invalida.\n");
        return;
    } else if (novaQtd == 0) {
        for (int j = pos; j < qtdCarrinho - 1; j++) carrinho[j] = carrinho[j + 1];
        qtdCarrinho--;
        printf("Quantidade zerada -> item removido do carrinho.\n");
    } else {
        carrinho[pos].quantidade = novaQtd;
        printf("Quantidade atualizada!\n");
    }
}

void gerarNota() {
    char cpf[20];
    printf("\nDigite o CPF (ou 0 para nao incluir): ");
    scanf("%s", cpf);

    printf("\n============== NOTA FISCAL ==============\n");
    if (strcmp(cpf, "0") != 0) printf("CPF: %s\n", cpf);

    float total = 0.0f;
    for (int i = 0; i < qtdCarrinho; i++) {
        float subtotal = carrinho[i].produto.preco * carrinho[i].quantidade;
        total += subtotal;
        printf("%s (x%d) - R$ %.2f\n",
               carrinho[i].produto.nome,
               carrinho[i].quantidade,
               subtotal);
    }
    printf("\nTOTAL FINAL: R$ %.2f\n", total);
    printf("=========================================\n");
}

int main() {
    strcpy(catalogo[0].codigo, "111");
    strcpy(catalogo[0].nome, "Arroz 5kg");
    catalogo[0].preco = 22.90;

    strcpy(catalogo[1].codigo, "222");
    strcpy(catalogo[1].nome, "Feijao 1kg");
    catalogo[1].preco = 7.50;

    strcpy(catalogo[2].codigo, "333");
    strcpy(catalogo[2].nome, "Acucar 1kg");
    catalogo[2].preco = 4.20;

    strcpy(catalogo[3].codigo, "444");
    strcpy(catalogo[3].nome, "Leite 1L");
    catalogo[3].preco = 5.80;

    strcpy(catalogo[4].codigo, "555");
    strcpy(catalogo[4].nome, "Oleo 900ml");
    catalogo[4].preco = 8.60;

    strcpy(catalogo[5].codigo, "666");
    strcpy(catalogo[5].nome, "Cafe 500g");
    catalogo[5].preco = 12.40;

    int opc;
    do {
        printf("\n=========== CAIXA SUPERMERCADO ===========\n");
        printf("1 - Adicionar produto ao carrinho\n");
        printf("2 - Remover produto do carrinho\n");
        printf("3 - Corrigir quantidade\n");
        printf("4 - Listar itens do carrinho\n");
        printf("5 - Cadastrar novo produto na base de dados\n");
        printf("6 - Finalizar compra (gerar nota)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opc);

        switch (opc) {
            case 1: adicionarCarrinho(); break;
            case 2: removerItem(); break;
            case 3: corrigirQuantidade(); break;
            case 4: listarCarrinho(); break;
            case 5: cadastrarProduto(); break;
            case 6: gerarNota(); break;
            default:
                if (opc != 0) printf("Opcao invalida.\n");
        }
    } while (opc != 0);

    return 0;
}
