#include <igl/read_triangle_mesh.h>
#include <igl/write_triangle_mesh.h>

int main(int argc, char* argv[])
{
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;

    igl::read_triangle_mesh("/home/nico/git/libigl/tutorial/data/cow.off", V,F);

    V = V.rowwise() - V.colwise().mean();
    igl::write_triangle_mesh("center.obj", V, F);
    return 0;
}
