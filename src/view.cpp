#include <igl/read_triangle_mesh.h>
#include <igl/readOBJ.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <igl/opengl/glfw/imgui/ImGuiHelpers.h>
#include <imgui/imgui.h>
#include <igl/png/writePNG.h>
#include <filesystem>
#include "CLI11.hpp"

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
  CLI::App app{"Use the libigl viewer to view a triangle mesh."};

  Eigen::MatrixXd V,TC;
  Eigen::MatrixXi F,FTC;
  std::string in_file, screenshot_file = "screenshot.png";

  app.add_option("-i,--input", in_file, "The triangle mesh you want to view.")->required();
  app.add_option("-s,--screenshot", screenshot_file, "Screenshot file. Default is 'screenshot.png");

  CLI11_PARSE(app, argc, argv);

  if(!igl::read_triangle_mesh(in_file, V,F))
  {
    return 1;
  }

  if(".obj" == fs::path(in_file).extension())
  {
    Eigen::MatrixXd N,FN;
    igl::readOBJ(in_file,V,TC,N,F,FTC,FN);
  }

  const bool has_texture = [&]() -> bool { return (FTC.rows() == F.rows() && TC.rows() >= V.rows());}();

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
      static size_t num_screenshot = 1;
      // Allocate temporary buffers
      Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> R(2560,1600);
      Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> G(2560,1600);
      Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> B(2560,1600);
      Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> A(2560,1600);

      // Draw the scene in the buffers
      viewer.core().draw_buffer(
        viewer.data(),false,R,G,B,A);

      // Save it to a PNG
      igl::png::writePNG(R,G,B,A,std::to_string(num_screenshot)+screenshot_file);
      num_screenshot++;
    }
    ImGui::Separator();
    if(has_texture)
    {
      static int e = 0;
      if (ImGui::RadioButton("3d mesh",&e,0))
      {
        viewer.data().clear();
        viewer.data().set_mesh(V, F);
      }
      if (ImGui::RadioButton("2d texture",&e,1))
      {
        viewer.data().clear();
        viewer.data().set_mesh(TC, FTC);
      }
    }
  };

  viewer.core().background_color.setConstant(1);
  // Plot the mesh
  viewer.data().set_mesh(V, F);
  viewer.launch();
}
