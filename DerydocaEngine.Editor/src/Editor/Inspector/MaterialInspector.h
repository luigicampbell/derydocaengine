#pragma once
#include "EditorPch.h"
#include "InspectorRendererFactory.h"
#include "Rendering\Material.h"
#include "Resources\MaterialResource.h"

namespace DerydocaEngine::Editor::Inspector
{

	class MaterialResourceInspector : public InspectorRenderer
	{
		virtual void render(std::shared_ptr<Object> object)
		{
			auto resource = std::static_pointer_cast<Resources::MaterialResource>(object);

			ImGui::Text("Material");
			ImGui::Text(resource->getName().c_str());
		}
	};

}
