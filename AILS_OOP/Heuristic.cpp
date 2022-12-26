#include "Heuristic.hpp"
#include "Sol.hpp"
#include "Instance.hpp"
#include <ctime>
#include <algorithm>
#include <stdio.h>
#include <chrono>

// Heuristic::Heuristic()
// {
// }

Heuristic::~Heuristic()
{
}

// Funções utilizadas ao longo da implementação das heurísticas:

// Incremento por inserção: calcula o custo de inserção de um pedido em posições pré-determinadas e sem checar factibilidade!
double delta_FO_ins(Sol &S_ins, double &pedido, int &index_rota, int &pos_no_pickup, int &pos_no_delivery){
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + S_ins.inst.n};
	
	// Objeto para a rota que potencialmente sofrerá a inserção
	// std::vector<double> novaRota = S_ins.Rotas[index_rota];
	
	if (pos_no_delivery == pos_no_pickup + 1){
		
		// Variação total
		// double delta = (S_ins.inst.t[(novaRota[pos_no_pickup - 1])][(no_pickup)]) + (S_ins.inst.t[(no_pickup)][(no_delivery)]) + (S_ins.inst.t[(no_delivery)][(novaRota[pos_no_delivery - 1])]) - (S_ins.inst.t[(novaRota[pos_no_pickup - 1])][(novaRota[pos_no_delivery - 1])]);
		double delta = (S_ins.inst.t[(S_ins.Rotas[index_rota][pos_no_pickup - 1])][(no_pickup)]) + (S_ins.inst.t[(no_pickup)][(no_delivery)]) + (S_ins.inst.t[(no_delivery)][(S_ins.Rotas[index_rota][pos_no_delivery - 1])]) - (S_ins.inst.t[(S_ins.Rotas[index_rota][pos_no_pickup - 1])][(S_ins.Rotas[index_rota][pos_no_delivery - 1])]);
		
		return delta;
		
	}else{
		
		
		// Variação pela inserção do nó de pickup
		// double delta_pickup = (S_ins.inst.t[(novaRota[pos_no_pickup - 1])][(no_pickup)]) + (S_ins.inst.t[(no_pickup)][(novaRota[pos_no_pickup])]) - (S_ins.inst.t[(novaRota[pos_no_pickup - 1])][(novaRota[pos_no_pickup])]);
		
		// Variação pela inserção do nó de delivery
		// double delta_delivery = (S_ins.inst.t[(novaRota[pos_no_delivery - 1])][(no_delivery)]) + (S_ins.inst.t[(no_delivery)][(novaRota[pos_no_delivery - 2])]) - (S_ins.inst.t[(novaRota[pos_no_delivery - 1])][(novaRota[pos_no_delivery - 2])]);
		
		// Variação pela inserção do nó de pickup
		double delta_pickup = (S_ins.inst.t[(S_ins.Rotas[index_rota][pos_no_pickup - 1])][(no_pickup)]) + (S_ins.inst.t[(no_pickup)][(S_ins.Rotas[index_rota][pos_no_pickup])]) - (S_ins.inst.t[(S_ins.Rotas[index_rota][pos_no_pickup - 1])][(S_ins.Rotas[index_rota][pos_no_pickup])]);
		
		// Variação pela inserção do nó de delivery
		double delta_delivery = (S_ins.inst.t[(S_ins.Rotas[index_rota][pos_no_delivery - 1])][(no_delivery)]) + (S_ins.inst.t[(no_delivery)][(S_ins.Rotas[index_rota][pos_no_delivery - 2])]) - (S_ins.inst.t[(S_ins.Rotas[index_rota][pos_no_delivery - 1])][(S_ins.Rotas[index_rota][pos_no_delivery - 2])]);
		
		// Variação total
		double delta {delta_pickup + delta_delivery};
		
		return delta;
		
	}
	
}

// Decréscimo por remoção: calcula o decréscimo na FO pela remoção de um pedido
double delta_FO_rem(Sol &S_rem, double &pedido){
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + S_rem.inst.n};
	
	// Achando índice da rota que contém o pedido
	int index_rota {0};
	
	for (auto &rota: S_rem.Rotas){
		
		if (count(rota.begin(), rota.end(), no_pickup)){
			
			break;
			
		}else{
			index_rota += 1;
		}
		
	}
	
	// Objeto para a rota que potencialmente sofrerá a remoção
	// std::vector<double> novaRota = S_rem.Rotas[index_rota];
	
	// Índice (posição) do nó de pickup na rota
	// double pos_no_pickup = std::find(novaRota.begin(),novaRota.end(), no_pickup) - novaRota.begin();
	
	// Índice (posição) do nó de delivery na rota
	// double pos_no_delivery = std::find(novaRota.begin(),novaRota.end(), no_delivery) - novaRota.begin();
	
	// Índice (posição) do nó de pickup na rota
	double pos_no_pickup = std::find(S_rem.Rotas[index_rota].begin(),S_rem.Rotas[index_rota].end(), no_pickup) - S_rem.Rotas[index_rota].begin();
	
	// Índice (posição) do nó de delivery na rota
	double pos_no_delivery = std::find(S_rem.Rotas[index_rota].begin(),S_rem.Rotas[index_rota].end(), no_delivery) - S_rem.Rotas[index_rota].begin();
	
	
	// Calculando decréscimo nos custos
	
	if (pos_no_delivery == pos_no_pickup + 1){
		
		// std::cout << novaRota[pos_no_pickup - 1] << " " << no_pickup << " " << novaRota[pos_no_delivery + 1] << " " << no_delivery << "\n";
		
		// Variação total
		// double delta = (S_rem.inst.t[(novaRota[pos_no_pickup - 1])][(no_pickup)]) + (S_rem.inst.t[(no_pickup)][(no_delivery)]) + (S_rem.inst.t[(no_delivery)][(novaRota[pos_no_delivery + 1])]) - (S_rem.inst.t[(novaRota[pos_no_pickup - 1])][(novaRota[pos_no_delivery + 1])]);
		
		double delta = (S_rem.inst.t[(S_rem.Rotas[index_rota][pos_no_pickup - 1])][(no_pickup)]) + (S_rem.inst.t[(no_pickup)][(no_delivery)]) + (S_rem.inst.t[(no_delivery)][(S_rem.Rotas[index_rota][pos_no_delivery + 1])]) - (S_rem.inst.t[(S_rem.Rotas[index_rota][pos_no_pickup - 1])][(S_rem.Rotas[index_rota][pos_no_delivery + 1])]);
		
		
		return delta;
		
	}else{
		
		// std::cout << novaRota[pos_no_pickup - 1] << " " << no_pickup << " " << novaRota[pos_no_pickup + 1] << " " << novaRota[pos_no_delivery -1] << " " << no_delivery << " " << novaRota[pos_no_delivery + 1] << "\n";
		//// Variação pela inserção do nó de pickup
		// double delta_pickup = (S_rem.inst.t[(novaRota[pos_no_pickup - 1])][(no_pickup)]) + (S_rem.inst.t[(no_pickup)][(novaRota[pos_no_pickup + 1])]) - (S_rem.inst.t[(novaRota[pos_no_pickup - 1])][(novaRota[pos_no_pickup + 1])]);
		
		//// Variação pela inserção do nó de delivery
		// double delta_delivery = (S_rem.inst.t[(novaRota[pos_no_delivery - 1])][(no_delivery)]) + (S_rem.inst.t[(no_delivery)][(novaRota[pos_no_delivery + 1])]) - (S_rem.inst.t[(novaRota[pos_no_delivery - 1])][(novaRota[pos_no_delivery + 1])]);
		
		//// Variação pela inserção do nó de pickup
		double delta_pickup = (S_rem.inst.t[(S_rem.Rotas[index_rota][pos_no_pickup - 1])][(no_pickup)]) + (S_rem.inst.t[(no_pickup)][(S_rem.Rotas[index_rota][pos_no_pickup + 1])]) - (S_rem.inst.t[(S_rem.Rotas[index_rota][pos_no_pickup - 1])][(S_rem.Rotas[index_rota][pos_no_pickup + 1])]);
		
		//// Variação pela inserção do nó de delivery
		double delta_delivery = (S_rem.inst.t[(S_rem.Rotas[index_rota][pos_no_delivery - 1])][(no_delivery)]) + (S_rem.inst.t[(no_delivery)][(S_rem.Rotas[index_rota][pos_no_delivery + 1])]) - (S_rem.inst.t[(S_rem.Rotas[index_rota][pos_no_delivery - 1])][(S_rem.Rotas[index_rota][pos_no_delivery + 1])]);
		
		//// Variação total
		double delta {delta_pickup + delta_delivery};
		
		return delta;
		
	}
	
}

// Retorna um vetor com o delta de melhor inserção, rota, posição de nó pickup e delivery correspondentes ao pedido passado
std::vector<double> delta_melhor_insercao(Sol &S, double &pedido){
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + S.inst.n};
	
	// Variável que controlará o número de rotas factíveis encontradas
	int num_rotas_factiveis {0};
	
	// Delta mínimo pela inserção do pedido da iteração
	double delta_min {9999};
	
	// Posição de inserção de nó de pickup com delta mínimo
	int pos_insercao_no_pickup_min {};
	
	// Posição de inserção de nó de delivery com delta mínimo
	int pos_insercao_no_delivery_min {};
	
	// Índice da rota com o menor valor de inserção
	int index_rota_min {};
	
	// Vetor a ser retornado pela função:
	std::vector<double> return_vector {};
	
	// Realizando inserções
	for (auto index_rota {0}; index_rota < S.Rotas.size(); index_rota++){
		
		for (auto pos_insercao_no_pickup {1}; pos_insercao_no_pickup < S.Rotas[index_rota].size() + 1; pos_insercao_no_pickup++){
			
			for (auto pos_insercao_no_delivery {1}; pos_insercao_no_delivery < S.Rotas[index_rota].size() + 1; pos_insercao_no_delivery++){
				
				// Testando apenas índices de inserção válidos: índice de delivery maior do que o de pickup (precedence) e diferente dele!
				// A iteração começa em 1 e termina no tamanho da rota porque não se considera a primeira e última posição da rota, que são o depósito
				if ((pos_insercao_no_pickup != pos_insercao_no_delivery) and (pos_insercao_no_pickup < pos_insercao_no_delivery)){
					
					// Criando cópia do objeto, para testar inserção (achar um jeito melhor de fazer isso!)
					Sol S_teste = S;
					
					// Inserindo nós na rota, nas posições da iteração
					S_teste.inserir_pedido(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
					
					if (S_teste.isFeasible(index_rota)){
						
						num_rotas_factiveis += 1;
						
						double delta_S = delta_FO_ins(S, pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
						
						if (delta_S < delta_min){
							
							delta_min = delta_S;
							
							pos_insercao_no_pickup_min = pos_insercao_no_pickup;
							
							pos_insercao_no_delivery_min = pos_insercao_no_delivery;
							
							index_rota_min = index_rota;
							
							
						}
						
						
					}
					
				}
			}		
		}			
	}
	
	//return_vector = {delta_min, pos_insercao_no_pickup_min, pos_insercao_no_delivery_min, index_rota_min};
	
	if (num_rotas_factiveis > 0){
		
		return_vector = {delta_min, index_rota_min, pos_insercao_no_pickup_min, pos_insercao_no_delivery_min};
		
	} else {
		
		return_vector = {99999, 0, 0,0};
		
		//return 99999;
		
	}
	
	return return_vector;
	
	//return delta_min;
	
}

// Delta melhor inserção considerando rota (regret insertion):
// Retorna um vetor com o delta de melhor inserção, rota, posição de nó pickup e delivery correspondentes ao pedido passado
std::vector<double> delta_melhor_insercao(Sol &S, double &pedido, int &index_rota){
	
	// Criando uma cópia do objeto solução:
	// Sol S(Rotas, L, A, inst);
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + S.inst.n};
	
	// Variável que controlará o número de rotas factíveis encontradas
	int num_rotas_factiveis {0};
	
	// Delta mínimo pela inserção do pedido da iteração
	double delta_min {9999};
	
	// Posição de inserção de nó de pickup com delta mínimo
	int pos_insercao_no_pickup_min {};
	
	// Posição de inserção de nó de delivery com delta mínimo
	int pos_insercao_no_delivery_min {};
	
	// Índice da rota com o menor valor de inserção
	int index_rota_min {};
	
	// Vetor a ser retornado pela função:
	std::vector<double> return_vector {};
	
	// Realizando inserções
		
	for (auto pos_insercao_no_pickup {1}; pos_insercao_no_pickup < S.Rotas[index_rota].size() + 1; pos_insercao_no_pickup++){
		
		for (auto pos_insercao_no_delivery {1}; pos_insercao_no_delivery < S.Rotas[index_rota].size() + 1; pos_insercao_no_delivery++){
			
			// Testando apenas índices de inserção válidos: índice de delivery maior do que o de pickup (precedence) e diferente dele!
			// A iteração começa em 1 e termina no tamanho da rota porque não se considera a primeira e última posição da rota, que são o depósito
			if ((pos_insercao_no_pickup != pos_insercao_no_delivery) and (pos_insercao_no_pickup < pos_insercao_no_delivery)){
				
				// Criando cópia do objeto, para testar inserção (achar um jeito melhor de fazer isso!)
				Sol S_teste = S;
				
				// Inserindo nós na rota, nas posições da iteração
				S_teste.inserir_pedido(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
				
				if (S_teste.isFeasible(index_rota)){
					
					num_rotas_factiveis += 1;
					
					// double delta_S = S.delta_FO_ins(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
					double delta_S = delta_FO_ins(S, pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
					
					if (delta_S < delta_min){
						
						delta_min = delta_S;
						
						pos_insercao_no_pickup_min = pos_insercao_no_pickup;
							
						pos_insercao_no_delivery_min = pos_insercao_no_delivery;
							
						index_rota_min = index_rota;
						
						
					}
					
					
				}
				
			}
		}		
	}			
	
	if (num_rotas_factiveis > 0){
		
		return_vector = {delta_min, index_rota_min, pos_insercao_no_pickup_min, pos_insercao_no_delivery_min};
		
		
	} else {
		
		// Possível problema em retornar um valor grande: se não for factível, como busca-se maximizar (regret), vai dar problema!
		
		return_vector = {0, 0, 0,0};
		
		
	}
	
	return return_vector;
	
}

Sol melhor_insercao(Sol &S_in,double &pedido){
	
	// Criando uma cópia do objeto solução (construtiva deu um bug inexplicável se não fizesse isso)
	Sol S(S_in.Rotas, S_in.L, S_in.A, S_in.inst);
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + S.inst.n};
	
	// Variável que controlará o número de rotas factíveis encontradas
	int num_rotas_factiveis {0};
	
	// Delta mínimo pela inserção do pedido da iteração
	double delta_min {9999};
	
	// Posição de inserção de nó de pickup com delta mínimo
	int pos_insercao_no_pickup_min {};
	
	// Posição de inserção de nó de delivery com delta mínimo
	int pos_insercao_no_delivery_min {};
	
	// Índice da rota com o menor valor de inserção
	int index_rota_min {};
	
	// Realizando inserções
	for (auto index_rota {0}; index_rota < S.Rotas.size(); index_rota++){
		
		for (auto pos_insercao_no_pickup {1}; pos_insercao_no_pickup < S.Rotas[index_rota].size() + 1; pos_insercao_no_pickup++){
			
			for (auto pos_insercao_no_delivery {1}; pos_insercao_no_delivery < S.Rotas[index_rota].size() + 1; pos_insercao_no_delivery++){
				
				// Testando apenas índices de inserção válidos: índice de delivery maior do que o de pickup (precedence) e diferente dele!
				// A iteração começa em 1 e termina no tamanho da rota porque não se considera a primeira e última posição da rota, que são o depósito
				if ((pos_insercao_no_pickup != pos_insercao_no_delivery) and (pos_insercao_no_pickup < pos_insercao_no_delivery)){
					
					// Criando cópia do objeto, para testar inserção
					Sol S_teste = S;
					
					// Inserindo nós na rota, nas posições da iteração
					S_teste.inserir_pedido(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
					
					if (S_teste.isFeasible(index_rota)){
						
						num_rotas_factiveis += 1;
						
						double delta_S = delta_FO_ins(S, pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
						
						if (delta_S < delta_min){
							
							delta_min = delta_S;
							
							pos_insercao_no_pickup_min = pos_insercao_no_pickup;
							
							pos_insercao_no_delivery_min = pos_insercao_no_delivery;
							
							index_rota_min = index_rota;
							
							
						}
						
						
					}
					
				}
			}		
		}			
	}
	
	if (num_rotas_factiveis > 0){
		
		S.inserir_pedido(pedido, index_rota_min, pos_insercao_no_pickup_min, pos_insercao_no_delivery_min);
		
	}
	
	return S;
	
}

// Fim das funções utilizadas ao longo da implementação das heurísticas

// Método principal das heurísticas

// Método de aplicação das heurísticas
// *OBS: vou colocar um switch case por enquanto, mas certamente existe um jeito melhor de fazer isso...

Sol Heuristic::apply(Sol &S){
	
	switch (name){
		
		// Heurística construtiva
		case 'C':{
			
			std::cout << "Solucao apos heuristica construtiva" << std::endl;
			
			/* Criando variáveis com valores atualizados a cada inserção */
			// Inserindo no conjunto L os pedidos não atendidos
			for(double value = 1; value < S.inst.n + 1; value++){
				
				S.L.push_back(value);
			}
			
			// Criando uma rota inicial vazia para a solução:
			S.Rotas.push_back({0, 2*(S.inst.n) + 1});
			
			// Quantidade de requests atendidos (inicia-se em 0)
			int qtd_atendidos {0};
			
			// Início do algoritmo de inserção:
			while (qtd_atendidos < S.inst.n){
				
				// Variável que abrigará o request a ser inserido na iteração
				double pedido {S.L.at(0)};
				
				// Objeto solução, que tentará fazer melhor inserção
				Sol S_best = melhor_insercao(S, pedido);
				
				// Caso o pedido tenha sido inserido:
				if (S_best.L.size() != S.L.size()){
					
					S = S_best;
					
				// Caso não seja possível fazer a inserção, isso significará que não foram encontradas posições de inserção factíveis para as rotas em questão
				// fazendo-se necessária uma nova rota
				
				} else {
					
					// Rota vazia com os nós da iteração
					std::vector <double> nova_rota {0, pedido, pedido + S.inst.n, 2*(S.inst.n) + 1};
					S.Rotas.push_back(nova_rota);
					
					// Removendo pedido de L
					S.L.erase(S.L.begin());
					
					// Adicionando pedido em A
					S.A.push_back(pedido);
					
				}
				
				// Atualizando quantidade de pedidos atendidos
				qtd_atendidos += 1;
				
				
			}
			// Fim da heurística construtiva
			
			break;
		}
			default:
			std::cout << "Invalido" << std::endl;
			
	}
	
	return S;
	
	
}
