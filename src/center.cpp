#include <igl/read_triangle_mesh.h>
#include <igl/write_triangle_mesh.h>
#include "CLI11.hpp"

int main(int argc, char* argv[])
{
    CLI::App app{"Center a triangle mesh at the origin (0,0,0)."};

    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    std::string in_file, out_file = "center.obj";

    app.add_option("-i,--input", in_file, "The triangle mesh you want to check.")->required();
    app.add_option("-o,--output", out_file, "The output files. Default is 'center.obj'.");

    CLI11_PARSE(app, argc, argv);

    if(!igl::read_triangle_mesh(in_file, V,F))
    {
        return 1;
    }

    V = V.rowwise() - V.colwise().mean();
    igl::write_triangle_mesh(out_file, V, F);
    return 0;
}
