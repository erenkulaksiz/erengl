#define GLFW_INCLUDE_NONE
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <ErenGL/Gui.h>
#include <ErenGL/Mesh.h>
#include <ErenGL/Camera.h>
#include <ErenGL/App.h>

Gui::Gui()
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
  ImGui_ImplOpenGL3_Init("#version 330");
  std::cout << "Gui created" << std::endl;
}

void Gui::shutdown()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void Gui::newFrame()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void Gui::render()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::drawGui(std::vector<Mesh *> *meshes, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, float fps, float deltaTime, bool *isPlaying)
{
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::Begin("scene");

  if (ImGui::Button(*isPlaying ? "pause" : "play"))
  {
    *isPlaying = !*isPlaying;
  }

  if (ImGui::CollapsingHeader("statistics"))
  {
    ImGui::Text("fps: %f", fps);
    ImGui::Text("deltaTime: %f", deltaTime * 1000.0f);
    ImGui::Text("meshes: %d", meshes->size());
  }

  if (ImGui::CollapsingHeader("world"))
  {
    static bool glLineRenderer = false;
    ImGui::Checkbox("Wireframe", &glLineRenderer);

    if (glLineRenderer)
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (ImGui::BeginListBox("##world", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
    {
      for (int i = 0; i < meshes->size(); i++)
      {
        const bool is_selected = (selectedMeshIndex == i);
        if (ImGui::Selectable((*meshes)[i]->getName().c_str(), is_selected))
        {
          if (selectedMeshIndex == i)
          {
            selectedMeshIndex = -1;
          }
          else
          {
            selectedMeshIndex = i;
          }
        }

        if (is_selected && selectedMeshIndex != -1)
        {
          ImGui::SetItemDefaultFocus();
          glm::vec3 currentPosition = (*meshes)[selectedMeshIndex]->getPosition();
          glm::vec3 currentRotation = (*meshes)[selectedMeshIndex]->getRotation();
          glm::vec3 currentScale = (*meshes)[selectedMeshIndex]->getScale();
          positionVec3[0] = currentPosition.x;
          positionVec3[1] = currentPosition.y;
          positionVec3[2] = currentPosition.z;
          rotationVec3[0] = currentRotation.x;
          rotationVec3[1] = currentRotation.y;
          rotationVec3[2] = currentRotation.z;
          scaleVec3[0] = currentScale.x;
          scaleVec3[1] = currentScale.y;
          scaleVec3[2] = currentScale.z;
          (*meshes)[i]->setIsSelected(true);
        }
        else
        {
          if ((*meshes)[i]->getIsSelected())
          {
            (*meshes)[i]->setIsSelected(false);
          }
        }
      }
      ImGui::EndListBox();
    }
    if (ImGui::Button("add"))
    {
    }
  }
  ImGui::End();

  if (selectedMeshIndex != -1)
  {
    ImGui::Begin("inspector");

    if ((*meshes)[selectedMeshIndex]->getIsCamera())
    {
      if (!(*meshes)[selectedMeshIndex]->getCamera()->isActiveCamera())
      {
        if (ImGui::Button("delete"))
        {
          (*meshes)[selectedMeshIndex]->deleteCamera();
          meshes->erase(meshes->begin() + selectedMeshIndex);
          selectedMeshIndex = -1;
          ImGui::End();
          return;
        }
      }
    }
    else
    {
      if (ImGui::Button("delete"))
      {
        (*meshes)[selectedMeshIndex]->deleteCamera();
        meshes->erase(meshes->begin() + selectedMeshIndex);
        selectedMeshIndex = -1;
        ImGui::End();
        return;
      }
    }

    if ((*meshes)[selectedMeshIndex]->getIsLightSource())
    {
      ImGui::SameLine();
      ImGui::Text("light source");
    }

    if ((*meshes)[selectedMeshIndex]->getIsCamera())
    {
      if (!(*meshes)[selectedMeshIndex]->getCamera()->isActiveCamera())
      {
        ImGui::SameLine();
        if (ImGui::Button("set active camera"))
        {
          for (int i = 0; i < meshes->size(); i++)
          {
            if ((*meshes)[i]->getIsCamera())
            {
              (*meshes)[i]->getCamera()->setActiveCamera(false);
            }
          }
          (*meshes)[selectedMeshIndex]->getCamera()->setActiveCamera(true);
        }
      }
      else
      {
        ImGui::SameLine();
        ImGui::Text("active camera");
      }
    }

    char meshName[128];
    strcpy(meshName, (*meshes)[selectedMeshIndex]->getName().c_str());
    if (ImGui::InputText("name", meshName, sizeof(meshName)))
    {
      (*meshes)[selectedMeshIndex]->setName(meshName);
    }

    ImGui::SeparatorText("transform");
    if (ImGui::DragFloat3("position", positionVec3, 0.1f, -1000.0f, 1000.0f, "%.1f"))
    {
      (*meshes)[selectedMeshIndex]->setPosition(glm::vec3(positionVec3[0], positionVec3[1], positionVec3[2]));
    }
    if (ImGui::DragFloat3("rotation", rotationVec3, 0.1f, -1000.0f, 1000.0f, "%.1f"))
    {
      if (rotationVec3[0] >= 360.0f)
      {
        rotationVec3[0] = 0.0f;
      }
      else if (rotationVec3[0] < 0.0f)
      {
        rotationVec3[0] = 360.0f;
      }

      if (rotationVec3[1] >= 360.0f)
      {
        rotationVec3[1] = 0.0f;
      }
      else if (rotationVec3[1] < 0.0f)
      {
        rotationVec3[1] = 360.0f;
      }

      if (rotationVec3[2] >= 360.0f)
      {
        rotationVec3[2] = 0.0f;
      }
      else if (rotationVec3[2] < 0.0f)
      {
        rotationVec3[2] = 360.0f;
      }

      (*meshes)[selectedMeshIndex]->setRotation(glm::vec3(rotationVec3[0], rotationVec3[1], rotationVec3[2]));
    }
    if (ImGui::DragFloat3("scale", scaleVec3, 0.1f, -1000.0f, 1000.0f, "%.1f"))
    {
      (*meshes)[selectedMeshIndex]->setScale(glm::vec3(scaleVec3[0], scaleVec3[1], scaleVec3[2]));
    }

    ImGui::End();
  }
}

void Gui::setMouseInteractions(bool value)
{
  mouseInteractions = value;
  if (mouseInteractions == false)
  {
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
  }
  else
  {
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
  }
}