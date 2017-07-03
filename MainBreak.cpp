#include "TypeDefs.hpp"
#include "BreakAnObject.hpp"

int main(int argc, char** argv) {
	srand (time(NULL));
	if(argc != 6) {
		std::cout << "Erreur : ./launch fileToImport nameToExport sensCoupure placementCoupure variation" << std::endl;
		std::cout << atof(argv[2]) << std::endl;
		return 1;
	}
		std::cout << argv[2] << std::endl;

	char const *input = argv[1];
	char const *output = argv[2];
	int newOrientationCoupure = atof(argv[3]);
	double placementCoupure = atof(argv[4]);
	double variation = atof(argv[5]);
	BreakAnObject o(input, output, newOrientationCoupure,placementCoupure,variation);

	return 0;
}
