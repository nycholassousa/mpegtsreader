# mpegTSreader

Atividade prática para o processo de seleção para Estagiário em Desenvolvimento de Sistemas do LAVID - Edital Nº 02/2016

## Cenário

Um fluxo de TV Digital (MPEG-2 Transport Stream ou MPEG-2 TS) é organizado em pacotes de 188 bytes (pacotes TS). Esses pacotes podem transmitir mídias de áudio, vídeo ou dados. Alguns pacotes de dados são responsáveis por transportarem tabelas de sinalização (metadados) com informações sobre os pacotes de mídia. Essas tabelas de sinalização são conhecidas como PAT (Program Association Table) e PMT (Program Map Table). Mais informações sobre fluxo de TV Digital e tabelas de sinalização podem ser encontradas no endereço: http://bit.ly/28Jy4hv.

## Atividade

Implementar um programa em C/C++ (ou Java) que leia o fluxo de pacotes TS do arquivo vídeo.ts e que extraia as informações das tabelas de sinalização PAT e PMT. A saída do programa deve apresentar as principais infomações contidas nessas tabelas. A especificação técnica H.222.01 estrutura dos pacotes TS, das tabelas de sinalização e de outras informações da ITU (International Telecomunication Union) define a estrutura dos pacotes TS, das tabelas de sinalização e de outras informações.

## Compilando o código

Atualmente, o código está utilizando o Visual Studio 2015, com isso, necessita-se do mesmo para que possa ser compilado.

## Executando o programa

	mpegtsreader.exe <arquivo>
	mpegtsreader.exe video.ts
	
## Durante a execução

Quando o programa está executando, ele ficará lendo o vídeo e imprimindo as principais informações das tabelas requisitadas num arquivo com o nome de "packet.txt", onde o mesmo estará no mesmo diretório do programa.
Dependendo do tamanho do vídeo, pode ser que demore um pouco pra gerar o txt com todas as tabelas, por isso, peço que aguarde um pouco enquanto isso é feito (Com o video.ts fornecido no edital, demora em torno de 5 minutos pra gerar o txt detalhado com TODOS os packets do vídeo).
	
## Observações

Caso não queira compilar, no repositório já possui a última versão compilada, precisando apenas que você execute.
O código foi testado apenas no Windows, não garanto que ele funcione corretamente caso você apenas pegue o código e compile para Linux, caso faça isso, o resultado pode (ou não) ser diferente do desejado.
