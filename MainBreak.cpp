#include "TypeDefs.hpp"
#include "BreakAnObject.hpp"

int main(int argc, char** argv) {
	srand (time(NULL));
	if(argc != 4) {
		std::cout << "Erreur : ./launch fileToImport nameToExport ecart" << std::endl;
		std::cout << atof(argv[2]) << std::endl;
		return 1;
	}
		std::cout << argv[2] << std::endl;

	char const *input = argv[1];
	char const *output = argv[2];
	double ecart = atof(argv[3]);
	BreakAnObject o(input, output, ecart);

	return 0;
}
