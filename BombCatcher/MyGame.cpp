#include "MyDirectX.h"


const string APPTITLE = "Bomb Catcher Game";
const int SCREENW = 1024;
const int SCREENH = 768;
LPDIRECT3DSURFACE9 bomb_surf = NULL;
LPDIRECT3DSURFACE9 bucket_surf = NULL;

struct BOMB
{
	float x, y;
	void reset()
	{
		x = (float)(rand() % (SCREENW - 128));
		y = 0;
	}
};
BOMB bomb;

struct BUCKET
{
	float x, y;
};
BUCKET bucket;

int score = 0;
int vibrating = 0;

bool Game_Init(HWND window)
{
	Direct3D_Intit(window, SCREENW, SCREENH, false);
	DirectInput_Init(window);
	bomb_surf = LoadSurface("bomb.bmp");
	if (!bomb_surf)
	{
		MessageBox(window, "Error loading bomb", "Error", 0);
		return false;
	}
	bucket_surf = LoadSurface("bucket.bmp");

	if (!bucket_surf)
	{
		MessageBox(window, "Error loading bucket", "Error", 0);
		return false;
	}

	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

	srand((unsigned int)time(NULL));
	bomb.reset();

	bucket.x = 500;
	bucket.y = 630;
	return true;
}

void Game_Run(HWND window)
{
	if (!d3ddev)
	{
		return;
	}

	DirectInput_Update();

	bomb.y += 2.0f;

	if (bomb.y > SCREENH)
	{
		MessageBox(0, "Oh no,the bomb exploded!!", "You Stink", 0);
		gameover = true;
	}

	int mx = Mouse_X();
	if (mx < 0)
	{
		bucket.x -= 6.0f;
	}
	else if (mx > 0)
	{
		bucket.x += 6.0f;
	}

	if (Key_Down(DIK_LEFT))
	{
		bucket.x -= 6.0f;
	}
	else if (Key_Down(DIK_RIGHT))
	{
		bucket.x += 6.0F;
	}

	if (XInput_Controller_Found())
	{
		if (controllers[0].sThumbLX < -5000)
		{
			bucket.x -= 6.0f;
		}
		else if (controllers[0].sThumbLX > 5000)
		{
			bucket.x += 6.0f;
		}

		if (controllers[0].bLeftTrigger > 128)
		{
			bucket.x -= 6.0f;
		}
		else if (controllers[0].bRightTrigger > 128)
		{
			bucket.x += 6.0f;
		}

		if (controllers[0].wButtons&XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			bucket.x -= 6.0f;
		}
		else if (controllers[0].wButtons&XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			bucket.x += 6.0f;
		}

		if (controllers[0].wButtons&XINPUT_GAMEPAD_DPAD_LEFT)
		{
			bucket.x -= 6.0f;
		}
		else if (controllers[0].wButtons&XINPUT_GAMEPAD_DPAD_RIGHT)
		{
			bucket.x += 6.0f;
		}
	}

	if (vibrating > 0)
	{
		vibrating++;
		if (vibrating > 20)
		{
			XInput_Vibrate(0, 0);
			vibrating = 0;
		}
	}

	if (bucket.x < 0)
	{
		bucket.x = 0;
	}

	if (bucket.x > SCREENW - 128)
	{
		bucket.x = SCREENW - 128;
	}

	int cx = int(bomb.x + 64);
	int cy = int(bomb.y + 64);
	if (cx > bucket.x&&cx<bucket.x + 128 && cy>bucket.y&&cy < bucket.y + 128)
	{
		score++;
		std::ostringstream os;
		os << APPTITLE << " [SCORE " << score << "]";
		string scorestr = os.str();
		SetWindowText(window, scorestr.c_str());

		XInput_Vibrate(0, 65000);
		vibrating = 1;

		bomb.reset();
	}

	d3ddev->ColorFill(backbuffer, NULL, D3DCOLOR_ARGB(0, 0, 0, 0));

	if (d3ddev->BeginScene())
	{
		DrawSurface(backbuffer, bomb.x, bomb.y, bomb_surf);

		DrawSurface(backbuffer, bucket.x, bucket.y, bucket_surf);

		d3ddev->EndScene();
		d3ddev->Present(NULL, NULL, NULL, NULL);
	}

	if (Key_Down(DIK_SPACE) || Key_Down(DIK_ESCAPE))
	{
		gameover = true;
	}

	if (controllers[0].wButtons&XINPUT_GAMEPAD_BACK)
	{
		gameover = true;
	}
}

void Game_End()
{
	if (bomb_surf)
	{
		bomb_surf->Release();
	}

	if (bucket_surf)
	{
		bucket_surf->Release();
	}

	DirectInput_Shutdown();
	Direct3D_Shutdown();
}

















