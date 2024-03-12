#include <fenv.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

int main() {
  float val1, val2, resultado = 0.0; 
  char op;     
  char IEEEr[33], IEEEval1[33], IEEEval2[33];
  // Configurando as flags de exceção a serem testadas
  feraiseexcept(FE_DIVBYZERO | FE_UNDERFLOW | FE_OVERFLOW | FE_INEXACT | FE_INVALID);
  feclearexcept(FE_DIVBYZERO | FE_UNDERFLOW | FE_OVERFLOW | FE_INEXACT | FE_INVALID);
  
  printf("Digite a expressão no formato 'val1 op val2', separando os valores e a operação por espaços.Caso a operação seja multiplicação, use o 'X' como indicador da operação: ");
  scanf("%f %c %f", &val1, &op, &val2);
  // Realizando as operações
  switch (op) {
  case '+': resultado = val1 + val2;
    break;
  case '-': resultado = val1 - val2;
    break;
  case 'X': resultado = val1 * val2;
    break;
  case '/': resultado = val1 / val2;
    break;
  }
  // Obtendo os bits das variáveis
  uint32_t *ponteiro_val1 = (uint32_t *)&val1;
  uint32_t *ponteiro_val2 = (uint32_t *)&val2;
  uint32_t *ponteiro_resultado = (uint32_t *)&resultado;
  // Copiando os bits para as variáveis de destino
  for (int i = 0; i < 32; i++) {
    IEEEval1[i] = (*ponteiro_val1 >> (31 - i)) & 1 ? '1' : '0';
    IEEEval2[i] = (*ponteiro_val2 >> (31 - i)) & 1 ? '1' : '0';
    IEEEr[i] = (*ponteiro_resultado >> (31 - i)) & 1 ? '1' : '0';
  }
  // Adicionando o caractere nulo ao final dos arrays
  IEEEval1[32] = '\0';
  IEEEval2[32] = '\0';
  IEEEr[32] = '\0';

  printf("Recebi %.6f %c %.6f e o resultado deu %.6f\n\n", val1, op, val2, resultado);
  printf("Bits de Val1: %c %.8s %.23s = %.3f\n", IEEEval1[0], IEEEval1 + 1, IEEEval1 + 9, val1);
  printf("Bits de Val2: %c %.8s %.23s = %.3f\n", IEEEval2[0], IEEEval2 + 1, IEEEval2 + 9, val2);
  printf("Bits do Resultado: %c %.8s %.23s = %.3f\n\n", IEEEr[0], IEEEr + 1, IEEEr + 9, resultado);
  printf("Exeção FE_INEXACT: %d\n",  0 != fetestexcept(FE_INEXACT));
  printf("Exeção FE_DIVBYZERO: %d\n", 0 != fetestexcept(FE_DIVBYZERO));
  printf("Exeção FE_UNDERFLOW: %d\n",  0 != fetestexcept(FE_UNDERFLOW));
  printf("Exeção FE_OVERFLOW: %d\n",  0 != fetestexcept(FE_OVERFLOW));
  printf("Exeção FE_INVALID: %d\n",  0 != fetestexcept(FE_INVALID));
}
