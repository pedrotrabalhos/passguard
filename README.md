# Passguard

Uma ferramenta para gerenciar senhas de forma segura e prática.

## Instalação das bibliotecas

A aplicação é feita para sistemas Linux, esses passos de instalação são para a instalação das bibliotecas em sistemas Debian/Ubuntu:

### Instalação da biblioteca `ncurses` para interfaces

```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```

### Instalação da biblioteca `xclip` para copiar senhas para a área de transferência

```bash
sudo apt install xclip
```

## Compilação manual

Para compilar o projeto manualmente, basta executar o comando `gcc` com os seguintes argumentos na raiz do projeto.

```bash
gcc -o passguard src/main.c src/password.c src/clipboard.c src/credential.c -lncurses
```

## Execução

Para executar o projeto, basta executar o comando `./passguard` na raiz do projeto.

```bash
./passguard
```
