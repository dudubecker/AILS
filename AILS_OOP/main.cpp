#include <iostream>
#include <vector>
#include "Instance.hpp"
#include "Sol.hpp"
#include "LocalSearchOperator.hpp"
#include "AILS.hpp"
//#include <heuristicsFunctions.h>
#include "Perturbation.hpp"
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <thread>
#include <random>
#include <unordered_map>

// init para rodar IRACE

int main(int argc, char *argv[]){
	
	// Verifica se o número correto de argumentos foi fornecido
	if (argc != 17) {
		std::cerr << "Uso: seu_executavel <id_configuration> <id_instance> <seed> <instancia> --eta <eta> --kappa <kappa> --Gamma <Gamma> --d_b <d_b> --noise <noise> --alpha <alpha>" << std::endl;
		return 1; // Retorna código de erro
	}
	
	
	// Mapeia as flags para os valores correspondentes
	std::unordered_map<std::string, std::string> args;
	for (int i = 5; i < argc; i += 2) {
		args[argv[i]] = argv[i + 1];
	}
	
	// Lê os argumentos da linha de comando
	long long seed = std::stoll(argv[3]);
	std::string instancia = argv[4];
	double eta = std::stod(args["--eta"]);
	double kappa = std::stod(args["--kappa"]);
	int Gamma = std::stoi(args["--gamma"]);
	int d_b = std::stoi(args["--d_b"]);
	double noise = std::stod(args["--noise"]);
	double alpha = std::stod(args["--alpha"]);
	
	// Lendo argumentos e atribuindo variáveis
	
	// // Parâmetros:
	
	// instancia: Diretório para a instância (em relação ao diretório onde está o executável) e nome
	// eta: Determina b_UP no critério de aceitação
	// kappa: Porcentagem de soluções aceitas
	// Gamma: Determina quantas iterações cada heurística de perturbação realizará com um mesmo "peso"
	// d_b: Distância de referência ("ideal") entre soluções
	// eta_noise: Utilizado no cálculo do ruído aleatório
	// alpha: Probabilidade de aplicação do ruído aleatório
	
	
	// // Código principal
	
	// Para gerar números aleatórios
	// srand(time(NULL));
	
	// Para ter controle sobre os outputs
	srand(seed);
	
	// /*
	
	// Inicializando métodos de perturbação:
	
	Perturbation Random('R');
	
	Perturbation Worst('W');
	
	Perturbation Concentric('C');
	
	// Inicializando operadores de busca local:
	
	LocalSearchOperator Or_opt_1('O', 1);
	
	LocalSearchOperator Or_opt_2('O', 2);
	
	LocalSearchOperator Shaw_1('H', 2,0.3,0.4,0.3);
	
	LocalSearchOperator Shaw_2('H', 2,1,0,0);
	
	LocalSearchOperator Shaw_3('H', 2,0,1,0);
	
	LocalSearchOperator Shaw_4('H', 2,0,0,1);
	
	LocalSearchOperator Shift_2('T', 2);
	
	LocalSearchOperator Shift_4('T', 4);
	
	LocalSearchOperator Swap_1_1('S', 1,1);
	
	LocalSearchOperator Swap_2_1('S', 2,1);
	
	LocalSearchOperator Swap_2_2('S', 2,2);
	
	
	// Vetor que guardará os métodos de perturbação já inicializados
	std::vector<Perturbation> PerturbationProcedures {Random, Worst, Concentric};
	
	// Vetor que guardará os operadores de busca local já inicializados
	std::vector<LocalSearchOperator> LSOperators{
										Or_opt_1,
										Or_opt_2,
										Shaw_1,
										Shaw_2,
										Shaw_3,
										Shaw_4,
										Shift_2,
										Shift_4,
										Swap_1_1,
										Swap_2_1,
										Swap_2_2,
	};
	
	// Executando algoritmo
	
	// Objeto instância
	Instance inst(instancia);
	
	// Inicializando objeto solução;
	Sol S(inst);
	
	// Medindo tempo
	// auto begin = std::chrono::high_resolution_clock::now();
	
	// Inicializando objeto da AILS
	AILS AILSObject(S, // Solução inicial
					LSOperators, // Vetor com operadores de busca local
					PerturbationProcedures, // Vetor com métodos de perturbação
					eta, // eta: Determina b_UP no critério de aceitação
					kappa, // kappa: Porcentagem de soluções aceitas
					Gamma, // Gamma: Determina quantas iterações cada heurística de perturbação realizará com um mesmo "peso"
					d_b, // d_b: Distância de referência ("ideal") entre soluções
					noise, // noise: Utilizado no cálculo do ruído aleatório
					alpha // alpha: Probabilidade de aplicação do ruído aleatório
	);
	
	
	// Executando algoritmo
	AILSObject.executeAILS(
					10000, // max_it: Número máximo de iterações do algoritmo
					2500, // max_it_no_improv: Número máximo de iterações sem melhoria
					10, // it_RRH_interval: Intervalo de iterações para aplicação de route reduction heuristic
					10 // it_RRH: Número iterações da route reduction heuristic a cada intervalo
	);
	
	// auto end = std::chrono::high_resolution_clock::now();
	// auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
	
	// std::cout << "\n" << instancia << ";" << AILSObject.S_p.FO() << ";" << elapsed.count() * 1e-9 << std::endl;
	
	std::cout << AILSObject.S_p.FO() << std::endl;
	
	// return AILSObject.S_p.FO();
	// return 0;
}

// Init normal, para ensaios computacionais

/*

int main(){
	
	// Para gerar números aleatórios
	// srand(time(NULL));
	
	
	// Para ter controle sobre os outputs
	srand(125);
	
	// Inicializando métodos de perturbação:
	
	Perturbation Random('R');
	
	Perturbation Worst('W');
	
	Perturbation Concentric('C');
	
	// Inicializando operadores de busca local:
	
	LocalSearchOperator Or_opt_1('O', 1);
	
	LocalSearchOperator Or_opt_2('O', 2);
	
	LocalSearchOperator Shaw_1('H', 2,0.3,0.4,0.3);
	
	LocalSearchOperator Shaw_2('H', 2,1,0,0);
	
	LocalSearchOperator Shaw_3('H', 2,0,1,0);
	
	LocalSearchOperator Shaw_4('H', 2,0,0,1);
	
	LocalSearchOperator Shift_2('T', 2);
	
	LocalSearchOperator Shift_4('T', 4);
	
	LocalSearchOperator Swap_1_1('S', 1,1);
	
	LocalSearchOperator Swap_2_1('S', 2,1);
	
	LocalSearchOperator Swap_2_2('S', 2,2);
	
	
	// Vetor que guardará os métodos de perturbação já inicializados
	std::vector<Perturbation> PerturbationProcedures {Random, Worst, Concentric};
	
	// Vetor que guardará os operadores de busca local já inicializados
	std::vector<LocalSearchOperator> LSOperators{
										Or_opt_1,
										Or_opt_2,
										Shaw_1,
										Shaw_2,
										Shaw_3,
										Shaw_4,
										Shift_2,
										Shift_4,
										Swap_1_1,
										Swap_2_1,
										Swap_2_2,
									};
	
	
	// Instâncias
	
	// Número de iterações por instância
	int iteracoes_por_instancia = 5;
	
	std::vector<std::string> instancias = {
		
		//"instances/AA5",
		//"instances/AA10",
		//"instances/AA15",
		//"instances/AA20",
		//"instances/AA25",
		//"instances/AA30",
		//"instances/AA35",
		//"instances/AA40",
		//"instances/AA45",
		//"instances/AA50",
		//"instances/AA55",
		//"instances/AA60",
		//"instances/AA65",
		//"instances/AA70",
		//"instances/AA75",
		//"instances/BB5",
		//"instances/BB10",
		//"instances/BB15",
		//"instances/BB20",
		//"instances/BB25",
		//"instances/BB30",
		//"instances/BB35",
		//"instances/BB40",
		//"instances/BB45",
		// "instances/BB50",
		//"instances/BB55",
		"instances/BB60",
		// "instances/BB65",
		// "instances/BB70",
		// "instances/BB75",
		//"instances/CC5",
		//"instances/CC10",
		//"instances/CC15",
		//"instances/CC20",
		// "instances/CC25",
		//"instances/CC30",
		//"instances/CC35",
		// "instances/CC40",
		//"instances/CC45",
		// "instances/CC50",
		//"instances/CC55",
		// "instances/CC60",
		// "instances/CC65",
		//"instances/CC70",
		//"instances/CC75",
		//"instances/DD5",
		//"instances/DD10",
		//"instances/DD15",
		//"instances/DD20",
		//"instances/DD25",
		//"instances/DD30",
		//"instances/DD35",
		//"instances/DD40",
		// "instances/DD45",
		// "instances/DD50",
		//"instances/DD55",
		// "instances/DD60",
		//"instances/DD65",
		// "instances/DD70",
		// "instances/DD75",
	};
	
	
	// Executando algoritmo
	
	for (auto instancia: instancias){
		
		// Objeto instância
		Instance inst(instancia);
		
		// Inicializando objeto solução;
		Sol S(inst);
		
		for (int i {0}; i < iteracoes_por_instancia; i++){
			
			
			// try{
				
			// Medindo tempo
			auto begin = std::chrono::high_resolution_clock::now();
			
			
			// Inicializando objeto da AILS
			AILS AILSObject(S, // Solução inicial
							LSOperators, // Vetor com operadores de busca local
							PerturbationProcedures, // Vetor com métodos de perturbação
							0.5, // eta: Determina b_UP no critério de aceitação
							0.35, // kappa: Porcentagem de soluções aceitas
							20, // Gamma: Determina quantas iterações cada heurística de perturbação realizará com um mesmo "peso"
							24, // d_b: Distância de referência ("ideal") entre soluções
							0.01, // eta_noise: Utilizado no cálculo do ruído aleatório
							0.01 // alpha: Probabilidade de aplicação do ruído aleatório
							);
			
			
			// Executando algoritmo
			AILSObject.executeAILS(
							10000, // max_it: Número máximo de iterações do algoritmo
							2500, // max_it_no_improv: Número máximo de iterações sem melhoria
							5, // it_RRH_interval: Intervalo de iterações para aplicação de route reduction heuristic
							100 // it_RRH: Número iterações da route reduction heuristic a cada intervalo
							);
			
			auto end = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
			
			std::cout << "\n" << instancia << ";" << AILSObject.S_p.FO() << ";" << elapsed.count() * 1e-9 << std::endl;
			// }
			
			// catch(...){
				
			//	std::cout << "\n\nErro: instancia " << instancia << "\n\n" << std::endl; 
			//	
			//}
		}
		
	}
	
	
	
	
	// return AILSObject.S_p.FO();
	return 0;
}

*/