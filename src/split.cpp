#include <igl/read_triangle_mesh.h>
#include <igl/write_triangle_mesh.h>
#include <igl/facet_components.h>
#include <igl/remove_unreferenced.h>
#include "CLI11.hpp"

int main(int argc, char *argv[])
{
    CLI::App app{"Split a triangle mesh containing multiple part into many files."};

    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    Eigen::VectorXi C;
    std::string in_file,out_file="out-part-";

    app.add_option("-i,--input", in_file, "The file you want to split into multiple files.")->required();
    app.add_option("-o,--output", out_file, "Prefix of the output files. Default is 'out-part-'.");

    CLI11_PARSE(app, argc, argv);

    if(!igl::read_triangle_mesh(in_file, V,F))
    {
        return 1;
    }

    const size_t n = igl::facet_components(F,C);
    for(size_t i = 0; i < n; i++)
    {
        Eigen::MatrixXd v;
        Eigen::MatrixXi Fcomponent,f;
        Eigen::VectorXi I;
        // only keep faces of current component
        igl::slice_mask(F,(C.array() == i),1,Fcomponent);
        // get rid all the other vertices
        igl::remove_unreferenced(Eigen::MatrixXd(V),Eigen::MatrixXi(Fcomponent),v,f,I);
        igl::write_triangle_mesh(out_file+std::to_string(i)+".obj", v, f);
    }
}
