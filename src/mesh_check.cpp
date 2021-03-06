#include <iostream>
#include <igl/read_triangle_mesh.h>
#include <igl/euler_characteristic.h>
#include <igl/edges.h>
#include <igl/boundary_loop.h>
#include <igl/is_edge_manifold.h>
#include <igl/is_vertex_manifold.h>
#include <igl/remove_unreferenced.h>
#include <igl/facet_components.h>

#include <iostream>

#include "CLI11.hpp"

int main(int argc, char* argv[])
{
    CLI::App app{"Perform various checks on a triangle mesh and print the result."};

    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    Eigen::VectorXi C;
    std::string in_file;

    app.add_option("-i,--input", in_file, "The triangle mesh you want to check.")->required();

    CLI11_PARSE(app, argc, argv);

    if(!igl::read_triangle_mesh(in_file, V,F))
    {
        return 1;
    }

    {
        // components
        igl::facet_components(F,C);
        std::cout << "#components: " << (1+C.maxCoeff())/* index statring at 0 */ << std::endl;
    }

    int number_of_boundaries = 0;

    {
        // boundaries
        std::vector<std::vector<int>> L;
        igl::boundary_loop(F, L);
        number_of_boundaries = L.size();
        std::cout << "#boundaries: " << number_of_boundaries << std::endl;
    }

    {
        // edge manifold
        if(igl::is_edge_manifold(F))
        {
            std::cout << "The mesh is an edge manifold!" << std::endl;
        }
        else
        {
            std::cout << "The mesh is NOT an edge manifold!" << std::endl;
        }
    }

    {
        // vertex manifold
        Eigen::VectorXi B;
        if(igl::is_vertex_manifold(F,B))
        {
            std::cout << "The mesh is vertex manifold!" << std::endl;
        }
        else
        {
            std::cout << "The mesh is NOT vertex manifold!" << std::endl;
        }
    }

    {
        // Euler characteristic
        Eigen::MatrixXi E;
        igl::edges(F,E);
        int euler_characteristic = igl::euler_characteristic(F);
        std::cout << "V.rows(): " << V.rows() << " F.rows(): "<< F.rows()  << " E.rows(): " << E.rows() << std::endl;
        std::cout << "Euler characteristic: "<< euler_characteristic << std::endl;
        std::cout << "Euler characteristic + #boundaries: "<< (euler_characteristic+number_of_boundaries) << std::endl;
    }

    {
        // check for unreferenced vertices
        Eigen::MatrixXd NV;
        Eigen::MatrixXi NF;
        Eigen::VectorXi I, J;
        igl::remove_unreferenced(V,F,NV,NF,I,J);
        int V_rows_diff = V.rows() - NV.rows();
        if(0 != V_rows_diff)
        {
            std::cout << "You have " << V_rows_diff << " unreferenced vertices in your mesh!" << std::endl;
        }
        else
        {
            std::cout << "no unreferenced vertices." << std::endl;
        }
    }
}
