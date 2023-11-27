#include "AILS.hpp"
#include "LocalSearchOperator.hpp"
#include "Sol.hpp"
#include "Instance.hpp"
#include <ctime>
#include <algorithm>
#include <stdio.h>
#include <chrono>
#include <random>


// Constructor com parâmetros da heurística
AILS::AILS(Sol &S_i, std::vector<LocalSearchOperator*> &objetos_busca_local, std::vector<Perturbation*> &objetos_perturbacoes,
		double eta_value, double kappa_value, double gamma_value, double d_b_value, double eta_noise_value, double alpha_value){
	
	// Atribuindo argumentos aos parâmetros
	eta = eta_value;
	kappa = kappa_value;
	gamma = gamma_value;
	d_b = d_b_value;
	
	// Inicializando objetos
	
	// Métodos de perturbação
	metodos_perturbacao = objetos_perturbacoes;
	
	// Operadores de busca local
	operadores_busca_local = objetos_busca_local;
	
	// Inicializando cálculos de ruído
	
	// Parâmetro alpha - probabilidade de aplicação do ruído - atributo de LSOperators e PerturbationProcedures
	
	for (auto metodo_perturbacao: metodos_perturbacao){
		
		metodo_perturbacao->alpha = alpha_value;
		
	}
	
	for (auto operador: operadores_busca_local){
		
		operador->alpha = alpha_value;
		
	}
	
	// Parâmetro eta_noise - utilizado no cálculo da dimensão do ruído - atributo de Sol
	S_i.eta_noise = eta_noise_value;
	
	// Solução incumbente e melhor solução encontrada:
	S_p = S_i;
	
	S_r = S_i;
	
}

// Default contructor
AILS::AILS()
{
	
	
}

AILS::~AILS()
{
}

// Método que tentará reduzir rotas
// /*
Sol AILS::reduzirRotas(Sol &S_i, int it_RRH){
	
	// Criando uma cópia da rota:
	Sol S = S_i;
	
	// Solução para armazenar a melhor solução (de menor número de rotas)
	Sol BKS = S_i;
	
	// Variável para o número de iterações:
	int n_it {0};
	
	while (n_it < it_RRH){
		
		// Caso "L" esteja vazio, a rota é excluída e os pedidos são colocados no banco de pedidos não atendidos
		if (S.L_size == 0){
			
			BKS = S;
			
			// Quantidade "m" de rotas na solução:
			int m = S.rotas.size();
			
			// Escolhendo índice da rota que será removida
			double index_rota = rand()%(m);
			
			// Removendo rota
			S.removerRota(index_rota);
			
		}
		
		// Escolhendo perturbações e aplicando-as à solução
		
		// Escolhendo e aplicando método de perturbação
		
		int random_index = std::rand() % metodos_perturbacao.size();
		
		int perturbationProcedureIndex = trunc(random_index);
		
		// Aplicando perturbação
		metodos_perturbacao.at(random_index)->aplicar(S);
		
		n_it += 1;
		
	}
	
	return BKS;
	
	
	
}
// */

// Método de aplicação iterativa das buscas locais até não haver mais melhorias
Sol AILS::executarBuscaLocal(Sol &S){
	
	// Criando cópia da solução, para controle das melhorias
	
	// Solução de referência da busca local
	Sol S_r_ls = S;
	
	// Criando uma cópia do vetor que contém os operadores de busca local
	std::vector<LocalSearchOperator*> operadores_busca_local_it = operadores_busca_local;
	
	
	// Enquanto o vetor não está vazio
	while(operadores_busca_local_it.size() > 0){
		
		// Aplicar operador de busca local aleatório
		int index_ls = rand()%(operadores_busca_local_it.size());
		
		LocalSearchOperator* operador_busca_local = operadores_busca_local_it.at(index_ls);
		
		operador_busca_local->aplicar(S);
		
		// Caso a solução tenha sido melhorada:
		if ((S.calcularFO() < S_r_ls.calcularFO())){
			
			// Atualizar solução de referência
			S_r_ls = S;
			
			// Restaurar vetor de operadores
			operadores_busca_local_it = operadores_busca_local;
			
		} else { // Caso a solução não tenha sido melhorada:
			
			
			
			// Restaurar solução incumbente
			S = S_r_ls;
			
			// Excluir operador do vetor
			operadores_busca_local_it.erase(operadores_busca_local_it.begin() + index_ls);
			
		}
		
	}
	
	return S_r_ls;
	
}

// Método para determinação da distância simétrica entre duas soluções
int AILS::calcularDistanciaSimetrica(Sol &S, Sol &S_r){
	
	// Variável para armazenar o número de arcos incomuns entre as soluções (distância simétrica)
	int distance {};
	
	// Matriz de 1 e 0 para arcos da solução S
	std::vector<std::vector<int>> E_S (2*S.inst.n + 2, std::vector<int> (2*S.inst.n + 2, 0));
	
	// Matriz de 1 e 0 para arcos da solução S_r
	std::vector<std::vector<int>> E_S_r = E_S;
	
	// Contabilizando arcos da solução S
	
	for (size_t index_rota {0}; index_rota < S.rotas.size(); index_rota++){
		
		for (int index_no {0}; index_no < S.rotas_size.at(index_rota) - 1; index_no++){
			
			E_S.at(S.rotas.at(index_rota).at(index_no)).at(S.rotas.at(index_rota).at(index_no + 1)) = 1;
			
		}
		
	}
	
	
	// Contabilizando arcos da solução S_r
	
	for (size_t index_rota {0}; index_rota < S_r.rotas.size(); index_rota++){
		
		for (int index_no {0}; index_no < S_r.rotas_size.at(index_rota) - 1; index_no++){
			
			E_S_r.at(S_r.rotas.at(index_rota).at(index_no)).at(S_r.rotas.at(index_rota).at(index_no + 1)) = 1;
			
		}
		
	}
	
	// Contabilizando arcos incomuns entre as soluções:
	
	for (int i {0}; i < 2*S.inst.n + 2 ; i++){
		
		for (int j {0}; j < 2*S.inst.n + 2 ; j++){
			
			// Caso o número em "i, j" nas matrizes seja diferente:
			if (E_S.at(i).at(j) !=  E_S_r.at(i).at(j)){
				
				distance += 1;
				
			}
			
		}
		
	}
	
	return distance;
}

// Método para atualização dos graus de perturbação
void AILS::atualizarGrauDePerturbacao(Sol &S, Sol &S_r, int index_metodo_perturbacao){
	
	int distance = calcularDistanciaSimetrica(S, S_r);
	
	// Contabilizando distância entre soluções:
	
	// Incrementando número de iterações da perturbação
	metodos_perturbacao.at(index_metodo_perturbacao)->it += 1;
	
	// Alterando valor de distância média encontrada pela perturbação
	metodos_perturbacao.at(index_metodo_perturbacao)->avgDist = ((metodos_perturbacao.at(index_metodo_perturbacao)->avgDist)*(metodos_perturbacao.at(index_metodo_perturbacao)->it - 1)+(distance))/(metodos_perturbacao.at(index_metodo_perturbacao)->it);
	
	if (metodos_perturbacao.at(index_metodo_perturbacao)->it == gamma){
		
		// Valor do novo grau de perturbação calculado
		
		// Corrigindo bug (relativamente raro) quando avgDist é igual a 0:
		if (metodos_perturbacao.at(index_metodo_perturbacao)->avgDist == 0){
			
			metodos_perturbacao.at(index_metodo_perturbacao)->avgDist = 1;
			
		}
		
		// Novo valor para o grau de perturbação
		int new_perturbation_degree = std::round((metodos_perturbacao.at(index_metodo_perturbacao)->w*d_b)/(metodos_perturbacao.at(index_metodo_perturbacao)->avgDist));
		
		metodos_perturbacao.at(index_metodo_perturbacao)->w = std::min(S.inst.n/2, (std::max(1, new_perturbation_degree)));
		
		// Reiniciando contagens
		metodos_perturbacao.at(index_metodo_perturbacao)->it = 0;
		
		metodos_perturbacao.at(index_metodo_perturbacao)->avgDist = 0;
		
	}
}

// Método para o critério de aceitação 
bool AILS::avaliarCriterioDeAceitacao(Sol &S){
	
	// Valor da função objetivo correspondente à solução f(S):
	double f_S = S.calcularFO();
	
	// Variável para retornar resultado do critério de aceitação
	bool accept = false;
	
	// Atualizando o valor de f_UP e f_UND
	
	// Caso o número de iterações não tenha chegado a "gamma":
	if (it <= gamma){
		
		f_UP = ((f_UP)*(it - 1) + (f_S) )/(it);
		f_UND = S_p.calcularFO();
		
		
	// Caso o algoritmo já tenha executado mais do que "gamma" iterações:
	}else{
		
		f_UP = f_UP*(1 - 1/gamma) + f_S/gamma;
		
	}
	
	// Atualizando valor de eta
	
	// O valor de eta é atualizado se o número de soluções aceitas atinge o valor gamma
	if (qtd_solucoes_totais == gamma){
		
		eta = (kappa*eta)/(qtd_solucoes_aceitas/qtd_solucoes_totais);
		
		// Caso eta resulte um valor muito pequeno
		if (eta < 0.001){
			
			eta = 0.001;
			
		} else if (eta > 1){
			
			eta = 1;
			
		}
		
		// Zerando valores com quantidades de soluções totais e aceitas
		qtd_solucoes_aceitas = 0;
		qtd_solucoes_totais = 0;
		
	}
	
	// Atualizando valor de b_UP
	
	b_UP = f_UND + eta*(f_UP - f_UND);
	
	// Se a solução atual for menor do que o limite calculado, haverá uma chance de "kappa" de ela ser escolhida
	
	if (f_S < b_UP){
		
		// Valor aleatório "n" entre 0 e 1
		double n = static_cast<double>(std::rand()) / RAND_MAX;
		
		// Caso o número aleatório seja menor do que kappa (kappa*100% chance)
		if (n < kappa){
			
			// Solução aceita!
			accept = true;
			
			qtd_solucoes_aceitas += 1;
			
		}
		
		// Independentemente da solução ter sido aceita, com kappa*100% de chance, computa-se a quantidade de soluções menores do que b_UP
		qtd_solucoes_totais += 1;
		
	}
	
	return accept;
	
}

// Método para execução do algoritmo em si:
void AILS::executarAILS(int max_it, int max_it_sem_melhoria, int it_RRH_intervalo, int it_RRH, double max_t){
	
	// Para gerar números aleatórios
	// srand(time(NULL));
	
	// Variável para o número de iterações sem melhoria:
	int it_sem_melhoria = 0;
	
	while (it < max_it){
		
		if (it_sem_melhoria == max_it_sem_melhoria){
			
			break;
			
		}
		
		
		//if (it%1000 == 0){
			
		//	std::cout << "Iteracao " << it << std::endl;
			
		//}
		
		
		// Criando uma cópia da solução de referência
		
		Sol S = S_r;
		
		// Aplicando código para redução de rotas, a cada it_RRH_interval iterações
		
		if ((it%it_RRH_intervalo == 0) && (S.rotas.size() > 1)){
			
			reduzirRotas(S, it_RRH);
			
		}
		
		// Escolhendo e aplicando método de perturbação
		
		int random_index = std::rand() % metodos_perturbacao.size();
		
		int index_metodo_perturbacao = trunc(random_index);
		
		metodos_perturbacao.at(index_metodo_perturbacao)->aplicar(S);
		
		// Aplicando buscas locais, na forma de RVND
		executarBuscaLocal(S);
		
		// Atualizando grau de perturbação
		atualizarGrauDePerturbacao(S, S_r, index_metodo_perturbacao);
		
		// Aplicando critério de aceitação
		if (avaliarCriterioDeAceitacao(S)){
			
			S_r = S;
			
		}
		
		// Atualizando melhor solução encontrada
		
		if (S.calcularFO() < S_p.calcularFO()){
			
			S_p = S;
			
			it_sem_melhoria = 0;
			
		} else {
			
			it_sem_melhoria += 1;
			
		}
		
		it += 1;
		
	}
	
}