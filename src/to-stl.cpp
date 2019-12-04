#include <igl/read_triangle_mesh.h>
#include <igl/writeSTL.h>
#include "CLI11.hpp"

int main(int argc, char *argv[])
{
    CLI::App app{"Convert a triangle mesh to STL file format."};

    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    Eigen::VectorXi C;
    std::string in_file, out_file = "out.stl";

    app.add_option("-i,--input", in_file, "The triangle mesh you want to convert.")->required();
    app.add_option("-o,--output", out_file, "The output STL files. Default is 'out.stl'.");

    CLI11_PARSE(app, argc, argv);

    igl::read_triangle_mesh(in_file, V, F);
    igl::writeSTL(out_file, V, F);
}
