#include <iostream>
#include <vector>
#include <Instance.hpp>
#include <Sol.hpp>
#include <LocalSearchOperator.hpp>
#include <AILS.hpp>
#include <heuristicsFunctions.h>
#include <Perturbation.hpp>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <thread>
#include <random>

using namespace std;

// Função para printar vetores: debug

void print(std::vector<double> const &input)
{
    for (auto const &i: input) {
        cout << i << " ";
    }
	
	cout << endl;
	
}


int main(){
		
		// Para gerar números aleatórios
		// srand(time(NULL));
		
		
		// Para ter controle sobre os outputs
		srand(125);
		
		
		/*
		
		std::vector<std::string> instancias = {
			
			
			"instances/CC50",
			"instances/DD50",
			
			};
		
		// Número de iterações para debug
		int iteracoes = 25000;
		
		// Métodos de perturbação
		Perturbation Random('R');
		
		Perturbation Worst('W');
		
		Perturbation Concentric('C');
		
		// Métodos de busca local
		LocalSearchOperator Or_opt_1('O', 1);
		
		LocalSearchOperator Or_opt_2('O', 2);
		
		LocalSearchOperator Shaw_1('H', 2,0.3,0.4,0.3);
		
		LocalSearchOperator Shaw_2('H', 2,0,0,1);
		
		LocalSearchOperator Shift_2('T', 2);
		
		LocalSearchOperator Shift_4('T', 4);
		
		LocalSearchOperator Shift_5('T', 5);
		
		LocalSearchOperator Shift_6('T', 6);
		
		LocalSearchOperator Swap_1_1('S', 1,1);
		
		LocalSearchOperator Swap_2_1('S', 2,1);
		
		LocalSearchOperator Swap_2_2('S', 2,2);
		
		
		std::vector<LocalSearchOperator> operators = {
							Or_opt_1,
							Or_opt_2,
							Shaw_1,
							Shaw_2,
							Shift_2,
							Shift_4,
							Shift_5,
							Shift_6,
							Swap_1_1,
							Swap_2_1,
							Swap_2_2,
							};
		
		
		for (auto instancia: instancias){
			
			// Medindo tempo
			auto begin = std::chrono::high_resolution_clock::now();
			
			std::cout << "\nInstancia: " << instancia << std::endl;
			
			// Inicializando solução
			
			Instance inst;
			
			inst.read(instancia);
			
			Sol S(inst);
			
			Sol S_best(inst);
			
			// Aplicando métodos iterativamente
			
			for (int i {0}; i < iteracoes; i++){
				
				// S = S_best;
				
				if (i%1000 == 0){
					
					std::cout << "Iteracao " << i << std::endl;
					
				}
				
				// Aplicando método de perturbação
				if(i % 2 == 0){
					
					Random.apply(S, 6);
					
					
				} else {
					
					Worst.apply(S, 8);
					
				}
				
				// Aplicando método de busca local
				for (auto op: operators){
					
					op.apply(S);
					
					if (S.FO() < S_best.FO()){
						
						S_best = S;
						
					}
					
				}
				
				
			}
			
			S_best.print_sol();
			auto end = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
			
			cout << "\n FO: " << std::setprecision(7) << S_best.FO() << endl;
			std::cout << "\n Tempo: " << elapsed.count() * 1e-9 << std::endl;
			
		}
		
		
		
		*/
		
		// /*
		
		std::vector<std::string> instancias = {
			
			"instances/AA5",
			"instances/AA10",
			"instances/AA15",
			"instances/AA20",
			"instances/AA25",
			"instances/AA30",
			"instances/AA35",
			"instances/AA40",
			"instances/AA45",
			"instances/AA50",
			"instances/AA55",
			"instances/AA60",
			"instances/AA65",
			"instances/AA70",
			"instances/AA75",
			"instances/BB5",
			"instances/BB10",
			"instances/BB15",
			"instances/BB20",
			"instances/BB25",
			"instances/BB30",
			"instances/BB35",
			"instances/BB40",
			"instances/BB45",
			"instances/BB50",
			"instances/BB55",
			"instances/BB60",
			"instances/BB65",
			"instances/BB70",
			"instances/BB75",
			"instances/CC5",
			"instances/CC10",
			"instances/CC15",
			"instances/CC20",
			"instances/CC25",
			"instances/CC30",
			"instances/CC35",
			"instances/CC40",
			"instances/CC45",
			"instances/CC50",
			"instances/CC55",
			"instances/CC60",
			"instances/CC65",
			"instances/CC70",
			"instances/CC75",
			"instances/DD5",
			"instances/DD10",
			"instances/DD15",
			"instances/DD20",
			"instances/DD25",
			"instances/DD30",
			"instances/DD35",
			"instances/DD40",
			"instances/DD45",
			"instances/DD50",
			"instances/DD55",
			"instances/DD60",
			"instances/DD65",
			"instances/DD70",
			"instances/DD75",
		};
		
		for (auto instancia: instancias){
			
			for (int i {0}; i < 5; i++){
				
				// double a;
				
				// std::cout << "A: ";
				
				// std::cin >> a;
				
				try{
					
					// Medindo tempo
					auto begin = std::chrono::high_resolution_clock::now();
					
					Instance inst;
					
					inst.read(instancia);
					
					// Inicializando solução a partir do objeto instância:
					Sol S(inst);
					
					// S.print_sol();
					
					// Inicializando objeto da AILS
					
					
					AILS AILSObject;
					
					// Solução incumbente e melhor solução encontrada:
					
					AILSObject.S_p = S;
					
					AILSObject.S_r = S;
					
					// Inicializando métodos de perturbação:
					
					Perturbation Random('R');
					
					Perturbation Worst('W');
					
					Perturbation Concentric('C');
					
					// Inicializando operadores de busca local:
					
					LocalSearchOperator Or_opt_1('O', 1);
					
					LocalSearchOperator Or_opt_2('O', 3);
					
					LocalSearchOperator Two_Opt('W');
					
					LocalSearchOperator Shaw_1('H', 2,0.3,0.4,0.3);
					
					LocalSearchOperator Shaw_2('H', 2,0,0,1);
					
					LocalSearchOperator Shift_2('T', 2);
					
					LocalSearchOperator Shift_4('T', 4);
					
					LocalSearchOperator Swap_1_1('S', 1,1);
					
					LocalSearchOperator Swap_2_1('S', 2,1);
					
					LocalSearchOperator Swap_2_2('S', 2,2);
					
					
					// Atribuindo ao objeto da AILS
					
					AILSObject.PerturbationProcedures = {Random, Worst, Concentric};
					
					AILSObject.LSOperators = {
								Or_opt_1,
								Or_opt_2,
								Shaw_1,
								Shaw_2,
								Shift_2,
								Shift_4,
								Swap_1_1,
								Swap_2_1,
								Swap_2_2,
								};
					
					
					AILSObject.executeAILS(2500);
					
					// AILSObject.S_p.print_sol();
					
					auto end = std::chrono::high_resolution_clock::now();
					auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
					
					// cout << "\n FO: " << std::setprecision(7) << AILSObject.S_p.FO() << endl;
					std::cout << "\n" << instancia << ";" << AILSObject.S_p.FO() << ";" << elapsed.count() * 1e-9 << std::endl;
				}
				
				catch(...){
					
					std::cout << "\n\nErro: instancia " << instancia << "\n\n" << std::endl; 
					break;
				}
			}
		}
		
		
		// */
		
		return 0;
}