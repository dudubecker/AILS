#include "heuristicsFunctions.h"
#include "Sol.hpp"
#include "Instance.hpp"
#include <ctime>
#include <algorithm>
#include <stdio.h>
#include <chrono>
#include <numeric>


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
	
	// std::cout << "\n\n";
	
	int contador {0};
	
	// Realizando inserções
	for (auto index_rota {0}; index_rota < S.Rotas.size(); index_rota++){
		
		for (auto pos_insercao_no_pickup {1}; pos_insercao_no_pickup < S.Rotas[index_rota].size() + 1; pos_insercao_no_pickup++){
			
			for (auto pos_insercao_no_delivery {1}; pos_insercao_no_delivery < S.Rotas[index_rota].size() + 1; pos_insercao_no_delivery++){
				
				// Testando apenas índices de inserção válidos: índice de delivery maior do que o de pickup (precedence) e diferente dele!
				// A iteração começa em 1 e termina no tamanho da rota porque não se considera a primeira e última posição da rota, que são o depósito
				if ((pos_insercao_no_pickup != pos_insercao_no_delivery) and (pos_insercao_no_pickup < pos_insercao_no_delivery)){
					
					//contador++;
					
					//std::cout << "Rota: " << index_rota << "\nPos. ins. no pickup: " << pos_insercao_no_pickup << "\nPos. ins. no delivery: " << pos_insercao_no_delivery << std::endl;
					
					//std::cout << "Contador: " << contador << std::endl;
					
					// Criando cópia do objeto, para testar inserção
					// Sol S_teste = S;
					
					// Inserindo nós na rota, nas posições da iteração
					// S_teste.inserir_pedido(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
					
					if (isInsertionFeasible(S, pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery)){
							
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
	
	// std::cout << "\n\n";
	
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
					// Sol S_teste = S;
					
					// Inserindo nós na rota, nas posições da iteração
					// S_teste.inserir_pedido(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
					
					// if (S_teste.isFeasible(index_rota)){
					if (isInsertionFeasible(S, pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery)){
						
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

// Avaliando factibilidade da inserção de um pedido (sem realizar a inserção!)
bool isInsertionFeasible(Sol &S, double &pedido, int index_rota, int &pos_no_pickup, int &pos_no_delivery){
	
	// Variável booleana, que controla a factibilidade
	bool factivel = true;
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + S.inst.n};
	
	// Rota que terá inserção testada:
	std::vector<double> Rota = S.Rotas.at(index_rota);
	
	// Realizando inserção:
	
	Rota.insert(Rota.begin() + pos_no_pickup, no_pickup);
	Rota.insert(Rota.begin() + pos_no_delivery, no_delivery);
	
	// Checando factibilidade:
	if (!isRouteFeasible(Rota, S.inst)){
		
		factivel = false;
		
	}
	
	return factivel;
	
}

bool isRouteFeasible(std::vector<double> &Rota, Instance &inst){
	
	// *** Checando factibilidade da rota ***
	// *OBS: não é necessário testar pairing e precedence, porque os pedidos são inseridos em pares nas rotas e as posições de inserção já asseguram precedência
	
	// Variável booleana, que controla a factibilidade
	bool factivel = true;
	
	
	// Capacidade atual do veículo na rota (inicia como 0)
	double cap_atual {0};
	
	// Tempo atual da rota (inicia como 0)
	double t_atual {0};
	
	for (unsigned index_no {1}; index_no < Rota.size(); index_no++){
	
		// Variável que guarda o nó atual considerado na checagem de factibilidade
		int no_atual {Rota.at(index_no - 1)};
		
		// Variável que guarda o nó seguinte considerado na checagem de factibilidade
		int no_seguinte {Rota.at(index_no)};
		
		// Checando se ir do no atual para o nó seguinte irá violar as restrições de capacidade e time window
		if ((cap_atual + inst.q.at(no_seguinte) > inst.Cap) || (inst.l.at(no_seguinte) < t_atual + inst.t.at(no_atual).at(no_seguinte))){
			
			// Atribuindo valor falso para a variável de factibilidade e quebrando o laço for
			factivel = false;
			break;
			
		// Caso seja possível, os valores são atualizados
		} else {
			// Atualizando valores
			
			// Capacidade
			cap_atual += inst.q.at(no_seguinte);
			
			// Tempo
			// Caso haja adiantamento (tempo de chegada menor que a janela de tempo de abertura)
			if (t_atual + inst.t.at(no_atual).at(no_seguinte) < inst.e.at(no_seguinte)){
				t_atual = inst.e.at(no_seguinte);
				
			} else {
				t_atual += inst.t.at(no_atual).at(no_seguinte);
				
			}
		}
	}
	
	return factivel;
	
}

Sol primeira_insercao_factivel(Sol &S,double &pedido){
	
	// Criando vetor 2D para armazenar todas as combinações possíveis (rota, pos_insercao_no_pickup, pos_insercao_no_delivery)
	std::vector<std::vector<int>> pos_insercoes {};
	
	for (auto index_rota {0}; index_rota < S.Rotas.size(); index_rota++){
		
		for (auto pos_insercao_no_pickup {1}; pos_insercao_no_pickup < S.Rotas[index_rota].size() + 1; pos_insercao_no_pickup++){
			
			for (auto pos_insercao_no_delivery {1}; pos_insercao_no_delivery < S.Rotas[index_rota].size() + 1; pos_insercao_no_delivery++){
				
				// Testando apenas índices de inserção válidos: índice de delivery maior do que o de pickup (precedence) e diferente dele!
				// A iteração começa em 1 e termina no tamanho da rota porque não se considera a primeira e última posição da rota, que são o depósito
				// if ((pos_insercao_no_pickup != pos_insercao_no_delivery) and (pos_insercao_no_pickup < pos_insercao_no_delivery)){
				if (pos_insercao_no_pickup < pos_insercao_no_delivery){
					
					pos_insercoes.push_back({index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery});
					
				}
			}
		}
	}
	
	// Printando vetor:
	
	//for (auto vetor: pos_insercoes){
		
	//	std::cout << "\n";
		
	//	for (auto i: vetor){
			
	//		std::cout << i << " ";
			
	//	}
	//}
	
	// Método para "sortear" uma combinação aleatória e assegurar que todas foram testadas:
	
	// Número de combinações disponíveis para teste, atualizado dinamicamente
	int n_combinacoes = pos_insercoes.size();
	
	// Vetor com índices de combinações
	std::vector<int> index_combinacoes(n_combinacoes);
	std::iota(index_combinacoes.begin(), index_combinacoes.end(), 0);
	
	// Aleatorizando vetor com índices de combinações
	std::random_shuffle(index_combinacoes.begin(), index_combinacoes.end());
	
	// int contador {0};
	
	while (n_combinacoes > 0){
		
		// Escolhendo objeto correspondente ao primeiro índice aleatório
		std::vector<int> combinacao = pos_insercoes.at(index_combinacoes.at(0));
		
		// Removendo índice do vetor de índices de combinações
		index_combinacoes.erase(index_combinacoes.begin());
		
		// std::cout << "Testando posicao numero: " << contador << std::endl;
		// contador++;
		
		// Avaliando factibilidade da inserção
		if (isInsertionFeasible(S, pedido, combinacao.at(0), combinacao.at(1), combinacao.at(2))){
			
			S.inserir_pedido(pedido, combinacao.at(0), combinacao.at(1), combinacao.at(2));
			
			break;
			
		}
		
		// Se a inserção for factível, o loop se interrompe
		
		// Atualizando valor do número de combinações disponíveis para serem testadas
		n_combinacoes -= 1;
		
	}
	
	return S;
	
}


// Fim das funções utilizadas ao longo da implementação dos operadores/perturbações


