#include <iostream>
#include "Parser.h"

/*
INFOS:
1. O documento H.222.0 apresenta a especificação do pacote TS na página 18;
da tabela PAT na página 43; e da tabela PMT na página 46.
2. PMT SÓ APARECE QUANDO PAT TABLE_ID = 2

TODO:
1. Corrigir erro do program_map_PID/netword_PID da tabela PAT, está imprimindo um valor incorreto (rever os bytes/bits)
2. Inserir tabela PMT
3. Inserir vídeo como argumento
4. Não ficar chamando a função readBytes() direto, fazer num loop
5. Salvar o que for impresso em um arquivo txt, para não poluir o console
*/

int main(int argc, char ** argv) {
	Parser* tsParser = new Parser("C:\\Users\\nycho\\Documents\\GitHub\\mpegtsreader\\Debug\\video.ts");
	tsParser->readBytes();
	tsParser->readBytes();
	tsParser->readBytes();
	tsParser->readBytes();

	system("pause");
	delete tsParser;
	return 0;
}
