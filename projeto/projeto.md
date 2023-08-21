<p align="center">
    Universidade Federal de Alagoas </br>
    Instituto de Computação </br>
    Matemática Discreta </br>
    Professor Bruno Pimentel </br>
    Projeto Final - Criptografia RSA </br>
</p>

## Requisitos do projeto sobre Criptografia RSA: 
Desenvolver um programa em uma linguagem de programação à escolha da equipe que realize a seguinte tarefa:

Solicite que o usuário escolha entre 3 opções: 

    1 - Gerar chave pública
    2 - Encriptar
    3 - Desencriptar

Caso escolhida a opção ***1 - Gerar chave pública***:

    - Solicite que o usuário digite um par de números primos `p` e `q` e um expoente `e` relativamente primo a(p−1)(q−1).
    - Salve, no diretório de execução do programa, um arquivo txt com os dois números que formam a chave pública.

Caso escolhida a opção ***2 - Encriptar***:

    - Solicite que o usuário digite a mensagem de texto a encriptar.
    - Solicite que o usuário digite a chave pública recebida previamente.
    - Salve, no diretório de execução do programa, um arquivo txt com a mensagem encriptada.

Caso escolhida a opção ***3 - Desencriptar***:

    - Solicite que o usuário digitep, `q` e `e`.
    - Salve, no diretório de execução do programa, um arquivo txt com a mensagem desencriptada.

> **Observação 1:** A mensagem deve ser encriptada usando o alfabeto de letras A - Z, codificado com inteiros de 2 a 28, onde 2 = A, 3 = B,..., 27 = Z, 28 = espaço.

## Teste dos programas

Uma primeira equipe fornecerá uma chave pública para uma segunda equipe, que criará uma mensagem e a encriptará com a chave fornecida pela primeira equipe. </br>
Em seguida, a segunda equipe entregará a mensagem encriptada para a primeira equipe (via e-mail ou pendrive) que terá que realizar a desencriptação sem quaisquer problemas.
Em seguida, o processo inverso será testado (a segunda equipe recebe a mensagem).

### Critérios de Avaliação:

1. Uso e entendimento correto dos conceitos matemáticos envolvidos _(30%)_;
2. Organização do programa _(20%)_;
3. Eficácia do programa: funcionamento correto no teste final descrito acima _(25%)_;
4. Apresentação: didática e clareza na explicação, demonstração de domínio do assunto, e etc _(25%)_.