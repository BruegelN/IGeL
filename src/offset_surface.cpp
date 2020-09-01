#include <igl/read_triangle_mesh.h>
#include <igl/write_triangle_mesh.h>
#include <igl/signed_distance.h>
#include <igl/copyleft/offset_surface.h>
#include "CLI11.hpp"

int main(int argc, char *argv[])
{
    CLI::App app{"Calculate the offset surface by running marching cube on a singed distance field of the given triangle mesh.\nMight close some holes!"};

    Eigen::MatrixXd V,SV;
    Eigen::MatrixXi F,SF;
    Eigen::MatrixXd GV;
    Eigen::VectorXd S;
    Eigen::RowVector3i side;

    std::string in_file, out_file = "out.obj";
    double isolevel = 0.01;
    int s = 100;
    igl::SignedDistanceType signed_distance_type = igl::SignedDistanceType::SIGNED_DISTANCE_TYPE_DEFAULT;

    app.add_option("-i,--input", in_file, "The triangle mesh to want to calc an offset surface.")->required();
    app.add_option("-o,--output", out_file, "The (expanded/shrinked) output file. Default is 'out.obj'");
    app.add_option("-l,--isolevel", isolevel, "Iso level to extract (signed distance: negative inside)");
    app.add_option("-s,--grid-resolution", s, "Number of grid cells along longest side (controls resolution).");

    CLI11_PARSE(app, argc, argv);

    if(!igl::read_triangle_mesh(in_file, V,F))
    {
        return 1;
    }
    igl::copyleft::offset_surface(V, F, isolevel,s , signed_distance_type, SV, SF, GV, side, S);
    igl::write_triangle_mesh(out_file, SV,SF);

    return 0;

}
