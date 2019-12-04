#include <igl/read_triangle_mesh.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <igl/opengl/glfw/imgui/ImGuiHelpers.h>
#include <imgui/imgui.h>
#include <igl/png/writePNG.h>
#include "CLI11.hpp"


int main(int argc, char *argv[])
{
  CLI::App app{"Use the libigl viewer to view a triangle mesh."};

  Eigen::MatrixXd V;
  Eigen::MatrixXi F;
  std::string in_file, screenshot_file = "screenshot.png";

  app.add_option("-i,--input", in_file, "The triangle mesh you want to view.")->required();
  app.add_option("-s,--screenshot", in_file, "Screenshot file. Default is 'screenshot.png");

  CLI11_PARSE(app, argc, argv);

  igl::read_triangle_mesh(in_file, V,F);

  // Init the viewer
  igl::opengl::glfw::Viewer viewer;

  // Attach a menu plugin
  igl::opengl::glfw::imgui::ImGuiMenu menu;
  viewer.plugins.push_back(&menu);

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
      igl::png::writePNG(R,G,B,A,screenshot_file);
    }
  };

  // Plot the mesh
  viewer.data().set_mesh(V, F);
  viewer.launch();
}
