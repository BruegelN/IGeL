#include <igl/read_triangle_mesh.h>
#include <igl/writeSTL.h>

int main(int argc, char *argv[])
{
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    Eigen::VectorXi C;


    igl::read_triangle_mesh("/home/nico/git/libigl/tutorial/data/cow.off", V, F);
    igl::writeSTL("out.stl", V, F);
}
