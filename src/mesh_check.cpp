#include <iostream>
#include <igl/read_triangle_mesh.h>
#include <igl/euler_characteristic.h>
#include <igl/edges.h>
#include <igl/boundary_loop.h>
#include <igl/is_edge_manifold.h>
#include <igl/remove_unreferenced.h>

#include <iostream>

int main(int argc, char* argv[])
{
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    igl::read_triangle_mesh("/home/nico/git/libigl/tutorial/data/truck.obj", V,F);

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
            std::cout << J << std::endl;
        }
        else
        {
            std::cout << "no unreferenced vertices." << std::endl;
        }
    }
}