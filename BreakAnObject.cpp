	#include "TypeDefs.hpp"
#include "BreakAnObject.hpp"


typedef CGAL::Nef_polyhedron_3<Kernel> Nef_polyhedron;

template<class HDS>
class polyhedron_builder : public CGAL::Modifier_base<HDS> {
public:
	std::vector<double> &coords;
	std::vector< std::vector<int> >    &faces;
	int minFacet;
	polyhedron_builder( std::vector<double> &_coords, std::vector< std::vector<int> > &_faces, int _minFacet ) : coords(_coords), faces(_faces), minFacet(_minFacet) {}
	void operator()( HDS& hds) {
		typedef typename HDS::Vertex   Vertex;
		typedef typename Vertex::Point Point;

		// create a cgal incremental builder
		CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
		B.begin_surface( coords.size()/3, faces.size() );

		// add the polyhedron vertices
		for( int i=0; i<(int)coords.size(); i+=3 ){
			B.add_vertex( Point( coords[i+0], coords[i+1], coords[i+2] ) );
		}
		// add the polyhedron faces
		for(int i = 0 ; i < faces.size() ; i++) {
			B.begin_facet();
			for(int j = 0 ; j < faces[i].size() ; j++) {
				B.add_vertex_to_facet( faces[i][j] - minFacet );
			}
			B.end_facet();
		}

		// finish up the surface
		B.end_surface();
	}
};

BreakAnObject::BreakAnObject(char const *input, char const *output, int changeOrientationCoupure,double placementCoupure, double variation) {
  Polyhedron P;
	this->output = output;
	// load the input file
	load_obj(input);
	if( coords.size() == 0 ) {
		std::cout << "Aucun objet n'a été chargé" << std::endl;
	}
	else {
		std::cout << "objet chargé" << std::endl;
	 // build polyhedrons from the loaded arrays
		for(int i = 0 ; i < names.size() ; i++) {

			polyhedron_builder<HalfedgeDS> builder( coords[i], faces[i], minFacets[i] );
			P.delegate( builder );
			polys.push_back(P);

		}
	}
	//std::vector<Halfedge_handle> vecHalfedge;

	std::vector<Point_3> pointsDecoupe;
	Polyhedron p2;
	for(int i = 0 ; i < polys.size() ; i++) {
		Facet_iterator fi = polys[i].facets_begin();
		Halfedge_handle hh = fi->halfedge();
		hh = hh->opposite()->next();
		if (changeOrientationCoupure == 1) { // CHANGER d'orientation de coupure 2eme cas
			hh = hh->next();
		}
		if (changeOrientationCoupure == 2) { // CHANGER d'orientation de coupure 3eme cas
			hh = hh->opposite()->next();
		}
		// Parcours de toutes les aretes qui nous intéressent
		// todo parcours autre que sur une face d'un cube.
		for(int j = 0; j < 4 ;j++){
				Halfedge_handle hnew = polys[i].split_edge(hh);

				Point_3 pt1 = hnew->vertex()->point();
				Point_3 pt2 = hnew->next()->vertex()->point();

				Point_3 pt3 = meanPoints(pt1,pt2);

				// GENERATION des POINTS
				Point_3 pt4;
				Point_3 pt5;

				float k1 = placementCoupure - variation; // Placement du point sur le segment
				float k2 = placementCoupure + variation ; // Placement du deuxième point sur le segment

				pt4 = Point_3(k1*(pt2.hx()-pt1.hx())+pt1.hx(),
											k1*(pt2.hy()-pt1.hy())+pt1.hy(),
											k1*(pt2.hz()-pt1.hz())+pt1.hz());
				pt5 = Point_3(k2*(pt2.hx()-pt1.hx())+pt1.hx(),
											k2*(pt2.hy()-pt1.hy())+pt1.hy(),
											k2*(pt2.hz()-pt1.hz())+pt1.hz());


				pointsDecoupe.push_back(pt4);
				pointsDecoupe.push_back(pt5);


				hh = hnew->opposite()->next()->next();
		}
		Point_3 pt;

		// AFFICHER les points de la découpe
		 /*for (int k = 0 ; k < pointsDecoupe.size(); k++){
			pt = pointsDecoupe[k];
			std::cout << "P( : " << pt.hx() << "," << pt.hy() << "," << pt.hz() <<')' << '\n';
		} */

		// Création du polyhedron qui va faire la découpe

		Halfedge_handle h = p2.make_tetrahedron( pointsDecoupe[1],
																				pointsDecoupe[6],
																				pointsDecoupe[0],
																				pointsDecoupe[2]);
		Halfedge_handle g = h->next()->opposite()->next();             // Fig. (a)
		p2.split_edge( h->next());
		p2.split_edge( g->next());
		p2.split_edge( g);                                              // Fig. (b)
		h->next()->vertex()->point()     = pointsDecoupe[7];
		g->next()->vertex()->point()     = pointsDecoupe[4];
		g->opposite()->vertex()->point() = pointsDecoupe[3];            // Fig. (c)
		Halfedge_handle f = p2.split_facet( g->next(),
																			 g->next()->next()->next()); // Fig. (d)
		Halfedge_handle e = p2.split_edge( f);
		e->vertex()->point() = pointsDecoupe[5];                        // Fig. (e)
		p2.split_facet( e, f->next()->next());                          // Fig. (f)
		CGAL_postcondition( p2.is_valid());


	}
	Polyhedron p3;
	Nef_polyhedron n1(P);
	Nef_polyhedron n2(p2);
	n1 = n1 -n2; // todo problème découpe lors d'une face non rectiligne
	if (n1.is_simple()){
		n1.convert_to_Polyhedron(p3);
	}else {
		std::cout << "Nouvelle objet pas du type polyhedron" << '\n';
	}
	exportObj(p3);
	}

// barebones .OFF file reader, throws away texture coordinates, normals, etc.
// stores results in input coords array, packed [x0,y0,z0,x1,y1,z1,...] and
// faces array packed [T0a,T0b,T0c,T1a,T1b,T1c,...]
void BreakAnObject::load_obj(const char *filename){
	std::string line;
	std::ifstream myFile (filename);
	if(myFile.is_open()) {
		readFromThisObject(myFile, "", 0);
		myFile.close();
	}
	std::reverse(minFacets.begin(), minFacets.end());
	std::reverse(names.begin(), names.end());
	std::reverse(coords.begin(), coords.end());
	std::reverse(faces.begin(), faces.end());
}

// Obj reader, transfère dans les structures de création des polyhedrons les données du fichier
void BreakAnObject::readFromThisObject(std::ifstream & myFile, std::string n, int it) {
	int curMinFacet = -1;
	std::string line;
	std::vector<double> vertexes;
	std::vector< std::vector<int> > facets;
	std::string name = n;

	while(getline(myFile,line)){
		if(line.size() >= 2) {
			if(line.at(0) == 'v' && (line.at(1) != 't' && line.at(1) != 'n')){
				std::vector<std::string> vert = split(line, ' ');
				vertexes.push_back(::atof(vert[1].c_str()));
				vertexes.push_back(::atof(vert[2].c_str()));
				vertexes.push_back(::atof(vert[3].c_str()));
			}
			else if(line.at(0) == 'f'){
				std::vector<std::string> face = split(line, ' ');
				std::vector<int> f;
				for(int i = 1 ; i < face.size() ; i++) {
					int j = ::atoi(face[i].c_str()) - 1;
					f.push_back(j);
					if(curMinFacet == -1) {
						curMinFacet = j;
					}
					else if(j < curMinFacet) {
						curMinFacet = j;
					}
				}
				facets.push_back(f);
			}
			else if(line.at(0) == 'o' || line.at(0) == 'g') {
				readFromThisObject(myFile, line.substr(2), it+1);
			}
		}
	}
	if(it != 0) {
		minFacets.push_back(curMinFacet);
		names.push_back(name);
		coords.push_back(vertexes);
		faces.push_back(facets);
	}
}

// Split un std::string
std::vector<std::string> BreakAnObject::split(const std::string &s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}

// Exporte les polyhedrons en obj. REVOIR UN DETAIL DESSUS VIS A VIS DE LA NUMEROTATION
void BreakAnObject::exportObj() {
	std::ofstream ofs(output);
	for(int i = 0 ; i < polys.size() ; i++) {
		ofs << "o " << names[i] << "\n";
		CGAL::print_polyhedron_wavefront(ofs, polys[i]);
	}
	ofs.close();
	std::cout << "objet exporté" << std::endl;

}

// Exporte un polyhedron ciblé en obj. Utile à des fins de tests
void BreakAnObject::exportObj(Polyhedron P) {
	std::ofstream ofs(output);
	CGAL::print_polyhedron_wavefront(ofs, P);
	ofs.close();
	std::cout << "objet exporté" << std::endl;

}

// Calcule une moyenne de deux points
Point_3 BreakAnObject::meanPoints(Point_3 p1, Point_3 p2) {
	Point_3 pt(p2.x() + p1.x(), p2.y() + p1.y(), p2.z() + p1.z());
	pt = Point_3(pt.x()/2, pt.y()/2, pt.z()/2);
	return pt;
}
