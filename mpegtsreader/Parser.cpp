#include "Parser.h"

int pkt_number = 0;
int pat_number = 0;
int pmt_number = 0;

/*
Conversão de char para binário
*/
void printbincharpad(char c) {
	for (int i = 7; i >= 0; --i) {
		putchar((c & (1 << i)) ? '1' : '0');
	}
	putchar('\n');
}

/*
Função responsável pelo início do parser no vídeo
Como a atividade requer que vídeo seja lido/aberto a cada 188 bytes, é criado o packetSize
*/
Parser::Parser(std::string videoFile) {
	fileName = videoFile;
	packetSize = 188;
	packet = new unsigned char[188]();

	reader.open(fileName, std::ifstream::in | std::ifstream::binary);
	if (!reader.is_open()) {
		throw "Could not open file";
	}
}

/*
Destructor da Classe
*/
Parser::~Parser() {
	reader.close();
	delete packet;
}

/*
Função readBytes()
Responsável pela leitura de bytes no arquivo e exibição dos valores
*/
void Parser::readBytes() {
	while (!reader.eof()) {
		myFile.open("packets.txt", std::ofstream::app);
		reader.read((char*)(&packet[0]), packetSize);

		//O valor de packet (packet[i]) muda quando passa 8 bytes 
		//sync_byte possui um valor fixo, que é 0x47, porém, caso esse valor não seja mais esse, ele faz a leitura de novo
		if (packet[0] != 0x47)
			//Para não mostrar pacotes nulos ou indevidos, foi feito tal coisa
			readBytes();
		else {
			//Separando os pacotes por numero, apenas por organização
			myFile << "--------------------   " << ++pkt_number << "o Pacote" << "   --------------------   " << std::endl;
			//sync_byte possui um valor fixo, '0100 0111' ou '0x47'
			myFile << "sync_byte = " << "0x47" << std::endl;
			//transport_error_indicator necessita de apenas 1 bit (1º mais a esquerda), ele é pegado e movido 7 "casas", para que possa ser impresso
			myFile << "transport_error_indicator = " << ((packet[1] & 0x80) >> 7) << std::endl;
			//payload_unit_start_indicator é o segundo bit mais a esquerda, pega-se ele e move 6 "casas" para ser impresso
			myFile << "payload_unit_start_indicator = " << ((packet[1] & 0x40) >> 6) << std::endl;
			//transport_priority é o 3º mais a esquerda, move-se 5 e é impresso
			myFile << "transport_priority = " << ((packet[1] & 0x20) >> 5) << std::endl;
			//PID é o restante do 2º byte e o 3º byte, faz-se uma soma de binário para imprimi-lo
			myFile << "PID = " << (((packet[1] & 0x1F) << 8) | packet[2]) << std::endl;

			/*
			Se o PID do pacote TS for 0, isso quer dizer que há uma tabela PAT
			Caso o table_id (packet[5]) seja 2, ele será PMT, por isso, não entra nessa condição
			De acordo com isso, quando ocorrer que o PID seja 0, os dados da tabela PAT será impressa logo abaixo do PID
			*/
			if (((((packet[1] & 0x1F) << 8) | packet[2]) == 0) && ((unsigned int)packet[5] != 2)) {
				//Simples informação ao usuário
				myFile << "\t ---- " << ++pat_number << "o Pacote PAT" << std::endl;
				//table_id corresponde a 1 byte, ele é modificado pra unsigned int devido a mnemonica dele (uimsbf)
				myFile << "\tPAT table_id = " << (unsigned int)packet[5] << std::endl;
				//section_syntax_indicator é apenas 1 bit, pega-se ele e move para direita para ser impresso
				myFile << "\tPAT section_syntax_indicator = " << ((packet[6] & 0x80) >> 7) << std::endl;

				//Como são valores reservados, não vi a necessidade de que fossem impressos
				//myFile << "\tPAT 0 = " << ((packet[6] & 0x40) >> 6) << std::endl;
				//myFile << "\tPAT reserved1 = " << ((packet[6] & 0x20) >> 5) << std::endl;

				//section_lenght, restante do byte somado com o próximo byte para ser exibido
				myFile << "\tPAT section_lenght = " << (unsigned int)((packet[6] & 0x0F) | packet[7]) << std::endl;
				//transport_stream_id soma 2 bytes e exibe
				myFile << "\tPAT transport_stream_id = " << (unsigned int)(packet[8] | packet[9]) << std::endl;

				/*
				Aqui há um segundo reserved da tabela, que seria os dois primeiros bits de packet[10]
				Por ser um valor reservado, não vi novamente necessidade de imprimir
				Além do mais, não vi nada na especificação falando sobre esses campos de nome 'reserved'
				*/

				//version_number, pega do 3º ao 7º bit e move para direita, mnemonica uimsbf
				myFile << "\tPAT version_number = " << (unsigned int)((packet[10] & 0x3E) >> 1) << std::endl;
				//current_next_indicator, é lido o último bit (mais a direita) e impresso
				myFile << "\tPAT current_next_indicator = " << (packet[10] & 0x01) << std::endl;
				//section_number possui um valor de 8 bytes e mnemonica uimsbf, por isso é pego apenas o valor completo e convertido
				myFile << "\tPAT section_number = " << (unsigned int)(packet[11]) << std::endl;
				//last_section_number possui um valor de 8 bytes e mnemonica uimsbf, por isso é pego apenas o valor completo e convertido
				myFile << "\tPAT last_section_number = " << (unsigned int)(packet[12]) << std::endl;

				//Uma pequena operação é realizada para calcular N, que seria a quantidade de vezes para achar program_number
				int table_count = ((packet[6] & 0x0F) | packet[7]);
				table_count = ((table_count - 9) / 4);
				int i;

				//loop para pesquisa do program_number, network_PID ou program_map_PID, ambos de mnemonica uimsbf
				for (i = 0; i < table_count; i++) {
					myFile << "\tPAT program_number = " << (unsigned int)(packet[13 + (i * 4)] | packet[14 + (i * 4)]) << std::endl;
					if ((unsigned int)(packet[13 + (i * 4)] | packet[14 + (i * 4)]) == 0) //if program_number == 0
						myFile << "\tPAT network_PID = " << (unsigned int)((packet[15 + (i * 4)] & 0x1F) | packet[16 + (i * 4)]) << std::endl;
					else
						myFile << "\tPAT program_map_PID = " << (unsigned int)((packet[15 + (i * 4)] & 0x1F) | packet[16 + (i * 4)]) << std::endl;
				}

				//Foi melhor mostrar o CRC_32 em hex, por isso,converti cada 8 bytes em hex
				myFile << "\tPAT CRC_32 = " << std::hex << (unsigned int)packet[17] << " " << (unsigned int)packet[18] <<
					" " << (unsigned int)packet[19] << " " << (unsigned int)packet[20] << std::dec << std::endl;
				myFile << "\t ---- " << std::endl;
			}

			if ((unsigned int)packet[5] == 2) { //quando table_id = 2, significa que contem informações sobre o PMT (pagina 42)
				myFile << "\t ---- " << ++pmt_number << "o Pacote PMT" << std::endl;
				//table_id tem 1 byte, é lido todo, mnemonica uimsbf
				myFile << "\tPMT table_id = " << (unsigned int)packet[5] << std::endl;
				//section_syntax_indicator é apenas 1 bit, pega-se ele e move para direita para ser impresso
				myFile << "\tPMT section_syntax_indicator = " << ((packet[6] & 0x80) >> 7) << std::endl;
				//section_lenght são 12 bits, necessário pegar os 4 restantes e somar com os 8 depois
				myFile << "\tPMT section_lenght = " << (unsigned int)((packet[6] & 0x0F) | packet[7]) << std::endl;
				//program_number são 16 bits, faz-se a soma
				myFile << "\tPMT program_number = " << (unsigned int)(packet[8] | packet[9]) << std::endl;
				//
				myFile << "\tPMT version_number = " << (unsigned int)((packet[10] & 0x3E) >> 1) << std::endl;
				//
				myFile << "\tPMT current_next_indicator = " << (packet[10] & 0x01) << std::endl;
				//
				myFile << "\tPMT section_number = " << (unsigned int)(packet[11]) << std::endl;
				//
				myFile << "\tPMT last_section_number = " << (unsigned int)(packet[12]) << std::endl;
				//
				myFile << "\tPMT PCR_PID = " << (unsigned int)((packet[13] & 0x1F) | packet[14]) << std::endl;
				//
				myFile << "\tPMT program_info_lenght = " << (unsigned int)((packet[15] & 0x0F) | packet[16]) << std::endl;

				int table_count = ((packet[6] & 0x0F) | packet[7]);
				table_count = ((table_count - 9) / 4);
				int i;

				for (i = 0; i < 1; i++) {
					//
					myFile << "\tPMT stream_type = " << (unsigned int)(packet[17 + (i * 4)]) << std::endl;
					//
					myFile << "\tPMT elementary_PID = " << (unsigned int)((packet[18 + (i * 4)] & 0x1F) | packet[19 +(i * 4)]) << std::endl;
					//
					myFile << "\tPMT ES_info_lenght = " << (unsigned int)((packet[18 + (i * 4)] & 0x1F) | packet[19 + (i * 4)]) << std::endl;

				}
				//
				myFile << "\tPMT CRC_32 = " << std::hex << (unsigned int)packet[20 + (i * 4)] << " " << (unsigned int)packet[21 + (i * 4)] <<
					" " << (unsigned int)packet[22 + (i * 4)] << " " << (unsigned int)packet[23 + (i * 4)] << std::dec << std::endl;

				myFile << "\t ---- " << std::endl;
			}

			//Aqui volta a ser exibido o restante do pacote TS, após finalizar as exibições da tabela PAT
			myFile << "transport_scrambling_control = " << (packet[3] & 0xC0) << std::endl;
			myFile << "adaptation_field_control = " << ((packet[3] & 0x30) >> 4) << std::endl;
			myFile << "continuity_counter = " << (packet[3] & 0xF) << std::endl;
			myFile << "----------------------------------------------------------" << std::endl;
			myFile << std::endl;

			myFile.close();
		}
	}
}