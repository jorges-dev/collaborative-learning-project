#include <stdio.h>
#include <string.h>

int main () {
char nome [50];
int idade;
int opcao;

// menu bem visual é importante para orientar e não deixar dúvidas no usuário
printf("=======================================\n");
printf("     SISTEMA DE CADASTRO DE USUÁRIO    \n");
printf("                 ________\n");
printf("                | |____| |\n");
printf("                |   __   |\n");
printf("                |  (__)  |\n");
printf("                |        |\n");
printf("                |________|\n");

printf("=======================================\n");

printf("Escolha uma opção: \n");
printf("1 - Realizar cadastro\n");
printf("2 - Sair\n"); // sempre ter opção de sair para não deixar o usuário preso no sistema
printf("Digite sua opção: ");
scanf("%d", &opcao);
getchar(); // limpa o enter do buffer

if (opcao == 2){
  printf("Encerrando o sistema. Até logo!");
  return 0;
  }
if (opcao != 1){
  printf("\nOpção invalida. Reinicie o programa e tente novamente.\n");
  return 1;
  }
printf("\n--- Cadastro ---\n"); // design diferente do primeiro menu, da sensação de avanço no processo

printf("Digite seu nome: "); // palavras do mundo real (nada de usuário, user, etc)
fgets(nome, sizeof(nome), stdin); // pega uma string, guarda na váriavel até o tamanho definido no int main

// remove o \n do final, se existir
nome[strcspn(nome, "\n")] = '\0'; //

printf("Digite sua idade: ");
scanf("%d", &idade);

if (strlen(nome) == 0){
  printf("\nErro: o nome não pode ficar vazio.\n");
  return 1;
}

if (idade < 0 || idade > 120) {
  return 1;
}

printf("\nCadastro realizado com sucesso!\n");
printf("Nome: %s\n", nome);
printf("Idade: %d anos\n", idade);

printf("\nObrigado por usar o sistema.\n");
return 0;

}
