#include <igl/read_triangle_mesh.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <igl/opengl/glfw/imgui/ImGuiHelpers.h>
#include <igl/boundary_facets.h>
#include <igl/unique.h>
#include <igl/stb/write_image.h>
#include "CLI11.hpp"

  Eigen::MatrixXd V, TC, N, FN;
  Eigen::MatrixXi F, FTC;

template <
      typename DerivedF,
      typename DerivedB,
      typename DerivedColor>
    void colorize_boundaries(const Eigen::MatrixBase<DerivedF>& F,
                            Eigen::PlainObjectBase<DerivedB>& boundary,
                            Eigen::PlainObjectBase<DerivedColor>& color)
    {

	// set color default
	color.resize(V.rows(), 3);
  	for (int i = 0; i < V.rows(); i++)
  	{
      	    color.row(i) << Eigen::RowVector3d (255.0 / 255.0, 228.0 / 255.0, 58.0 / 255.0);
  	}

        typename DerivedB::PlainObject temp;
        igl::boundary_facets(F, temp);
        igl::unique(temp.col(0), boundary);
	// colorize boundaries red
        for(size_t i = 0; i < boundary.size(); i++)
        {
            int idx = boundary.data()[i];
            color.row(idx) << 1,0,0;
        }
    }


int main(int argc, char *argv[])
{
  CLI::App app{"Use the libigl viewer to highlight all boundaries of a triangle mesh in red."};

  std::string in_file, screenshot_file = "screenshot.png";

  app.add_option("-i,--input", in_file, "The triangle mesh you want to view the boundaries.")->required();
  app.add_option("-s,--screenshot", in_file, "Screenshot file. Default is 'screenshot.png");

  CLI11_PARSE(app, argc, argv);

  if(!igl::readOBJ(in_file, V, TC, N, F, FTC, FN))
  {
    return 1;
  }

  // Init the viewer
  igl::opengl::glfw::Viewer viewer;

  // Attach a menu plugin
  igl::opengl::glfw::imgui::ImGuiPlugin imgui_plugin;
  viewer.plugins.push_back(&imgui_plugin);

  igl::opengl::glfw::imgui::ImGuiMenu menu;
  imgui_plugin.widgets.push_back(&menu);

  // Add content to the default menu window
  menu.callback_draw_viewer_menu = [&]()
  {
    // Draw parent menu content
    menu.draw_viewer_menu();
    ImGui::Separator();
    if (ImGui::Button("Take screenshot (2560×1600)", ImVec2(-1, 0)))
    {
      // Allocate temporary buffers
      Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> R(2560,1600);
      Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> G(2560,1600);
      Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> B(2560,1600);
      Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> A(2560,1600);

      // Draw the scene in the buffers
      viewer.core().draw_buffer(
        viewer.data(),false,R,G,B,A);

      // Save it to a PNG
      igl::stb::write_image(screenshot_file,R,G,B,A);
    }
  };
  Eigen::MatrixXd C(V.rows(), 3);
  for (int i = 0; i < V.rows(); i++)
  {
      C.row(i) << Eigen::RowVector3d (255.0 / 255.0, 228.0 / 255.0, 58.0 / 255.0);
  }
  Eigen::MatrixXi F_boundary;
  colorize_boundaries(F,F_boundary, C);

  // Plot the mesh
  viewer.data().set_mesh(V, F);
  viewer.data().set_colors(C);
  viewer.launch();
}
