#include "ObjectRenderer.hpp"

ObjectRenderer::ObjectRenderer(const ParsedObject& parsedObject)
	: m_translation(0, 0, 0),
	  m_proj(glm::ortho(-5.0f, 5.0f, -3.0f, 3.0f, -5.0f, 5.0f)),
	  m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
	  view_vec(0, 0, 0)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	m_VAO = std::make_unique<VertexArray>();
	m_VBO = std::make_unique<VertexBuffer>(parsedObject.getPositions().data(), parsedObject.getPositions().size() * sizeof(float));
	VertexBufferLayout layouts;
	layouts.push<float>(3); // size of data 
	// layouts.push<float>(3); // colors 
	// add color layout
	// add texture layout
	// layouts.push<float>(2);
	m_VAO->addBuffer(*m_VBO, layouts);
	m_IBO = std::make_unique<IndexBuffer>(parsedObject.getIndices().data(), parsedObject.getIndices().size());

	m_shader = std::make_unique<Shader>("res/shaders/Basic.shader");

	m_shader->bind();

	m_texture = std::make_unique<Texture>("res/texture/texture.png");
	m_texture->bind();
	m_shader->setUniform1i("u_Texture", 0);
}

void ObjectRenderer::onRender(ModelState& modelState)
{
	glm::mat4 model(1.0f);
	glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
	// glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Renderer renderer;
	m_texture->bind();
	model = glm::translate(model, m_translation); // move model. rotate have to be after move 
	model = glm::rotate(model, glm::radians(modelState.rotation_x), glm::vec3(0.0f, 1.0f, 0.0f)); // y set to 
	model = glm::rotate(model, glm::radians(modelState.rotation_y), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model,  modelState.scale);
	m_view = glm::translate(glm::mat4(1.0f), view_vec);
	glm::mat4 mvp = m_proj * m_view * model;
	m_shader->bind();
	m_shader->setUniformMat4f("u_MVP", mvp);
	clearError();
	renderer.draw(*m_VAO, *m_IBO, *m_shader);
	checkError();
}

void ObjectRenderer::onImGuiRender(int& fill)
{
	ImGui::SliderFloat3("translationA", &m_translation.x, -5.0f, 10.0f); // Edit 1 float using a slider from 0.0f to 1.0f
	// ImGui::SliderFloat3("view", &view_vec.x, 0.0f, 10.0f);				// Edit 1 float using a slider from 0.0f to 1.0f
	// ImGui::SliderFloat("rotate x", &m_rotation_x, 0.0f, 360.0f);
	// ImGui::SliderFloat("rotate y", &m_rotation_y, 0.0f, 360.0f);
	// ImGui::SliderFLoat3


}