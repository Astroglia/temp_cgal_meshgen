#define CGAL_MESH_3_VERBOSE

// volume mesh generation imports
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Mesh_triangulation_3.h>
#include <CGAL/Mesh_complex_3_in_triangulation_3.h>
#include <CGAL/Mesh_criteria_3.h>
#include <CGAL/Labeled_mesh_domain_3.h>
#include <CGAL/make_mesh_3.h>
#include <CGAL/Image_3.h>
#include <CGAL/IO/output_to_vtu.h>

#ifdef CGAL_CONCURRENT_MESH_3
typedef CGAL::Parallel_tag Concurrency_tag;
#else
typedef CGAL::Sequential_tag Concurrency_tag;
#endif

//// use a cartesian coordinate kernel: allows for construction of points from "double" cartesian coordinate.
//// geometric predicates are exact (e.g. boolean ops between geometric domains), construction of geometry is inexact.
using K = CGAL::Exact_predicates_inexact_constructions_kernel;
using Mesh_domain = CGAL::Labeled_mesh_domain_3<K>; // contains functionality/information to discretize a labelmap into a mesh.
// Triangulation
using Tr = CGAL::Mesh_triangulation_3<Mesh_domain, CGAL::Default, Concurrency_tag>::type;
using C3t3 = CGAL::Mesh_complex_3_in_triangulation_3<Tr>;
using Mesh_criteria = CGAL::Mesh_criteria_3<Tr>; // mesh criteria --> define facet angles, distances, cell sizes.
using namespace CGAL::parameters; //for facet size, facet distance, etc.

int main(int argc, char* argv[])
{
    const char* inputfilepath = "C:\\Users\\markus\\source\\repos\\Project2\\test_aligned.inr";
    CGAL::Image_3 image;
    image.read(inputfilepath);

    Mesh_domain domain = Mesh_domain::create_labeled_image_mesh_domain(image);
    // Mesh criteria
    // cell size - how large the tetrahedra are  ( upper bound ) - controls circumradius.
    // facet size - how large each facet is      ( upper bound )
    // facet distance -> circular center of each facet, this is how far away the center of each facet (of a tetrahedron) is away
                        // from the center of the tetrahedrons delauney ball
                        // e.g., like a center of mass https://www.researchgate.net/profile/Jean-Daniel_Boissonnat/publication/280788991/figure/fig2/AS:614141945454612@1523434258807/a-Surface-Delaunay-ball-b-Surface-Delaunay-ball-radius-R-approximation-error-of_W640.jpg
                        // ( upper bound )

    // NOTES: these criterias lead to to >30 min mesh generation (regardless of manifold tag)
    //Mesh_criteria criteria(facet_angle = 30, facet_size = 3, cell_size = 3, facet_distance = 4, cell_radius_edge_ratio = 3);
    //Mesh_criteria criteria(facet_angle = 15, facet_size = 8, cell_radius_edge_ratio = 3);

       // DEFAULT -- from tutorial
    Mesh_criteria criteria(facet_angle = 30, facet_size = 6, facet_distance = 4,
        cell_radius_edge_ratio = 3, cell_size = 8);
    std::cout << "Starting Initial Mesh Generation" << std::endl;
    C3t3 c3t3 = CGAL::make_mesh_3<C3t3>(domain, criteria, manifold());
    std::cout << "Finished mesh generation" << std::endl;

    std::ofstream newFile("test3.off");
    c3t3.output_boundary_to_off(newFile);
}