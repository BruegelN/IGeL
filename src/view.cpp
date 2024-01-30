#include <igl/read_triangle_mesh.h>
#include <igl/readOBJ.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <igl/opengl/glfw/imgui/ImGuiHelpers.h>
#include <igl/stb/write_image.h>
#include <igl/principal_curvature.h>
#include <igl/colormap.h>
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

  Eigen::VectorXd H,K;
  Eigen::MatrixXd C;
  Eigen::VectorXd PV1,PV2;
  Eigen::VectorXd AbsMaxCurvature;
  {
    Eigen::MatrixXd PD1,PD2;
    igl::principal_curvature(V,F,PD1,PD2,PV1,PV2);
    // mean curvature
    H = 0.5*(PV1+PV2);
    // gaussian curvature
    K = PV1.array()*PV2.array();
    AbsMaxCurvature = PV1.cwiseAbs().cwiseMax(PV2.cwiseAbs());
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
    if (ImGui::CollapsingHeader("Curvature", ImGuiTreeNodeFlags_DefaultOpen))
    {
      static int e=-1;
      if (ImGui::RadioButton("First Principal Curvature",&e,0))
      {
        igl::colormap(igl::COLOR_MAP_TYPE_TURBO,PV1,true,C);
        viewer.data().set_colors(C);
      }
      if (ImGui::RadioButton("Second Principal Curvature",&e,1))
      {
        igl::colormap(igl::COLOR_MAP_TYPE_TURBO,PV2,true,C);
        viewer.data().set_colors(C);
      }
      if (ImGui::RadioButton("Mean Curvature",&e,2))
      {
        igl::colormap(igl::COLOR_MAP_TYPE_TURBO,H,true,C);
        viewer.data().set_colors(C);
      }
      if (ImGui::RadioButton("Gaussian Curvature",&e,3))
      {
        igl::colormap(igl::COLOR_MAP_TYPE_TURBO,K,true,C);
        viewer.data().set_colors(C);
      }
      if (ImGui::RadioButton("Abs. Max. Curvature",&e,4))
      {
        igl::colormap(igl::COLOR_MAP_TYPE_TURBO,AbsMaxCurvature,true,C);
        viewer.data().set_colors(C);
      }
    }
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
      igl::stb::write_image(std::to_string(num_screenshot)+screenshot_file,R,G,B,A);
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
