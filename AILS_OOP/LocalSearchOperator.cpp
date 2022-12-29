#include "LocalSearchOperator.hpp"
#include "Sol.hpp"
#include "Instance.hpp"
#include <ctime>
#include <algorithm>
#include <stdio.h>
#include <chrono>


//LocalSearchOperator::LocalSearchOperator()
//{
//}

LocalSearchOperator::~LocalSearchOperator()
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

// Melhor inserção considerando todas as rotas:

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

// Melhor inserção considerando uma rota específica

Sol melhor_insercao(Sol &S_in,double &pedido, int index_rota){
	
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
	//for (auto index_rota {0}; index_rota < S.Rotas.size(); index_rota++){
		
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
	//}
	
	if (num_rotas_factiveis > 0){
		
		S.inserir_pedido(pedido, index_rota_min, pos_insercao_no_pickup_min, pos_insercao_no_delivery_min);
		
	}
	
	return S;
	
}

// Fim das funções utilizadas ao longo da implementação das heurísticas

// Método principal das heurísticas

// Método de aplicação das heurísticas
// *OBS: vou colocar um switch case por enquanto, mas certamente existe um jeito melhor de fazer isso...

Sol LocalSearchOperator::apply(Sol &S){
	
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
		
		// Estrutura de busca local: Swap
		
		case 'S':{
			
			// Para gerar números aleatórios (índices das rotas e pedidos escolhidos)
			srand(time(NULL));
			
			// Quantidade "m" de rotas na solução:
			int m = S.Rotas.size();
			
			// Escolhendo índice da rota R1:
			double index_R1 = rand()%(m);
			
			// Escolhendo índice da rota R2, necessariamente diferente de R1:
			double index_R2 = rand()%(m);
			
			while (index_R1 == index_R2){
				
				index_R2 = rand()%(m);
				
			}
			
			// Vetor com pedidos retirados da rota R1:
			std::vector<double> pedidosRemovidos_R1 {};
			
			// Vetor com pedidos retirados da rota R2:
			std::vector<double> pedidosRemovidos_R2 {};
			
			std::cout << "\nindex_R1: " << index_R1 << "\nindex_R2: " << index_R2 << std::endl;
			
			// Removendo pedidos:
			
			// Escolhendo aleatoriamente k1 pedidos da rota R1 para serem retirados:
			for (int i {0}; i < k1; i++){
				
				// Número de nós da rota R1 (talvez fique mais rápido contabilizando isso fora do laço e subtraindo 2 a cada remoção);
				int n_nodes = S.Rotas.at(index_R1).size();
				
				// Inicializando pedido a ser removido:
				double pedido {9999};
				
				// Escolhendo pedido: deve ser representado por um nó de pickup, menor ou igual a "n"!
				while (pedido > S.inst.n){
				
					// Índice do pedido a ser removido na rota R1 (a partir do índice 1)
					double index_pedido_removido = 1 + rand()%(n_nodes - 1);
					
					pedido = S.Rotas.at(index_R1).at(index_pedido_removido);
					
				}
				
				std::cout << "\nPedido escolhido: " << pedido << std::endl;
				
				// Removendo pedido de R1:
				
				S.remover_pedido(pedido, index_R1);
				pedidosRemovidos_R1.push_back(pedido);
				
			}
			
			// Escolhendo aleatoriamente k2 pedidos da rota R2 para serem retirados:
			for (int i {0}; i < k2; i++){
				
				// Número de nós da rota R2 (talvez fique mais rápido contabilizando isso fora do laço e subtraindo 2 a cada remoção);
				int n_nodes = S.Rotas.at(index_R2).size();
				
				// Inicializando pedido a ser removido:
				double pedido {9999};
				
				// Escolhendo pedido: deve ser representado por um nó de pickup, menor ou igual a "n"!
				while (pedido > S.inst.n){
				
					// Índice do pedido a ser removido na rota R1 (a partir do índice 1)
					double index_pedido_removido = 1 + rand()%(n_nodes - 1);
					
					pedido = S.Rotas.at(index_R2).at(index_pedido_removido);
					
				}
				
				std::cout << "\nPedido escolhido: " << pedido << std::endl;
				
				// Removendo pedido de R2:
				
				S.remover_pedido(pedido);
				pedidosRemovidos_R2.push_back(pedido);
				
			}
			
			
			std::cout << "\nSolucao apos remocoes: \n" << std::endl;
			
			S.print_sol();
			
			// Inserindo os "k1" pedidos na rota R2, na melhor posição factível:
			for (auto &pedido: pedidosRemovidos_R1){
				
				S = melhor_insercao(S, pedido, index_R2);
				
			}
			
			// Inserindo os "k2" pedidos na rota R2, na melhor posição factível:
			
			// Obs -> inserir na melhor posição considerando todos os pedidos ou apenas a ordem deles na lista?
			
			for (auto &pedido: pedidosRemovidos_R2){
				
				S = melhor_insercao(S, pedido, index_R1);
				
			}
			
			// Teste: inserindo em quaisquer posições os pedidos após tentar trocá-los:
			
			//for (auto &pedido: S.L){
				
			//	S = melhor_insercao(S, pedido);
				
			//}
			
			break;
			
		}
		
		// Estrutura de busca local: Shift
		
		case 'T':{
			
			// Possível ponto de melhoria na heurística:
			
			// Calculando probabilidades de escolha: desejável que, quanto maior a rota, mais provável é sua escolha para ter pedidos removidos!
			
			// *obs: para reduzir o número de rotas, a lógica deve ser a oposta: quanto maior o número de pedidos na rota, menor a chance de ela ser destruída
			
			// Para gerar números aleatórios
			srand(time(NULL));
			
			// Quantidade "m" de rotas na solução:
			int m = S.Rotas.size();
			
			// Escolhendo índice da rota R1, que terá os nós removidos
			double index_R1 = rand()%(m);
			
			// Escolhendo índice da rota R2, necessariamente diferente de R1:
			double index_R2 = rand()%(m);
			
			while (index_R1 == index_R2){
				
				index_R2 = rand()%(m);
				
			}
			
			// Retirando subseção com "k" elementos da rota:
			
			// Número de nós da rota R1
			int n_nodes = S.Rotas.at(index_R1).size();
			
			// Escolhendo índice do nó inicial para remoção (a partir do índice 1)
			int index_no_inicial = 1 + rand()%(n_nodes - 1);
			
			// Escolhendo índice do nó final para remoção: mínimo entre o último nó visitado e "no_inicial + k" (não considerando o depósito central) 
			int index_no_final = std::min(n_nodes - 2, index_no_inicial + k);
			
			// Contabilizando todos os pedidos contidos na seção de "index_no_inicial" a "index_no_final":
			
			// Obtendo seção:
			std::vector<double> k_nodes = {S.Rotas.at(index_R1).begin() + index_no_inicial, S.Rotas.at(index_R1).begin() + index_no_final};
			
			std::cout << "\n";
			
			for (auto &node: k_nodes){
				
				std::cout << node << " ";
				
			}
			
			std::cout << "\n";
			
			for (auto &node: k_nodes){
				
				// Se o nó referido for de pickup e o nó de delivery correspondente também estiver na subseção:
				if ((node <= S.inst.n) && (count(k_nodes.begin(), k_nodes.end(), node + S.inst.n))){
					
					std::cout << node << std::endl;
					
					S.remover_pedido(node, index_R1);
					
				}
				
			}
			
			// Questão: tentar inserir todos pedidos de L ou apenas os recém-removidos pela heurística?
			
			//for (auto &pedido: S.L){
				
			//	S = melhor_insercao(S, pedido, index_R2);
				
			//}
			
			break;
			
		}
		
		// Estrutura de busca local: Or-opt
		
		case 'O':{
			
			// Para gerar números aleatórios
			srand(time(NULL));
			
			// Quantidade "m" de rotas na solução:
			int m = S.Rotas.size();
			
			// Escolhendo índice da rota que terá os nós removidos
			double index_rota = rand()%(m);
			
			// Número de nós contidos na rota:
			int n_nodes = S.Rotas.at(index_rota).size();
			
			// Removendo pedidos:
			
			// Removendo "or_opt" pedidos da rota
			for (auto i {0}; i < or_opt; i++){
				
				// Inicializando pedido a ser removido:
				double pedido {9999};
				
				// Escolhendo pedido: deve ser representado por um nó de pickup, menor ou igual a "n"!
				while (pedido > S.inst.n){
				
					// Índice do pedido a ser removido na rota R1 (a partir do índice 1)
					double index_pedido_removido = 1 + rand()%(n_nodes - 1);
					
					pedido = S.Rotas.at(index_rota).at(index_pedido_removido);
					
				}
				
				S.remover_pedido(pedido, index_rota);
				
				// Atualizando variável com tamanho da rota, após remoção do pedido
				n_nodes -= 2;
				
			}
			
			// Reinserindo pedidos:
			
			// Obs -> inserir na melhor posição considerando todos os pedidos ou apenas a ordem deles na lista?
			
			// Questão: tentar inserir todos pedidos de L ou apenas os recém-removidos pela heurística?
			
			//for (auto &pedido: S.L){
				
			//	S = melhor_insercao(S, pedido, index_R2);
				
			//}
			
			
			break;
		}
		
		// Estrutura de busca local: 2-opt
		
		case 'W':{
			
			// Para gerar números aleatórios
			srand(time(NULL));
			
			// Quantidade "m" de rotas na solução:
			int m = S.Rotas.size();
			
			// Escolhendo índice da rota que terá os nós removidos
			double index_rota = rand()%(m);
			
			// Número de nós contidos na rota:
			int n_nodes = S.Rotas.at(index_rota).size();
			
			std::cout << "\nRota: " << index_rota << std::endl;
			
			// Escolhendo pedidos 1 e 2, tal que D1 é servido anteriormente à P2
			
			// Índice do último nó de pickup visitado
			int index_ultimo_no_pickup {};
			
			// Achando último nó de pickup visitado:
			for (int index_no {n_nodes - 2}; index_no > 0; index_no--){
				
				int node = S.Rotas.at(index_rota).at(index_no);
				
				if (node <= S.inst.n){
					
					index_ultimo_no_pickup = index_no;
					
					break;
					
				}
			}
			
			// Escolhendo pedido D1 -> Deve estar antes do último nó de pickup visitado
			int index_D1 {};
			
			int no_D1 {};
			
			// O pedido deve ser de delivery!
			while (no_D1 <= S.inst.n){
				
				std::cout << "\nPrimeiro while" << std::endl;
				
				index_D1 = 1 + rand()%(index_ultimo_no_pickup - 1);
				
				no_D1 = S.Rotas.at(index_rota).at(index_D1);
				
			}
			
			// Escolhendo pedido P2, com ordinalidade necessariamente maior que D1
			int index_P2 {};
			
			int no_P2 {9999};
			
			// O pedido deve ser de pickup!
			while (no_P2 > S.inst.n){
				
				std::cout << "\nSegundo while" << std::endl;
				
				index_P2 = index_D1 + rand()%(n_nodes - index_D1 - 2);
				
				no_P2 = S.Rotas.at(index_rota).at(index_P2);
				
			}
			
			std::cout << "D1: " << no_D1 << "\nP2: " << no_P2 << std::endl;
			
			// Trocando nós de posição:
			
			S.Rotas.at(index_rota).at(index_D1) = no_P2;
			
			S.Rotas.at(index_rota).at(index_P2) = no_D1;
			
			
			
			
			break;
		}
		
		
		
		default:
			std::cout << "Invalido" << std::endl;
			
	}
	
	// if S.isfeasible // se a mudança melhorou a função objetivo
	
	return S;
	
	
}
