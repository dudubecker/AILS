#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include <iostream>
#include <vector>
#include <cmath>

class Instance
{
	//// Atributos:
	//private:
	public:
	
	// Coordenadas x de cada nó i da instância
	std::vector <double> x {};
	
	// Coordenadas y de cada nó i da instância
	std::vector <double> y {};
	
	// Tempo de serviço de cada nó i da instância
	std::vector <double> d {};
	
	// Demanda de cada nó i da instância
	std::vector <double> q {};
	
	// Início da janela de tempo de cada nó i da instância
	std::vector <double> e {};
	
	// Fim da janela de tempo de cada nó i da instância
	std::vector <double> l {};
	
	// Número de pedidos
	int n {};
	
	// Capacidade de cada veículo
	int Cap {};
	
	// Valor da máxima distância t_ij, utilizado para aplicar random noise no cálculo da FO
	double max_dist = {};
	
	// Definindo vetor com "t_ij", para cada par de nós i j:
	
	std::vector<std::vector<double>> t {};
	
	// Atributos para a perturbação adaptada de concentric removal
	
	// Vetor com valores "x" médios para cada pedido (ponto médio entre pickup e delivery)
	std::vector<double> x_med {};
	
	// Vetor com valores "y" médios para cada pedido (ponto médio entre pickup e delivery)
	std::vector<double> y_med {};
	
	// Vetor com distâncias entre pontos médios dos pedidos "i" e "j"
	std::vector<std::vector<double>> t_med {};
	
	
	// Dados normalizados para Shaw's removal: demandas (q) e tempos de viagem (t)
	
	std::vector <double> q_norm {};
	
	std::vector<std::vector<double>> t_norm {};
	
	// Constructor/Destructor
	Instance();
	
	~Instance();
	
	// Methods
	
	void read(std::string file_name);
	
};

#endif // _INSTANCE_H_
