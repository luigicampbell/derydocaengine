#include "EditorPch.h"
#include "Settings\EngineSettings.h"
#include "ObjectLibrary.h"
#include "Editor/Execution/CommandLineArgs.h"
#include <iostream>
#include "UI\FontFace.h"
#include "UI\Spritesheet.h"

#include "Scenes\SceneManager.h"
#include "Editor\EditorRenderer.h"
#include "Rendering\GraphicsAPI.h"
#include "SystemWindowingLayer.h"

int main(int argc, char* argv[])
{
	DerydocaEngine::SystemWindowingLayer::init();
	DerydocaEngine::SystemWindowingLayer::setVSync(true);
	DerydocaEngine::SystemWindowingLayer::setDepthTesting(true);
	DerydocaEngine::SystemWindowingLayer::setMultisampling(1, 16);

	// Load the settings
	auto settings = DerydocaEngine::Settings::EngineSettings(".\\engineSettings.yaml");

	// Parse the command line arguments
	DerydocaEngine::Editor::Execution::CommandLineArgs args = DerydocaEngine::Editor::Execution::CommandLineArgs(argc, argv);

	if (args.keyExists("processDirectory"))
	{
		std::vector<std::string> directoriesToProcess = args.getValues("processDirectory");

		for (auto it = directoriesToProcess.begin(); it != directoriesToProcess.end(); ++it)
		{
			DerydocaEngine::ObjectLibrary::getInstance().updateMetaFilesDirectory(it->c_str());
		}

		return 0;
	}

	if (args.keyExists("processFont"))
	{
		std::vector<std::string> processArgs = args.getValues("processFont");
		if (processArgs.size() != 3)
		{
			std::cout << "Unable to process font file. Incorrect argument count.\n";
			return 0;
		}

		DerydocaEngine::UI::FontFace face;
		face.loadFromFontFile(processArgs[0]);
		face.setFontSize((float)atof(processArgs[1].c_str()));
		face.saveToSerializedFile(processArgs[2]);
	}

	if (args.keyExists("processSpriteSheet"))
	{
		std::vector<std::string> processArgs = args.getValues("processSpriteSheet");
		if (processArgs.size() != 2)
		{
			std::cout << "Unable to process sprite sheet. Incorrect argument count.\n";
			return 0;
		}

		// Load the project file
		DerydocaEngine::ObjectLibrary::getInstance().loadEngineResources(settings.getEngineResourceDirectory());
		DerydocaEngine::ObjectLibrary::getInstance().loadProjectResources(processArgs[0]);

		// Process the sprite sheet
		DerydocaEngine::UI::SpriteSheet ss;
		ss.LoadFromDisk(processArgs[1]);
		ss.updateTexture();
		ss.saveToDisk(processArgs[1]);
	}

	// Get information about the project specified in the command arguments
	std::vector<std::string> projectArgs = args.getValues("project");
	auto projectDirectory = projectArgs.size() > 0 ?
		boost::filesystem::path(projectArgs.front()) :
		boost::filesystem::path();
	bool projectDirectoryIsDefined = projectArgs.size() > 0;

	// Get information about the scene specified in the command arguments
	std::vector<std::string> sceneArgs = args.getValues("scene");
	bool sceneIdIsDefined = sceneArgs.size() > 0;
	boost::uuids::uuid sceneId;
	if (sceneArgs.size() > 0)
	{
		boost::uuids::string_generator stringGen;
		sceneId = stringGen(sceneArgs.front().c_str());
	}

	// Create the renderer
	auto renderer = DerydocaEngine::Rendering::Renderer(DerydocaEngine::Editor::EditorRenderer::GetInstance());

	// Load the engine's resources
	DerydocaEngine::ObjectLibrary::getInstance().loadEngineResources(settings.getEngineResourceDirectory());

	// Load the project directory
	if (projectDirectoryIsDefined)
	{
		if (boost::filesystem::exists(projectDirectory))
		{
			// Load the project file
			DerydocaEngine::ObjectLibrary::getInstance().loadProjectResources(projectDirectory);
		}
		else
		{
			std::cout << "Invalid project directory: " << projectDirectory << std::endl;
		}
	}
	else
	{
		std::cout << "No project was specified. Please supply the argument -project [MyProjectPath]\n";
	}

	renderer.init();

	// Load a scene if it is defined
	if (sceneIdIsDefined)
	{
		DerydocaEngine::Scenes::SceneManager::getInstance().loadScene(sceneId);
	}

	// Start the render loop
	return renderer.runRenderLoop();
}
