#include "InputHandler.h"

#include "FireBallGenerator.h"


bool InputHandler::KeyPressed(const KeyEvent& arg)
{
	printf("key press %c\n", arg.wc);
	switch(arg.code)
	{
	case KEY_Q:
		render->SetTurnLeftCam(true);
		break;
	case KEY_E:
		render->SetTurnRightCam(true);
		break;
	case KEY_W:
		render->SetMoveForwardCam(true);
		break;
	case KEY_1:
		player->torchPos.x += 0.1;
		break;
	case KEY_2:
		player->torchPos.x -= 0.1;
		break;
	case KEY_3:
		player->torchPos.y += 0.1;
		break;
	case KEY_4:
		player->torchPos.y -= 0.1;
		break;
	case KEY_5:
		player->torchPos.z += 0.1;
		break;
	case KEY_6:
		player->torchPos.z -= 0.1;
		break;
	case KEY_S:
		render->SetMoveBackCam(true);
		break;
	case KEY_D:
		render->SetMoveRightCam(true);
		break;
	case KEY_A:
		render->SetMoveLeftCam(true);
		break;
	case KEY_SHIFT:
		render->SetMoveDownCam(true);
		break;
	case KEY_SPACE:
		render->SetMoveUpCam(true);
		break;
	case KEY_LEFT:
		player->SetTurnLeft(true);
		break;
	case KEY_RIGHT:
		player->SetTurnRight(true);
		break;
	case KEY_UP:
		player->SetMoveForward(true);
		break;
	case KEY_DOWN:
		player->SetMoveBack(true);
		break;
	}
	
	return true;
}

bool InputHandler::KeyReleased(const KeyEvent& arg)
{
	switch (arg.code)
	{
	case KEY_Q:
		render->SetTurnLeftCam(false);
		break;
	case KEY_E:
		render->SetTurnRightCam(false);
		break;
	case KEY_W:
		render->SetMoveForwardCam(false);
		break;
	case KEY_S:
		render->SetMoveBackCam(false);
		break;
	case KEY_D:
		render->SetMoveRightCam(false);
		break;
	case KEY_A:
		render->SetMoveLeftCam(false);
		break;
	case KEY_SHIFT:
		render->SetMoveDownCam(false);
		break;
	case KEY_SPACE:
		render->SetMoveUpCam(false);
		break;
	case KEY_LEFT:
		player->SetTurnLeft(false);
		break;
	case KEY_RIGHT:
		player->SetTurnRight(false);
		break;
	case KEY_UP:
		player->SetMoveForward(false);
		break;
	case KEY_DOWN:
		player->SetMoveBack(false);
		break;
	}
	
	return true;
}

bool InputHandler::MousePressed(const MouseEventClick& arg)
{
	if(arg.btn == MOUSE_LEFT)
	{
		if(render->NumPointLight() < 10)
		{
			auto pos = player->GetPosition();
			pos.y += 2;
			render->AddFireBallToRender(FireBallGenerator::Generate(pos, player->GetRotation(), 5000));
		}
	}
	else if (arg.btn == MOUSE_RIGHT)
	{
		if (render->NumPointLight() < 10)
		{
			auto pos = player->GetPosition();
			pos.y += 2;
			XMFLOAT3 rot = render->GetWorldCords(arg.x, arg.y);

			rot.x -= pos.x;
			rot.z -= pos.z;

			auto angle = XMVector3AngleBetweenVectors(
				XMVectorSet(rot.x, rot.y, rot.z, 0),
				XMVectorSet(0, 0, 1, 0)
			);
			rot.y = convertRadToDegree(XMVectorGetByIndex(angle, 1));
			if (rot.x < 0)
				rot.y = 360 - rot.y;

			rot.x = 0;
			rot.z = 0;
			render->AddFireBallToRender(FireBallGenerator::Generate(pos, rot, 2000));
		}
	}
	
	printf("mouse %d - %d\n", arg.x, arg.y);
	//printf("mouse %lf - %lf\n", x1, x2);
	//printf("vPickRayDir %lf - %lf - %lf\n", vPickRayDir.x, vPickRayDir.y, vPickRayDir.z);
	
	return true;
}

bool InputHandler::MouseMove(const MouseEvent& arg)
	{
		//printf("mouse %d - %d\n", arg.x, arg.y);
		return false;
	}
