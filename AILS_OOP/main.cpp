#include <iostream>
#include <vector>
#include <Instance.hpp>
#include <Sol.hpp>
#include <LocalSearchOperator.hpp>
#include <heuristicsFunctions.h>
#include <Perturbation.hpp>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <thread>

using namespace std;

// Função para printar vetores: debug

void print(std::vector<int> const &input)
{
    for (auto const &i: input) {
        cout << i << " ";
    }
	
	cout << endl;
	
}

int main(){
	
	Instance inst;
	
	inst.read("BB30");
	
	
	// Inicializando a partir do objeto instância:
	
	Sol S(inst);
	
	// Heurística construtiva:
	
	LocalSearchOperator Constructive('C');
	
	Constructive.apply(S);
	
	S.print_sol();
	
	cout << "\n FO: " << std::setprecision(7) << S.FO() << endl;
	
	Sol S_cons = S;
	
	// LocalSearchOperator Swap('S',1,1);
	
	// Swap.apply(S);
	
	// LocalSearchOperator Shift('T',6);
	
	// Shift.apply(S);
	
	LocalSearchOperator Or_opt('O', 2);
	
	// Or_opt.apply(S);
	
	LocalSearchOperator Two_Opt('W');
	
	// Two_Opt.apply(S);
	
	// Perturbation Random('R');
	
	// Random.apply(S, 6);
	
	// Perturbation Worst('W');
	
	// Worst.apply(S, 4);
	
	LocalSearchOperator Shaw('H', 2,0.3,0.4,0.3);
	
	// Sol BKS = S_cons;
	
	double min_FO = S.FO();
	
	for (int k {0}; k < 1000; k++){
		
		//if (k%2 == 0){
			
			Perturbation Random('R');
		
			Random.apply(S, 2);
			
		//}else{
			
			Perturbation Worst('W');
			
			Worst.apply(S, 6);
			
		//}
		
		
		// LocalSearchOperator Shaw('H', 2,0.3,0.4,0.3);
		
		for (int j {0}; j < 10; j++){
			
			Shaw.apply(S);
			
			Or_opt.apply(S);
			
		}
		
		if ((S.FO() < min_FO) && (S.L.size() == 0)){
			
			min_FO = S.FO();
			
		}
		
		
		// LocalSearchOperator Or_opt('O', 4);
		
		// Or_opt.apply(S);
		
		// Shift.apply(S);
		
		// Two_Opt.apply(S);
		
		S.print_sol();
		
		cout << "\n FO: " << std::setprecision(7) << S.FO() << endl;
		
		// cout << "\n Factibilidade: " << S.isFeasible()  << endl;
		
		// std::this_thread::sleep_for(std::chrono::milliseconds(300));
		
		
		//if ((S.L.size() == 0) && (S.isFeasible())){
			
		//	S.print_sol();
			
		//	cout << "\n FO: " << std::setprecision(7) << S.FO() << endl;
			
		//}
		
		std::cout << "\n\nIteracao: " << k << std::endl;
		
		// S = S_cons;
		
	}
	
	
	cout << "Menor funcao objetivo encontrada: " << std::setprecision(7) << min_FO << endl;
	
}