#include "Sol.hpp"
#include "Instance.hpp"
#include <ctime>
#include <algorithm>
#include <stdio.h>
#include <chrono>
#include "Perturbation.hpp"

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
			
			while (n_pedidos_removidos < n_requests){
				
				double pedido_aleatorio = S.A.at(rand()%S.A.size() + 1);
				
				std::cout << pedido_aleatorio << std::endl;
				
				// Se o pedido não estiver no vetor de pedidos a serem removidos e nem nos pedidos ausentes (L), ele pode ser escolhido
				//if ((!count(pedidos_a_remover.begin(), pedidos_a_remover.end(), pedido_aleatorio)) && (!count(S.L.begin(), S.L.end(), pedido_aleatorio))){
					
				S.remover_pedido(pedido_aleatorio);
				
				n_pedidos_removidos += 1;
					
				//}
				
			}
			
			// Inserindo pedidos com método de primeira inserção aleatória
			
			
			// Fim da heurística random removal
			break;
			
		}
		
		default:
		std::cout << "Invalido" << std::endl;
			
	}
	
	// if S.isfeasible // se a mudança melhorou a função objetivo
	
	return S;
		
}

