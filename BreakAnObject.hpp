#include "TypeDefs.hpp"

class BreakAnObject {

	std::vector<std::string> names;
	std::vector<Polyhedron> polys;
	std::vector< std::vector<double> > coords;
	std::vector< std::vector< std::vector<int> > > faces; // for each polyhedron, for each face, each coord.
	char const *output;
	double ecart;
	std::vector<int> minFacets;

	void load_obj( const char *filename );
	void readFromThisObject(std::ifstream & myFile, std::string n, int it);
	std::vector<std::string> split(const std::string &s, char delim);
	Point_3 meanPoints(Point_3 p1, Point_3 p2);


	public :
	void exportObj();
	void exportObj(Polyhedron P);
	BreakAnObject(char const *input, char const *output, double ecart);
};
