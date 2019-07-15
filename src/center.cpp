#include <igl/read_triangle_mesh.h>
#include <igl/write_triangle_mesh.h>

int main(int argc, char* argv[])
{
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;

    igl::read_triangle_mesh("/home/nico/git/libigl/tutorial/data/cow.off", V,F);

    Eigen::Vector3d means = V.colwise().mean();
    V.col(0).array() -= means(0);
    V.col(1).array() -= means(1);
    V.col(2).array() -= means(2);

    igl::write_triangle_mesh("center.obj", V, F);
    return 0;
}
