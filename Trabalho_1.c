#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

int main(){
    char entrada[20]; // recebe a equação do usuário, em c string n é nativa
    float val1, val2; // variáveis para armazenar os valores do cálculo
    char op[2]; // armazana a operação escolhida
    char IEEEr[32], IEEEval1[32], IEEEval2[32];
    bool inexato = 0, div_zero = 0, underflow = 0, overflow = 0, invalid = 0;bool inexato, div_zero, underflow, overflow, invalid = 0;

    printf("Digite a expressão no formato 'val1 op val2', sem espaços entre os valores e a operação: ");
    scanf("%s", entrada);
    sscanf(entrada, "%f %1s %f", &val1, op, &val2); // le os valores da entrada e os separa
    printf("Valores separados: %.6f %s %.6f\n", val1, op, val2);

    //CALCULANDO O BIT DE SINAL
    if(val1 < 0){
        IEEEval1[31] = 1; // se o número negativo o bit de sinal é um
    }else{
        IEEEval1[31] = 0; // se o número positivo o bit de sinal é zero
    }

    if(val2 < 0){
        IEEEval2[31] = 1; // se o número negativo o bit de sinal é um
    }else{
        IEEEval2[31] = 0; // se o número positivo o bit de sinal é zero
    }

    //CALCULANDO OS EXPOENTES
    uint32_t *ponteiro1 = (uint32_t*)&val1; // Ponteiro para acessar os bits do valor 1
    uint32_t expoente1 = (*ponteiro1 >> 23) & 0xFF;// ponteiro para extrair o expoente shiftando 23 bits e aplicando a máscara 0xFF para garantir que só os bits do expoente sejam pegos
    for (int i = 7; i >= 0; i--) {  // Copiar o expoente para IEEEval1[30] até IEEEval1[23]
        IEEEval1[i + 23] = (expoente1 & (1 << i)) ? '1' : '0'; //ele shifta os bits para pegar só a posição correspondente, e verifica se é um, se for é preenchido como um, caso contrário como zero
    }

    uint32_t *ponteiro2 = (uint32_t*)&val2; 
    uint32_t expoente2 = (*ponteiro2 >> 23) & 0xFF;
    for (int i = 7; i >= 0; i--) { 
        IEEEval2[i + 23] = (expoente2 & (1 << i)) ? '1' : '0'; // CASO DE MERDA NESSA PARTE É ESSE 23 Q EU N TENHO CERTEZA AINDA
    }

    //CALCULANDO AS MANTISSAS, PASSOS:
    uint32_t *ponteiro1_1 = (uint32_t*)&val1; // Ponteiro para acessar os bits do número
    //1: Extração dos bits
    uint32_t mantissa1 = *ponteiro1_1 & 0x7FFFFF; // Máscara para obter os 23 bits da mantissa para garantir que só os bits da mantissa sejam pegos
    //2:armazenando nos bits finais do IEEEval1
    for (int i = 22; i >= 0; i--) {
        IEEEval1[i + 23] = (mantissa1 & (1 << i)) ? '1' : '0';//ele shifta os bits para pegar só a posição correspondente, e verifica se é um, se for é preenchido como um, caso contrário como zero
    }
    uint32_t *ponteiro2_2 = (uint32_t*)&val2;
    uint32_t mantissa2 = *ponteiro2_2 & 0x7FFFFF; 
    for (int i = 22; i >= 0; i--) {
        IEEEval2[i + 23] = (mantissa2 & (1 << i)) ? '1' : '0';
    }

    // CALCULO DA OPERAÇÃO 
    switch (op[2]){
    case'+':
        //comparação dos expoentes(eles devem ser iguais, caso n devo adequar)
        
        //soma das mantissas
        //normalizar resultado, aqui pode dar over ou underflow, caso de devo arredondar e normalizar
        break;
    case'-':
        //Identificar os Sinais e Expoentes: Verifique os sinais e expoentes dos números que serão subtraídos.
        //Ajustar os Expoentes: Igualar os expoentes dos números, deslocando a mantissa do número com o expoente menor para alinhar com o expoente do número com o maior expoente.
        //Subtrair as Mantissas: Subtraia as mantissas dos números alinhados, considerando os sinais.
        //Normalizar o Resultado: Normalize o resultado da subtração ajustando o expoente e a mantissa conforme necessário
        //Verificar Underflow/Overflow: Certifique-se de verificar se houve underflow (resultado muito pequeno) ou overflow (resultado muito grande) durante a operação.
        break;
    case'*':
        //Soma os expoentes (tomando cuidado com o bias).
        //Multiplicar as mantissas.
        break;
    case'/':
        // verificar se é divisão por zero
        //Identificar os Sinais e Expoentes: Verifique os sinais e expoentes dos números que serão divididos.
        //Ajustar os Expoentes: Subtraia o expoente do divisor do expoente do dividendo para obter o novo expoente.
        //Dividir as Mantissas: Divida as mantissas dos números, considerando os sinais e ajustando o expoente conforme necessário.
        //Normalizar o Resultado: Normalize o resultado da divisão ajustando o expoente e a mantissa conforme necessário.
        //Verificar Underflow/Overflow: Certifique-se de verificar se houve underflow (resultado muito pequeno) ou overflow (resultado muito grande) durante a operação.
        if(val2 == 0){
                div_zero = 1;
            }
        break;    
    default:
        invalid = 1;
        break;
    }
    // FALTA A  COMPARAÇÃO PRA SABER SE DEU +-INF OU +-0 OU NAN
   


    // SAÍDA COM AS RESPOSTAS:
    //printf("Recebi %.6f / %.6f e o resultado deu %.6f\n", val1, val2, resposta);
    printf("Exeção FE_INEXACT: %d\n",inexato );
    printf("Exeção FE_DIVBYZERO: %d\n",div_zero );
    printf("Exeção FE_UNDERFLOW: %d\n",underflow );
    printf("Exeção FE_OVERFLOW: %d\n",overflow);
    printf("Exeção FE_INVALID: %d\n",invalid );
}
