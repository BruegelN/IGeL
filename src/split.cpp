#include <igl/read_triangle_mesh.h>
#include <igl/write_triangle_mesh.h>
#include <igl/facet_components.h>
#include <igl/remove_unreferenced.h>

int main(int argc, char *argv[])
{
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    Eigen::VectorXi C;

    igl::read_triangle_mesh("/home/nico/git/libigl/tutorial/data/truck.obj", V, F);

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
        igl::write_triangle_mesh("out-part-"+std::to_string(i)+".stl", v, f);
    }
}
