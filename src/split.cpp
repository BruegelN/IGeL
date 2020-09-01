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

    app.add_option("-i,--input", in_file, "The file you want to split into muliple files.")->required();
    app.add_option("-o,--output", out_file, "Prefix of the output files. Default is 'out-part-'.");

    CLI11_PARSE(app, argc, argv);

    if(!igl::read_triangle_mesh(in_file, V,F))
    {
        return 1;
    }

    igl::facet_components(F,C);
    std::vector<Eigen::MatrixXi> vec;
    for(int i = 0; i <= C.maxCoeff(); i++)
    {
        vec.emplace_back(1,3);
    }

    for(int rowIndex = 0; rowIndex < F.rows(); rowIndex++)
    {
        int comp = C(rowIndex);
        auto face = F.row(rowIndex);

        vec.at(comp).conservativeResize(vec.at(comp).rows()+1, vec.at(comp).cols());
        vec.at(comp).row(vec.at(comp).rows()-1) = face;
    }

    for(int i = 0; i <= C.maxCoeff(); i++)
    {
        Eigen::MatrixXi f = vec.at(i).bottomRows(vec.at(i).rows()-1);
        Eigen::MatrixXd v = V;
        Eigen::VectorXi I;
        igl::remove_unreferenced(Eigen::MatrixXd(v),Eigen::MatrixXi(f),v,f,I);
        igl::write_triangle_mesh(out_file+std::to_string(i)+".obj", v, f);
    }
}
