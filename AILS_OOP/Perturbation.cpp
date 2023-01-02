#include "Sol.hpp"
#include "Instance.hpp"
#include <ctime>
#include <algorithm>
#include <stdio.h>
#include <chrono>
#include "Perturbation.hpp"
#include "heuristicsFunctions.h"
#include <numeric>

//Perturbation::Perturbation()
//{
//}

Perturbation::~Perturbation()
{
}

// Método principal das perturbações

// Método de aplicação das perturbações
// *OBS: vou colocar um switch case por enquanto, mas certamente existe um jeito melhor de fazer isso...

Sol Perturbation::apply(Sol &S, int n_requests){
	
	switch (name){
		
		// Random removal + Primeira Inserção Factível Aleatória
		case 'R':{
			
			std::cout << "Solucao apos random removal" << std::endl;
			
			srand(time(NULL));
			
			// Variável com quantidade de pedidos removidos
			int n_pedidos_removidos {0};
			
			// Retirando pedidos da solução iterativamente
			while (n_pedidos_removidos < n_requests){
				
				double pedido_aleatorio = S.A.at(rand()%S.A.size());
				
				S.remover_pedido(pedido_aleatorio);
				
				n_pedidos_removidos += 1;
				
			}
			
			S.print_sol();
			
			// Inserindo pedidos com método de primeira inserção factível aleatória (sem critérios para a ordem dos pedidos)
			
			// Pedidos não inseridos na solução (referenciar diretamente no for loop deu um bug)
			
			std::vector<double> pedidos_nao_inseridos = S.L;
			
			for (auto pedido: pedidos_nao_inseridos){
				
				// std::cout << "\nPedido: " << pedido << std::endl;
				
				S = primeira_insercao_factivel(S, pedido);
				
			}
			
			std::cout << "Solucao apos insercoes" << std::endl;
			
			S.print_sol();
			
			
			// Fim da heurística random removal
			break;
			
		}
		
		default:
		std::cout << "Invalido" << std::endl;
			
	}
	
	// if S.isfeasible // se a mudança melhorou a função objetivo
	
	return S;
		
}

