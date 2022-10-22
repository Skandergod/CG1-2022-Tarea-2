#include "Application.h"
#include <imgui.h>
#include <imfilebrowser.h>
#include <../../EasyDIPAPI/EasyDIPAPI/EDpch.h>
#include <../../EasyDIPAPI/EasyDIPAPI/Loaders.h>
#include <fstream>
#include <../../EasyDIPAPI/EasyDIPAPI/Object.h>
#include <glm/gtc/constants.hpp>
#include "GLFW/glfw3.h"
#include <cstring>

#define RGB(x) (x/255.f)
Object* hi;
glm::vec3 rotation(0.0f);
glm::vec3 scale(1.0f);
glm::vec3 translation(0.0f);
glm::vec3 origin(0.0f, 0.0f, -0.1f);
glm::vec3 destiny(0.0f, 0.0f, 100.0f);
glm::vec3 colorRelleno(0.7f, 0.7f, 0.7f);
glm::vec3 colorLineas(0.7f, 0.7f, 0.7f);
glm::vec3 colorVertices(0.7f, 0.7f, 0.7f);
glm::vec3 colorFondo(0.0f, 0.0f, 0.0f);
std::string Dir;

const float pi = 3.14159265358979323846264338327950288;
bool antialising = true;
bool zBuffer = true;
bool lines = true;
bool points = true;
bool fill = true;
bool FrontFaceCulling = true;
bool BackFaceCulling = false;
bool Normals = true;
float FPS = 0;
float realFPS = 0;
float color[3];

Application::Application() {

	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
	{
		__debugbreak();
		return;
	}
	
	
		glfwWindowHint(GLFW_SAMPLES, 4);

	
	// Decide GL+GLSL versions
#if __APPLE__
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context
	 window = glfwCreateWindow(800, 600, "Proyecto 2 - Daniel Da Costa", NULL, NULL);
	 if (window == NULL) {
		__debugbreak();
		return;
	}
	
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync


	bool err = gladLoadGL() == 0;

	printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		__debugbreak();
		return;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	bwShader = new Shader("bw.vert", "bw.frag");

	//CG::Model model = CG::Load("../Models/modelo.obj", );
	//models.push_back(model);
	//	Scene.LoadModel("pelota.obj")

	//	Scene.add("pelota");

	//Scene.Save("escena_de_prueba");
	//Scene.Load("escena_de_prueba.graficas");
	//
	//void Scene::Load(std::string path) {
	//	LoadScene(path);
	//}

	// (optional) set browser properties
	//fileDialog.SetTitle("title");
	//fileDialog.SetTypeFilters({ ".jpg", ".png", ".jpeg" });
	Init();
}

Application::~Application() {

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	//delete img;
	//delete composite;
	//delete bw;
}


void Application::MainLoop()
{

	double lastTime = glfwGetTime();
	int nbFrames = 0;
	float t1, t2, elapse;

	while (!glfwWindowShouldClose(window))
	{
		double currentTime = glfwGetTime();
		nbFrames++;

		t1 = glfwGetTime();
		if (currentTime - lastTime >= 5.0) { // If last prinf() was more than 1 sec ago
		// printf and reset timer
			FPS = double(nbFrames) / 5.0;
			nbFrames = 0;
			lastTime += 5.0;
		}


		glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
		if (hi) {
			hi->proyec((float)(windowWidth / windowHeight));
		}
		glViewport(0, 0, windowWidth, windowHeight);
		glClearColor(colorFondo.x, colorFondo.y, colorFondo.z, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (ImGui::IsKeyPressed(GLFW_KEY_O)) {
			std::cin >> Dir;
		}

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		ImGui();

		// Rendering
		ImGui::Render();
		Render();


		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		t2 = glfwGetTime();
		elapse = t2 - t1;

		realFPS = 1 / elapse;

	}
}

void Application::Render()
{

	//Object *object = new Object();
	if (bwShader && hi) {
		bwShader->use();
		glActiveTexture(0);
		glEnable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_3D, texId);
		bwShader->setMat4("rotMat", hi->rotateMatrix);
		bwShader->setMat4("transMat", hi->translateMatrix);
		bwShader->setMat4("scaleMat", hi->scaleMatrix);
		bwShader->setMat4("modelMatrix", hi->matrixModel);
		bwShader->setMat4("viewMatrix", hi->matrixView);
		bwShader->setMat4("proyecMatrix", hi->matrixProyec);
		bwShader->setInt("tex", 0);
		bwShader->setFloat("test", test);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (BackFaceCulling && FrontFaceCulling) {
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT_AND_BACK);
		}
		else {
			if (BackFaceCulling) {
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);
			}
			if (FrontFaceCulling) {
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
			}
		}

		if (fill) {
			bwShader->setVec3("color", glm::vec3(colorRelleno.x, colorRelleno.y, colorRelleno.z));
			//bwShader->setMat4("modelMatrix", hi->modelMatrix);
			hi->Bind();
			hi->Draw();
		}

		if (antialising) {
			
			//bwShader->setMat4("modelMatrix", hi->modelMatrix);
			
			glEnable(GL_MULTISAMPLE);
		}
		else {

			glDisable(GL_MULTISAMPLE);

		}

		if (zBuffer) {

			//bwShader->setMat4("modelMatrix", hi->modelMatrix);
			glEnable(GL_DEPTH_TEST);
		}
		else {

			glDisable(GL_DEPTH_TEST);

		}

		if (lines) {
			bwShader->setVec3("color", glm::vec3(colorLineas.x, colorLineas.y, colorLineas.z));
			hi->Bind();
			hi->DrawLines();
		}
		if (points) {
			bwShader->setVec3("color", glm::vec3(colorVertices.x, colorVertices.y, colorVertices.z));
			hi->Bind();
			hi->DrawPoints();
		}
		if (Normals) {
			hi->drawNormals();
		}
	}
}

void Application::ImGui()
{ 

	ImGui::SliderFloat("test", &test, 0, 1);

	ImGui::Begin("Convolution Editor");


	ImGui::Text("Color button with Picker:");
	ImGui::SameLine(); HelpMarker("With the ImGuiColorEditFlags_NoInputs flag you can hide all the slider/text inputs.\nWith the ImGuiColorEditFlags_NoLabel flag you can pass a non-empty label which will only be used for the tooltip and picker popup.");


	static std::vector<const char*> names = {
		"0,0",
		"1,0",
		"2,0",
	};



	for (size_t yy = 0, nn = 0; yy < heightConv; yy++)
	{
		for (size_t xx = 0; xx < widthConv; xx++, nn++)
		{
			ImGui::ColorEdit4(names[nn], (float*)&color[yy][xx], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
			ImGui::SameLine();
		}
		ImGui::NewLine();
	}

	ImGui::Text("FPS Cada 5 Segundos: %f", FPS);
	ImGui::Text("FPS: %f", realFPS);
	/*
	if (texOGImg)
	{
		//ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
	}*/
	/*
	if (ImGui::Button("recompile"))
	{
		//delete bwShader;
		//std::string vert = Shader::GetSrcFromFile("bw.vert");
		//std::string frag = Shader::GetSrcFromFile("bw.frag");


		//bwShader = Shader::FromString(vert.c_str(), frag.c_str());
		//std::cout << "recompiled" << std::endl;

		//std::unique_ptr<RawData> negative{ EDNegativeHA(img->data, img->GetWidth(), img->GetHeight()) };
		//texId = GetTexture(negative.get(), img->GetWidth(), img->GetHeight());


		delete bwShader;
		bwShader = new Shader("bw.vert", "bw.frag");


	}
	*/
	/*if (ImGui::ColorEdit3("color 1", &color,0)) {
		std::cout << color[0] << ","<< color[1] << ","<< color[2] << std::endl;
	}*/
	//if (ImGui::Button("Save Image"))
	//{
	//	ImGui::SameLine();
	//	//ImGui::InputText("imgFile")
	//}

	//if (ImGui::Button("Load Image"))
	//{
		//fileDialog.Open();
	//}
		//fileDialog.Display();


	/*if (filedialog.hasselected())
	{
		std::cout << "selected filename" << filedialog.getselected().string() << std::endl;
		
		filedialog.close();
		filedialog.clearselected();
	}*/
	if (ImGui::Checkbox("Vertices", &points)) {

	}

	if (ImGui::Checkbox("Front Face Culling", &FrontFaceCulling)) {

	}

	if (ImGui::Checkbox("Back Face Culling", &BackFaceCulling)) {

	}

	if (ImGui::Checkbox("Lineas", &lines)) {

	}
	if (ImGui::Checkbox("Relleno", &fill)) {

	}
	if (ImGui::Checkbox("Normals", &Normals)) {

	}

	if (ImGui::Checkbox("Antialising", &antialising)) {

	}

	if (ImGui::Checkbox("zBuffer", &zBuffer)) {

	}

	if (ImGui::Button("Load .obj"))
	{
		hi = new Object();
		CG::LoadObj(Dir, hi);
		/*for (int i = 0; i < hi->vertex.size(); i++) {
			hi->vertex[i];
		}*/
		hi->Init();
		//fileDialog.Open();
		hi->proyec((float)(windowWidth / windowHeight));
	}

	if (ImGui::Button("Despliegue"))
	{
		//CG::Load("../Objects/Object.off");
		//fileDialog.Open();
		

	}
		
	if (ImGui::SliderFloat("Rot x", &rotation.x, -pi, pi, "%.4f", 2.0f) && hi != NULL) {
		hi->Rotation(rotation);
		hi->modelMatrix();
	}

	if (ImGui::SliderFloat("Rot y", &rotation.y, -pi, pi, "%.4f", 2.0f) && hi != NULL) {
		hi->Rotation(rotation);
		hi->modelMatrix();
	}

	if (ImGui::SliderFloat("Rot z", &rotation.z, -pi, pi, "%.4f", 2.0f) && hi != NULL) {
		hi->Rotation(rotation);
		hi->modelMatrix();
	}


	if (ImGui::SliderFloat("Scale x", &scale.x, -10.0f, 10.0f, "%.4f", 2.0f) && hi != NULL) {
		hi->Scale(scale);
		hi->modelMatrix();
	}

	if (ImGui::SliderFloat("Scale y", &scale.y, -10.0f, 10.0f, "%.4f", 2.0f) && hi != NULL) {
		hi->Scale(scale);
		hi->modelMatrix();
	}

	if (ImGui::SliderFloat("Scale z", &scale.z, -10.0f, 10.0f, "%.4f", 2.0f) && hi != NULL) {
		hi->Scale(scale);
		hi->modelMatrix();
	}

	if (ImGui::SliderFloat("tras x", &translation.x, -20.0f, 20.0f, "%.4f", 2.0f) && hi != NULL) {
		hi->Translation(translation);
		hi->modelMatrix();
	}

	if (ImGui::SliderFloat("tras y", &translation.y, -20.0f, 20.0f, "%.4f", 2.0f) && hi != NULL) {
		hi->Translation(translation);
		hi->modelMatrix();
	}

	if (ImGui::SliderFloat("tras z", &translation.z, -20.0f, 20.0f, "%.4f", 2.0f) && hi != NULL) {
		hi->Translation(translation);
		hi->modelMatrix();
	}

	if (ImGui::SliderFloat("Lineas Color R", &colorLineas.x, 0.0f, 1.0f, "%.04f", 2.0f) && hi != NULL) {

	}

	if (ImGui::SliderFloat("Lineas Color G", &colorLineas.y, 0.0f, 1.0f, "%.04f", 2.0f) && hi != NULL) {

	}

	if (ImGui::SliderFloat("Lineas Color B", &colorLineas.z, 0.0f, 1.0f, "%.04f", 2.0f) && hi != NULL) {

	}

	if (ImGui::SliderFloat("Relleno Color R", &colorRelleno.x, 0.0f, 1.0f, "%.04f", 2.0f) && hi != NULL) {
		
	}

	if (ImGui::SliderFloat("Relleno Color G", &colorRelleno.y, 0.0f, 1.0f, "%.04f", 2.0f) && hi != NULL) {
		
	}

	if (ImGui::SliderFloat("Relleno Color B", &colorRelleno.z, 0.0f, 1.0f, "%.04f", 2.0f) && hi != NULL) {
	
	}

	if (ImGui::SliderFloat("Vertices Color R", &colorVertices.x, 0.0f, 1.0f, "%.04f", 2.0f) && hi != NULL) {

	}

	if (ImGui::SliderFloat("Vertices Color G", &colorVertices.y, 0.0f, 1.0f, "%.04f", 2.0f) && hi != NULL) {

	}

	if (ImGui::SliderFloat("Vertices Color B", &colorVertices.z, 0.0f, 1.0f, "%.04f", 2.0f) && hi != NULL) {

	}

	if (ImGui::SliderFloat("Fondo Color R", &colorFondo.x, 0.0f, 1.0f, "%.04f", 2.0f) && hi != NULL) {

	}

	if (ImGui::SliderFloat("Fondo Color G", &colorFondo.y, 0.0f, 1.0f, "%.04f", 2.0f) && hi != NULL) {

	}

	if (ImGui::SliderFloat("Fondo Color B", &colorFondo.z, 0.0f, 1.0f, "%.04f", 2.0f) && hi != NULL) {

	}

	if (ImGui::Button("Borrar"))
	{
		//CG::Load("../Objects/Object.off");
		//fileDialog.Open();
		
		delete hi;
		hi = nullptr;


	}
	/*if (hi != nullptr) {
		hi->Bind();
		hi->Draw();
	}*/
		//fileDialog.Display();

	ImGui::End();

}

void Application::Init() {}

void Application::HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

