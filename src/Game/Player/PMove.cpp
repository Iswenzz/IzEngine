#include "PMove.hpp"
#include "Modules/Modules.hpp"
#include "Game/Definitions/Functions.hpp"

namespace IW3SR
{
	void PMove::FinishMove(usercmd_s* cmd)
	{
		ModuleCallback(OnFinishMove, cmd);
		CL_FinishMove_h(cmd);
	}
}