//
//  main.cpp
//  EasyDIPBenchmark
//
//  Created by Leonardo Monsalve on 1/3/20.
//

#include "main.h"
#include <glad/glad.h>
//#include "EDImage.hpp"
#include "EasyDIPAPI/EDConvolution.h"

int main()
{

	// Setup window
	//glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
	{
		__debugbreak();
		return 1;
	}

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
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
	if (window == NULL){
		__debugbreak();
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync


	bool err = gladLoadGL() == 0;

	printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		__debugbreak();
		return 1;
	}

	EDInit();
	std::ifstream imgNames;
	imgNames.open("imgs.txt");


	if (!imgNames.is_open()) {
		std::cout << "couldn´t open imgs file \n";
		__debugbreak();
	}

	std::vector<std::string> convNames;
	std::string imgName;
	std::vector<EDBaseConvolution*> kernels;

	convNames.push_back("gaussian3x3");
	kernels.push_back(EDConvolution::CreateCustom(
		EDConvolution::Normalize({
			1,2,1,
			2,3,2,
			1,2,1
		}),
		{ 0,0,0,0 }, 3, 3, 1, 1));

	convNames.push_back("gaussian5x5");
	kernels.push_back(EDConvolution::CreateCustom(
		EDConvolution::Normalize({
			1,2,3,2,1,
			2,4,6,4,2,
			3,6,8,6,3,
			2,4,6,4,2,
			1,2,3,2,1
			}),
		{ 0,0,0,0 }, 5, 5, 2, 2) );

	convNames.push_back("sobelY3x3");
	auto sobelY = EDConvolution::CreateCustom(
		{
			-1, -2, 1,
			0, 0, 0,
			1, 2, 1,
		},
		{ 0,0,0,0 }, 3, 3, 1, 1);
	kernels.push_back(sobelY);

	convNames.push_back("sobelX3x3");
	auto sobelX = EDConvolution::CreateCustom(
		{
			-1 , 0, 1,
			-2 , 0, 2,
			-1 , 0, 1,
		},
		{ 0,0,0,0 }, 3, 3, 1, 1);
	kernels.push_back(sobelX);


	convNames.push_back("box3x3");
	kernels.push_back(EDConvolution::CreateCustom(
		EDConvolution::Normalize({
			1,1,1,
			1,1,1,
			1,1,1,
		}),
		{ 0,0,0,0 }, 3, 3, 1, 1));


	convNames.push_back("laplaciangausian3x3");

	kernels.push_back(EDConvolution::CreateCustom(
		{
			-1,-1,-1,
			-1,8,-1,
			-1,-1,-1,
		},
		{ 0,0,0,0 }, 3, 3, 1, 1));


	EDConvolution *prewittX = EDConvolution::CreateCustom({
		-1,0,1,
		-1,0,1,
		-1,0,1,
		}, {0,0,0,0}, 3,3,1,1);

	EDConvolution *prewittY = EDConvolution::CreateCustom({
		-1,-1,-1,
		0,0,0,
		1,1,1,
		}, { 0,0,0,0 }, 3, 3, 1, 1);


	while (std::getline(imgNames, imgName))
	{
		EDImage* img;

		if (!EDImage::TryLoad(imgName, img)) 
		{
			std::cout << stbi_failure_reason()<<std::endl;
			std::cout << "something is wrong";
			__debugbreak();
		}

		EDImage* sobelXImg = nullptr;
		EDImage* sobelYImg = nullptr;


		auto red = EDImage::GetUniqueColors(*img, 0);


		for (auto it = red.begin(); it != red.end(); it++){
			
			int num = (*it);
			
			std::cout << num <<"  times  "<< red.count(*it) << std::endl;
		}

		std::cout << EDImage::GetUniqueColors(*img, 1).size() << std::endl;
		std::cout << EDImage::GetUniqueColors(*img, 2).size() << std::endl;
		std::cout << EDImage::GetUniqueColors(*img) << std::endl;

		std::cin.ignore();





	/*	sobelXImg = EDImage::Operate(*img, *img, [](unsigned char* dest, unsigned char* a, unsigned char* b, int nChannels) {
				float color = clamp(255,0, a[0] * 0.2125 + a[1] * 0.7154 + a[2] * 0.0721);
				dest[0] = color;
				dest[1] = color;
				dest[2] = color;
		});

		EDImage::TrySave(*sobelXImg, "grey.png");
*/
		
		/*sobelXImg = sobelX->ApplyConvolution(*img, sobelXImg);
		sobelYImg = sobelY->ApplyConvolution(*img, sobelYImg);

		EDImage::TrySave(*sobelXImg, "sobelX.png");
		EDImage::TrySave(*sobelYImg, "sobelY.png");
		EDImage* sobelFinal = EDImage::Gradient(*sobelXImg, *sobelYImg);
		EDImage::TrySave(*sobelFinal, "sobelFinal.png");

		sobelYImg = prewittY->ApplyConvolution(*img, sobelYImg);
		EDImage::TrySave(*sobelYImg, "prewittY.png");

		sobelYImg = EDImage::Multiply(*sobelYImg, *sobelYImg, sobelYImg);
		EDImage::TrySave(*sobelYImg, "prewittYprewittY.png");

		sobelXImg = prewittX->ApplyConvolution(*img, sobelXImg);
		EDImage::TrySave(*sobelXImg, "prewittX.png");

		sobelXImg = EDImage::Multiply(*sobelXImg, *sobelXImg, sobelXImg);
		EDImage* prewitt = EDImage::Gradient(*sobelXImg, *sobelYImg);
		EDImage::TrySave(*prewitt, "prewittFinal.png");*/

		

		/*for (int ii= 0; ii < convolutions.size(); ii++)
		{
			destiny = convolutions[ii]->ApplyConvolution(*img, destiny);
			destiny->TrySave(*destiny, imgName + "_" + convNames[ii]+".png");
		}*/



		delete sobelXImg;
		delete sobelYImg;
	}
	
	
	
	//std::cin.ignore();
    return 0;
}
