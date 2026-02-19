#include <igl/read_triangle_mesh.h>
#include <igl/write_triangle_mesh.h>
#include <igl/decimate.h>
#include <igl/qslim.h>
#include "CLI11.hpp"

int main(int argc, char *argv[])
{
    CLI::App app{"Deciamte a triangle mesh."};

    Eigen::MatrixXd V,U;
    Eigen::MatrixXi F,G;
    Eigen::VectorXi J,I;
    size_t desired_num_faces;
    std::string in_file, out_file = "decimated.obj";
    float decimation_percent = 30;

    app.add_option("-i,--input", in_file, "The triangle mesh you want to convert.")->required();
    app.add_option("-d,--desired_decimation", decimation_percent, "The desired number of remaining faces after decimation (in percents of the input mesh).");
    app.add_option("-o,--output", out_file, "The decimated mesh. Default is 'decimated.obj'.");

    CLI11_PARSE(app, argc, argv);

    if(!igl::read_triangle_mesh(in_file, V,F))
    {
        return 1;
    }

    desired_num_faces = (decimation_percent/100) * F.rows();
    igl::decimate(V,F,desired_num_faces,U,G,J,I);
    igl::write_triangle_mesh(out_file,U,G);

}