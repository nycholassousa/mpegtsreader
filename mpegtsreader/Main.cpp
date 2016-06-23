#include "Parser.h"

/*
INFOS:
1.  O documento H.222.0 apresenta a especificação do pacote TS na página 18;
	da tabela PAT na página 43; e da tabela PMT na página 46.
2.  A tabela PMT apenas aparece quando o table_id tiver o valor 2.
4.  Correções e verificações dos valores foram feitos através do software disponibilizado em:
	http://www.pjdaniel.org.uk/mpeg/.
5.  A tabela PAT apenas ocorre quando o valor do PID do pacote TS for 0
6.  Aparentemente, não ocorre PMT e PAT ao mesmo tempo (ver sobre isso, não estou certo sobre isso ainda)

TODO:
1.  Corrigir erro do program_map_PID/netword_PID da tabela PAT, 
	está imprimindo um valor incorreto (rever os bytes/bits) - EM ANDAMENTO
2.  Inserir tabela PMT - EM ANDAMENTO
3.  Inserir vídeo como argumento - PRONTO
4.  Não ficar chamando a função readBytes() direto, fazer um loop -- PRONTO
5.  Salvar o que for impresso em um arquivo txt, para não poluir o console
	Possivelmente terei que alterar os cout da função readByte() pra tal coisa - PRONTO
*/

int main(int argc, char ** argv) {
	//Usando o programa através de argumentos
	/*
	if (argc != 2) {
		std::cerr << "Uso: mpegtsreader <video.extensao>" << std::endl;
		return EXIT_FAILURE;
	}
	*/

	//Parser* tsParser = new Parser(argv[1]);
	argv[1] = "video.ts";
	Parser *tsParser = new Parser("C:\\Users\\nycho\\Documents\\GitHub\\mpegtsreader\\Debug\\video.ts");

	//Para evitar erros e devido o append ao abrir o arquivo, o packets.txt antigo sempre será deletado ao executar o programa
	remove("packets.txt");

	std::cout << "Começando a ler o "<< argv[1] << ", aguarde o aviso que o arquivo seja lido completamente." << std::endl;
	tsParser->readBytes();
	std::cout << "Arquivo lido e packet.txt com as informações do pacote TS criados. " << std::endl;

	delete tsParser;
	return 0;
}
