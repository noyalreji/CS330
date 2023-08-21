#include "Pyramid.h"


void Pyramid::Initialize()
{
}

void Pyramid::Update(float deltaTime)
{
//	Transform = glm::rotate(Transform, glm::radians(45.f) * deltaTime, glm::vec3(0, 1, 0));
//	Transform = glm::rotate(Transform, glm::radians(45.f) * deltaTime, glm::vec3(1, 0, 0));
}

void Pyramid::Draw(const SceneParameters& sceneParms) {

    Shader* lastShader = nullptr;

    for (auto& model : this->models) {
        auto* shader = model.GetShader();
        auto* mesh = model.GetMesh();

        if (shader != lastShader) {
            // Bind the shader and set common matrices and lighting parameters
            shader->Bind();
            shader->SetMat4("projection", sceneParms.ProjectionMatrix);
            shader->SetMat4("view", sceneParms.ViewMatrix);
            shader->SetVec3("camPos", sceneParms.CamPosition); // Camera position for specular

            // Set lighting parameters for point lights
            for (auto i = 0; i < sceneParms.Lights.size() && i < MAXPOINTLIGHTS; i++) {
                std::string baseUniformName = "pointLights[" + std::to_string(i) + "]";
                shader->SetVec3(baseUniformName + ".Position", sceneParms.Lights[i].Position);
                shader->SetVec3(baseUniformName + ".AmbientColor", sceneParms.Lights[i].AmbientColor);
                shader->SetVec3(baseUniformName + ".DiffuseColor", sceneParms.Lights[i].DiffuseColor);
                shader->SetVec3(baseUniformName + ".SpecColor", sceneParms.Lights[i].SpecColor);
                shader->SetFloat(baseUniformName + ".ConstantVar", sceneParms.Lights[i].ConstantVar);
                shader->SetFloat(baseUniformName + ".LinearVar", sceneParms.Lights[i].LinearVar);
                shader->SetFloat(baseUniformName + ".QuadVar", sceneParms.Lights[i].QuadVar);
            }

            // Set lighting parameters for directional light
            shader->SetVec3("dirLight.Direction", sceneParms.DirLight.Direction);
            shader->SetVec3("dirLight.Diffuse", sceneParms.DirLight.DiffuseColor);
            shader->SetVec3("dirLight.Ambient", sceneParms.DirLight.AmbientColor);
            shader->SetVec3("dirLight.Specular", sceneParms.DirLight.SpecColor);
        }

        // Set the model matrix and draw the mesh
        shader->SetMat4("model", Transform * mesh->Transform);
        mesh->Draw();

        lastShader = shader;
    }
}


void Pyramid::createShaders()
{
	Path shaderPath = std::filesystem::current_path() / "assets" / "shaders";
	Path textPath = std::filesystem::current_path() / "assets" / "textures";

	this->basicUnlitShader = std::make_shared<Shader>(
		shaderPath / "basicLightNew.vert",
		//		shaderPath / "basicLightNew.frag");
		//		shaderPath / "basicLightNewDirection.frag");
		// shaderPath / "basicLightNewPoint.frag");
		shaderPath / "basicLightNewMulti.frag");


	auto brickTexture = std::make_shared<Texture>(textPath / "sand.jpg");
	this->basicUnlitShader->AddTexture(brickTexture);
//	glActiveTexture(GL_TEXTURE0);
}


void Pyramid::ProcessLighting(SceneParameters& sceneParms) {

	// return as this is does not process any light
	return;
}

void Pyramid::createMesh()
{

	auto pyramid = std::make_shared<Mesh>(PredefinedShapes::pyramidVertices, 
		PredefinedShapes::pyramidElements,
		glm::vec3(0.5f, 0.5f, 0.5f));

	auto& pyramidModel = models.emplace_back(pyramid, basicUnlitShader);

//	pyramidModel.GetMesh()->Transform = glm::scale(pyramidModel.GetMesh()->Transform, glm::vec3(1.0f, 1.0f, 1.0f));
//	pyramidModel.GetMesh()->Transform = glm::rotate(pyramidModel.GetMesh()->Transform, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}