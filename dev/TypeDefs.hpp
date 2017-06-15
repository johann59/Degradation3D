#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <math.h>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Polyhedron_traits_with_normals_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/print_wavefront.h>
#include <CGAL/squared_distance_3.h>
#include <CGAL/aff_transformation_tags.h>
#include <CGAL/intersections.h>
#include <CGAL/number_utils.h>


typedef CGAL::Exact_predicates_exact_constructions_kernel     Kernel;
//typedef CGAL::Simple_cartesian<double>     Kernel;
typedef CGAL::Polyhedron_traits_with_normals_3<Kernel> Traits;
typedef CGAL::Polyhedron_3<Traits>                     Polyhedron;
typedef Polyhedron::HalfedgeDS             HalfedgeDS;
typedef Polyhedron::Vertex_iterator Vertex_iterator;
typedef Kernel::Point_3 Point_3;
typedef Kernel::Vector_3 Vector_3;
typedef Polyhedron::Facet_iterator Facet_iterator;
typedef Polyhedron::Facet Facet;
typedef Polyhedron::Facet_handle Facet_handle;
typedef Polyhedron::Halfedge_handle Halfedge_handle;
typedef Polyhedron::Halfedge_iterator Halfedge_iterator;
typedef Polyhedron::Point_iterator Point_iterator;
typedef Polyhedron::Vertex_handle Vertex_handle;
typedef Kernel::Plane_3 Plane_3;
typedef Kernel::Segment_3 Segment_3;
