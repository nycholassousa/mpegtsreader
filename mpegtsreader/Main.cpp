#include "Parser.h"

/*
INFOS:
1.  O documento H.222.0 apresenta a especificação do pacote TS na página 18;
	da tabela PAT na página 43; e da tabela PMT na página 46.
2.  Correções e verificações dos valores foram feitos através do software disponibilizado em:
	http://www.pjdaniel.org.uk/mpeg/.

TODO:
1.  Corrigir erro do program_map_PID/netword_PID da tabela PAT, 
	está imprimindo um valor incorreto (rever os bytes/bits) - EM ANDAMENTO
2.  Inserir tabela PMT - EM ANDAMENTO
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

	std::cout << "Começando a ler o "<< argv[1] << ", aguarde o aviso de que o arquivo foi lido completamente." << std::endl;
	tsParser->readBytes();
	std::cout << "Arquivo lido e packet.txt com as informações do pacote TS criados." << std::endl;

	system("pause");
	delete tsParser;
	return 0;
}
