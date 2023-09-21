<p align="center">
    Universidade Federal de Alagoas </br>
    Instituto de Computação </br>
    Matemática Discreta </br>
    Professor Bruno Pimentel </br>
    Projeto Final - Criptografia RSA </br>
</p>

## Requisitos do projeto sobre Criptografia RSA: 
Desenvolver um programa com interface gráfica e backend que realize as seguintes tarefa

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

> **Observação 1:** O backend, responsável pelo processamento das tarefas anteriormente listadas, deve ser escrito na linguagem C.
> **Observação 2:** A interface gráfica pode ser feita em qualquer linguagem de programação.
> **Observação 3:** O grupo tem a opção de desenvolver para desktop ou web.
> **Observação 4:** A mensagem deve ser encriptada usando a tabela ASCII, contendo os caracteres no intervalo de 32 (espaço) até o 126 (∼). [A tabela ASCII pode ser consultada aqui](https://www.ime.usp.br/~pf/algoritmos/apend/ascii.html).

## Teste dos programas

Os programas apresentados serão testados da seguinte forma:
1. Uma primeira equipe fornecerá uma chave pública para uma segunda equipe, que criará uma mensagem (por exemplo: O meu e-mail: bruno123@ic.com {sem .br}.) e a encriptará com a chave fornecida pela primeira equipe;
2. Em seguida, a segunda equipe entregará a mensagem encriptada para a primeira equipe (via e-mail ou pendrive) que terá que realizar a desencriptação sem quaisquer problemas;
3. Em seguida, o processo inverso será testado (a segunda equipe recebe a mensagem).

### Critérios de Avaliação:

1. Uso e entendimento correto dos conceitos matemáticos envolvidos _(30%)_;
2. Organização do programa _(20%)_;
3. Eficácia do programa: funcionamento correto no teste final descrito acima _(25%)_;
4. Apresentação: didática e clareza na explicação, demonstração de domínio do assunto, e etc _(25%)_.

### Link Úteis:

#### Como implementar o backend em C
-   [Como criar um servidor com sockets em C](https://beej.us/guide/bgnet/html/#system-calls-or-bust)
-   [Dicas de um usuário de como criar um servidor HTTP simples em C](https://stackoverflow.com/questions/176409/build-a-simple-http-server-in-c)

#### Serviços de web servers em C
-   [Mongoose](https://mongoose.ws/)
-   [Biblioteca Vely](https://vely.dev/)
-   [Web server LightTPD](http://www.lighttpd.net/)