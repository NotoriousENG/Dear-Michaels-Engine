#include "UComponent.h"
#include "Actors/AActor.h"
#include "imgui.h"

UComponent::UComponent(std::shared_ptr<AActor> owner)
{
	this->owner = owner;
}

void UComponent::Init()
{
}

void UComponent::Tick(float delta)
{
}

bool UComponent::ShowInspector()
{
	ImGui::Text("____________________________________________________________________");
	ImGui::Text("%s:", name.c_str());
	
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 0, 0, 1));
	ImGui::SameLine();
	if (ImGui::Button("X"))
	{
		this->GetOwner()->RemoveComponent(name);
		ImGui::PopStyleColor();
		return false;
	}
	ImGui::PopStyleColor();
	ImGui::Checkbox("Active", &this->isActive);
	return true;
}
