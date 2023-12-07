#include "Instance.hpp"
#include "Sol.hpp"
#include "LocalSearchOperator.hpp"
#include "AILS.hpp"
#include "Perturbation.hpp"
#include <iostream>
#include <vector>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <thread>
#include <random>
#include <unordered_map>
#include <fstream>

// init para rodar IRACE

/*
int main(int argc, char *argv[]){
	
	
	// Parâmetros de controle dos critérios de parada
	
	int max_it = 1250; // Número máximo de iterações do algoritmo
	int max_it_sem_melhoria = 750; // Número máximo de iterações sem melhoria
	int it_RRH_intervalo = 125; // it_RRH_intervalo: Intervalo de iterações para aplicação de route reduction heuristic
	int it_RRH = 40; // it_RRH: Número iterações da route reduction heuristic a cada intervalo
	
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
	
	
	// Inicializando métodos de perturbação:
	
	// Inicializando métodos de perturbação:
	
	RandomRemoval* Random = new RandomRemoval;
	
	WorstRemoval* Worst = new WorstRemoval(6);
	
	ConcentricRemoval* Concentric = new ConcentricRemoval;
	
	// Inicializando operadores de busca local:
	
	OrOpt* Or_opt_1 = new OrOpt(1);
	
	OrOpt* Or_opt_2 = new OrOpt(2);
	
	Shaw* Shaw_1 = new Shaw(2,0.3,0.4,0.3);
	
	Shaw* Shaw_2 = new Shaw(2,1,0,0);
	
	Shaw* Shaw_3 = new Shaw(2,0,1,0);
	
	Shaw* Shaw_4 = new Shaw(2,0,0,1);
	
	Shift* Shift_2 = new Shift(2);
	
	Shift* Shift_4 = new Shift(4);
	
	Swap* Swap_1_1 = new Swap(1,1);
	
	Swap* Swap_2_1 = new Swap(2,1);
	
	Swap* Swap_2_2 = new Swap(2,2);
	
	// Vetor que guardará os métodos de perturbação já inicializados
	std::vector<Perturbation*> PerturbationProcedures {Random, Worst, Concentric};
	
	// Vetor que guardará os operadores de busca local já inicializados
	std::vector<LocalSearchOperator*> LSOperators {
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
	AILSObject.executarAILS(
					max_it, // max_it: Número máximo de iterações do algoritmo
					max_it_sem_melhoria, // max_it_sem_melhoria: Número máximo de iterações sem melhoria
					it_RRH_intervalo, // it_RRH_intervalo: Intervalo de iterações para aplicação de route reduction heuristic
					it_RRH, // it_RRH: Número iterações da route reduction heuristic a cada intervalo
					600 // max_t: tempo máximo de execução do algoritmo
	);
	
	// auto end = std::chrono::high_resolution_clock::now();
	// auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
	
	// std::cout << "\n" << instancia << ";" << AILSObject.S_p.FO() << ";" << elapsed.count() * 1e-9 << std::endl;
	
	std::cout << AILSObject.S_p.calcularFO() << std::endl;
	
}
*/


// Init normal, para ensaios computacionais

// /*

int main(){
	
	// Parâmetros de controle dos critérios de parada
	
	int max_it = 12000; // Número máximo de iterações do algoritmo
	int max_it_sem_melhoria = 5000; // Número máximo de iterações sem melhoria
	int it_RRH_intervalo = 125; // it_RRH_intervalo: Intervalo de iterações para aplicação de route reduction heuristic
	int it_RRH = 40; // it_RRH: Número iterações da route reduction heuristic a cada intervalo
	int max_t = 3600; // Tempo máximo de execução do algoritmo
	int iteracoes_por_instancia = 5;
	
	// Parâmetros da meta-heurística
	
	long long seed = 1382364237;
	double eta = 0.7;
	double kappa = 0.80;
	double gamma = 25;
	int d_b = 16;
	double noise = 0.07;
	double alpha = 0.03;
	
	// Para ter controle sobre os outputs
	srand(seed);
	
	// Arquivo de saída
	std::string output = "output.txt";
	
	std::ofstream output_file(output, std::ios::app);
	
	// Escrevendo parâmetros
	
	output_file << "Parametros de controle dos criterios de parada\n\n";
	
	output_file << "max_it : " << max_it << std::endl;
	output_file << "max_it_no_improv : " << max_it_sem_melhoria << std::endl;
	output_file << "it_RRH_intervalo : " << it_RRH_intervalo << std::endl;
	output_file << "it_RRH : " << it_RRH << std::endl;
	
	output_file << "\n\nParametros da meta-heuristica\n\n";
	
	output_file << "seed : " << seed << std::endl;
	output_file << "eta : " << eta << std::endl;
	output_file << "kappa : " << kappa << std::endl;
	output_file << "gamma : " << gamma << std::endl;
	output_file << "d_b : " << d_b << std::endl;
	output_file << "noise : " << noise << std::endl;
	output_file << "alpha : " << alpha << "\n\n\n";
	
	
	// Inicializando métodos de perturbação:
	
	RandomRemoval* Random = new RandomRemoval;
	
	WorstRemoval* Worst = new WorstRemoval(3);
	
	ConcentricRemoval* Concentric = new ConcentricRemoval;
	
	// Inicializando operadores de busca local:
	
	OrOpt* Or_opt_1 = new OrOpt(1);
	
	OrOpt* Or_opt_2 = new OrOpt(2);
	
	Shaw* Shaw_1 = new Shaw(2,0.3,0.4,0.3);
	
	Shift* Shift_2 = new Shift(2);
	
	Shift* Shift_4 = new Shift(4);
	
	Swap* Swap_1_1 = new Swap(1,1);
	
	Swap* Swap_2_1 = new Swap(2,1);
	
	Swap* Swap_2_2 = new Swap(2,2);
	
	// Vetor que guardará os métodos de perturbação já inicializados
	std::vector<Perturbation*> PerturbationProcedures {Random, Worst, Concentric};
	
	// Vetor que guardará os operadores de busca local já inicializados
	std::vector<LocalSearchOperator*> LSOperators {
										Or_opt_1,
										Or_opt_2,
										Shaw_1,
										Shift_2,
										Shift_4,
										Swap_1_1,
										Swap_2_1,
										Swap_2_2,
									};
	
	// Caminho para as instância:
	std::string path = "instances/li_lim/400/";
	
	// Instâncias
	
	std::vector<std::string> instancias = {
		
		//"AA5",
		//"AA10",
		//"AA15",
		//"AA20",
		//"AA25",
		//"AA30",
		//"AA35",
		//"AA40",
		//"AA45",
		//"AA50",
		//"AA55",
		//"AA60",
		//"AA65",
		//"AA70",
		//"AA75",
		//"BB5",
		//"BB10",
		//"BB15",
		//"BB20",
		//"BB25",
		//"BB30",
		//"BB35",
		//"BB40",
		//"BB45",
		//"BB50",
		//"BB55",
		//"BB60",
		// "BB65",
		//"BB70",
		//"BB75",
		//"CC5",
		//"CC10",
		//"CC15",
		//"CC20",
		// "CC25",
		//"CC30",
		//"CC35",
		//"CC40",
		//"CC45",
		//"CC50",
		//"CC55",
		// "CC60",
		// "CC65",
		//"CC70",
		//"CC75",
		//"DD5",
		//"DD10",
		//"DD15",
		//"DD20",
		//"DD25",
		//"DD30",
		//"DD35",
		//"DD40",
		//"DD45",
		// "DD50",
		//"DD55",
		//"DD60",
		//"DD65",
		// "DD70",
		// "DD75",
		//"LR2_4_4.txt",
		"LRC2_4_2.txt"
	};
	
	
	// Executando algoritmo
	
	for (auto instancia: instancias){
		
		for (int i {0}; i < iteracoes_por_instancia; i++){
			
			// Abrindo arquivo de saída
			// Modo append
			std::ofstream output_file(output, std::ios::app);
			
			// Medindo tempo
			auto begin = std::chrono::high_resolution_clock::now();
			
			std::string path_to_instance = path + instancia;
			
			// Objeto instância
			Instance inst(path_to_instance);
			
			// Inicializando objeto solução;
			Sol S(inst);
			
			// Inicializando objeto da AILS
			AILS AILSObject(S, // Solução inicial
							LSOperators, // Vetor com operadores de busca local
							PerturbationProcedures, // Vetor com métodos de perturbação
							eta, // eta: Determina b_UP no critério de aceitação
							kappa, // kappa: Porcentagem de soluções aceitas
							gamma, // Gamma: Determina quantas iterações cada heurística de perturbação realizará com um mesmo "peso"
							d_b, // d_b: Distância de referência ("ideal") entre soluções
							noise, // noise: Utilizado no cálculo do ruído aleatório
							alpha // alpha: Probabilidade de aplicação do ruído aleatório
							);
			
			
			// Executando algoritmo
			AILSObject.executarAILS(
							max_it, // max_it: Número máximo de iterações do algoritmo
							max_it_sem_melhoria, // max_it_sem_melhoria: Número máximo de iterações sem melhoria
							it_RRH_intervalo, // it_RRH_intervalo: Intervalo de iterações para aplicação de route reduction heuristic
							it_RRH, // it_RRH: Número iterações da route reduction heuristic a cada intervalo
							max_t // max_t: tempo máximo de execução do algoritmo
							);
			
			auto end = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
			
			
			// Printando output
			std::cout << instancia << ";" << std::setprecision(7) << AILSObject.S_p.calcularFO() << ";" << AILSObject.S_p.checarFactibilidadeSolucao() << ";" << AILSObject.S_p.rotas.size() << ";" << elapsed.count() * 1e-9 << std::endl;
			
			// Escrevendo output no arquivo
			output_file << instancia << ";" << std::setprecision(7) << AILSObject.S_p.calcularFO() << ";" << AILSObject.S_p.checarFactibilidadeSolucao() << ";" << AILSObject.S_p.rotas.size() << ";" << elapsed.count() * 1e-9 << std::endl;

			// Fechando arquivo
			output_file.close();
			
			
			
		}
	}
	
	
	return 0;
	
}

// */