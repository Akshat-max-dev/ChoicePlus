#include"ProjectExplorer.h"

#include<imgui.h>
#include"FontAwesome.h"

namespace ChoicePlus
{
	void ProjectExplorer::Draw()
	{
		ImGui::Begin(ICON_FK_FOLDER_OPEN_O " Project Explorer");
		ImGui::End();
	}
}