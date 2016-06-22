# mpegtsreader

Atividade prática para o processo de seleção para Estagiário em Desenvolvimento de Sistemas do LAVID - Edital Nº 02/2016

## Cenário

Um fluxo de TV Digital (MPEG-2 Transport Stream ou MPEG-2 TS) é organizado em pacotes de 188 bytes (pacotes TS). Esses pacotes podem transmitir mídias de áudio, vídeo ou dados. Alguns pacotes de dados são responsáveis por transportarem tabelas de sinalização (metadados) com informações sobre os pacotes de mídia. Essas tabelas de sinalização são conhecidas como PAT (Program Association Table) e PMT (Program Map Table). Mais informações sobre fluxo de TV Digital e tabelas de sinalização podem ser encontradas no endereço: http://bit.ly/28Jy4hv.

## Atividade

Implementar um programa em C/C++ (ou Java) que leia o fluxo de pacotes TS do arquivo vídeo.ts e que extraia as informações das tabelas de sinalização PAT e PMT. A saída do programa deve apresentar as principais infomações contidas nessas tabelas. A especificação técnica H.222.01 estrutura dos pacotes TS, das tabelas de sinalização e de outras informações da ITU (International Telecomunication Union) define a estrutura dos pacotes TS, das tabelas de sinalização e de outras informações.

## Informações para compilar

Atualmente, o código está utilizando o Visual Studio 2015, com isso, necessita-se do mesmo para que possa ser compilado e executado.
