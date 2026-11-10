/* bankAccount.h simulates a simple bank account
 *
 * Joel Adams, Calvin College, Fall 2013.
 */

typedef struct {
  int id;
  double saldo;
  pthread_mutex_t lock;
  FILE *extrato;
} Conta;

Conta contas[5];

void init_contas(){ 
  for (int i = 0; i < 5; i++){
    char nome[30];
    sprintf(nome, "extrato_conta_%d.txt", i);
    contas[i].extrato = fopen(nome, "w");
    fprintf(contas[i].extrato, "Extrato da conta %d - saldo inicial: R$ %.2f\n", i, contas[i].saldo);
    contas[i].id = i;
    contas[i].saldo = 1000.00;
    pthread_mutex_init(&contas[i].lock, NULL);
  }
}

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
double bankAccountBalance = 0;

void deposito(int conta, double valor) {
  pthread_mutex_lock(&contas[conta].lock);
    contas[conta].saldo += valor;
    fprintf(contas[conta].extrato, "DEPOSITO       +%.2f    saldo: %.2f\n", valor, contas[conta].saldo);
  pthread_mutex_unlock(&contas[conta].lock);
}

int saque(int conta, double valor) {
  int ok = 0;
  pthread_mutex_lock(&contas[conta].lock);
    if (contas[conta].saldo >= valor){
      contas[conta].saldo -= valor;
      ok = 1;
      fprintf(contas[conta].extrato, "SAQUE          -%.2f   saldo: %.2f\n", valor, contas[conta].saldo);
    } else {
      fprintf(contas[conta].extrato, "SAQUE NEGADO    %.2f   saldo insuficiente: %.2f\n", valor, contas[conta].saldo);
    }
  pthread_mutex_unlock(&contas[conta].lock);
  return ok;
}

double consulta_saldo(int conta){
  pthread_mutex_lock(&contas[conta].lock);
  double saldo = contas[conta].saldo;
  fprintf(contas[conta].extrato, "CONSULTA                 saldo: %.2f\n", saldo);
  pthread_mutex_unlock(&contas[conta].lock);
  return saldo;
}

int transferencia(int origem, int destino, double valor){
  if (origem == destino) {return 0;}

  int primeira, segunda;

  /* Isso serve pra criar uma "regra" na ordem dos locks, para que duas threads nao tranquem, por exemplo, 
  as duas contas em que a transferencia sera realizada, Ex.: Thread 1 -> transferencia de A -> B, Thread 2 -> transferencia de B -> A*/

  if (origem < destino) {
    primeira = origem;
    segunda = destino;
  } else {
    primeira = destino;
    segunda = origem;
  }

  int ok = 0;

  pthread_mutex_lock(&contas[primeira].lock);
  pthread_mutex_lock(&contas[segunda].lock);

  if (contas[origem].saldo >= valor){
    contas[origem].saldo -= valor;
    contas[destino].saldo += valor;
    ok = 1;

    fprintf(contas[origem].extrato,  "TRANSF. P/ %d   -%.2f   saldo: %.2f\n", destino, valor, contas[origem].saldo);
    fprintf(contas[destino].extrato, "TRANSF. DE %d   +%.2f   saldo: %.2f\n", origem,  valor, contas[destino].saldo);
  } else {
    fprintf(contas[origem].extrato, "TRANSF. NEGADA %.2f p/ %d   saldo insuficiente: %.2f\n", valor, destino, contas[origem].saldo);
  }

  pthread_mutex_unlock(&contas[segunda].lock);
  pthread_mutex_unlock(&contas[primeira].lock);
  
  return ok;
}

void limpar() {
    for (int i = 0; i < 5; i++){
      fprintf(contas[i].extrato, "Saldo final: R$ %.2f\n", contas[i].saldo);
      fclose(contas[i].extrato);
      pthread_mutex_destroy(&contas[i].lock);
    }
}