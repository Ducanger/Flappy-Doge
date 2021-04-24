#include "game.h"
#include <iostream>
#include "lib.h"

void game::takeInput()
{
    while(SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            UserInput.Type = input::QUIT;
            quit = true;
        }
        else if ( e.type == SDL_MOUSEBUTTONDOWN || (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_UP) && e.key.repeat == 0) )
        {
            UserInput.Type = input::PLAY;
        }
		else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE && e.key.repeat == 0)
		{
			UserInput.Type = input::PAUSE;
		}
    }
}

game::game()
{
    initGraphic();
	//shiba.init(false);
    pipe.init();
    land.init();
    sound.init();
}

game::~game()
{

    shiba.Free();

    pipe.Free();

    land.Free();

    sound.Free();

    free();

    releaseGraphic();
}

void game::releaseGraphic()
{
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    SDL_DestroyRenderer( gRenderer );
    gRenderer = NULL;

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

bool game::initGraphic()
{
    //Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window

		gWindow = SDL_CreateWindow( "Flappy Doge", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		
		//SDL_Surface* icon = IMG_Load("image/shiba.png");
		//SDL_SetWindowIcon(gWindow, icon);


		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

                //Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}


void game::display()
{

    SDL_RenderPresent(gRenderer);
    SDL_RenderClear(gRenderer);
}

void game::renderScore()
{
    string s = to_string(score);
	signed char len = s.length();
	LTexture image;

	for (signed char i = 0; i < len; i++)
	{
		signed char number = s[i] - '0';
		if (number == 1)
		{
			image.Load("number/large/1.png", 1);
		}
		else if (number == 2)
		{
			image.Load("number/large/2.png", 1);
		}
		else if (number == 3)
		{
			image.Load("number/large/3.png", 1);
		}
		else if (number == 4)
		{
			image.Load("number/large/4.png", 1);
		}
		else if (number == 5)
		{
			image.Load("number/large/5.png", 1);
		}
		else if (number == 6)
		{
			image.Load("number/large/6.png", 1);
		}
		else if (number == 7)
		{
			image.Load("number/large/7.png", 1);
		}
		else if (number == 8)
		{
			image.Load("number/large/8.png", 1);
		}
		else if (number == 9)
		{
			image.Load("number/large/9.png", 1);
		}
		else 
		{
			image.Load("number/large/0.png", 1);
		}
		image.Render((SCREEN_WIDTH - (image.getWidth() * len + (len - 1) * 10)) / 2 + (i + 30) * i, 100);
	}
	image.free();
}

void game::renderScoreOver()
{
	string s = to_string(score);
	signed char len = s.length();
	LTexture image;

	for (signed char i = len - 1; i >= 0; i--)
	{
		signed char number = s[i] - '0';
		if (number == 1)
		{
			image.Load("number/small/1.png", scaleNumberS);
		}
		else if (number == 2)
		{
			image.Load("number/small/2.png", scaleNumberS);
		}
		else if (number == 3)
		{
			image.Load("number/small/3.png", scaleNumberS);
		}
		else if (number == 4)
		{
			image.Load("number/small/4.png", scaleNumberS);
		}
		else if (number == 5)
		{
			image.Load("number/small/5.png", scaleNumberS);
		}
		else if (number == 6)
		{
			image.Load("number/small/6.png", scaleNumberS);
		}
		else if (number == 7)
		{
			image.Load("number/small/7.png", scaleNumberS);
		}
		else if (number == 8)
		{
			image.Load("number/small/8.png", scaleNumberS);
		}
		else if (number == 9)
		{
			image.Load("number/small/9.png", scaleNumberS);
		}
		else
		{
			image.Load("number/small/0.png", scaleNumberS);
		}
		image.Render(260 - image.getWidth() * (len - i - 1) * 0.75 - 5 * (len - i - 1), 268);
	}

	image.free();
}


void game::renderBestScore()
{
	ifstream fileIn("data/bestScore.txt");
	fileIn >> bestScore; 
	ofstream fileOut("data/bestScore.txt", ios::trunc);
	if (score > bestScore)
	{
		bestScore = score;
	}
	string s = to_string(bestScore);
	signed char len = s.length();
	LTexture image;

	for (signed char i = len-1; i >= 0; i--)
	{
		signed char number = s[i] - '0';
		if (number == 1)
		{
			image.Load("number/small/1.png", scaleNumberS);
		}
		else if (number == 2)
		{
			image.Load("number/small/2.png", scaleNumberS);
		}
		else if (number == 3)
		{
			image.Load("number/small/3.png", scaleNumberS);
		}
		else if (number == 4)
		{
			image.Load("number/small/4.png", scaleNumberS);
		}
		else if (number == 5)
		{
			image.Load("number/small/5.png", scaleNumberS);
		}
		else if (number == 6)
		{
			image.Load("number/small/6.png", scaleNumberS);
		}
		else if (number == 7)
		{
			image.Load("number/small/7.png", scaleNumberS);
		}
		else if (number == 8)
		{
			image.Load("number/small/8.png", scaleNumberS);
		}
		else if (number == 9)
		{
			image.Load("number/small/9.png", scaleNumberS);
		}
		else
		{
			image.Load("number/small/0.png", scaleNumberS);
		}
		image.Render(260 - image.getWidth()*(len-i-1)*0.75 - 5*(len - i - 1), 315);
	}
	image.free();
	fileOut << bestScore; 
	fileIn.close();
	fileOut.close();
}

void game::renderMessage()
{
	LTexture image;
	image.Load("image/message.png", 1);
	image.Render((SCREEN_WIDTH - image.getWidth()) / 2, 180);
	image.free();
}

void game::renderBackground()
{
	LTexture image;
	image.Load("image/background.png", 1);
	image.Render(0, 0);
	image.free();
}

void game::renderBackgroundNight()
{
	LTexture image;
	image.Load("image/background-night.png", 1);
	image.Render(0, 0);
	image.free();
}

void game::renderLand()
{
	LTexture image;
	image.Load("image/land.png", 1);
	image.Render((SCREEN_WIDTH - image.getWidth()) / 2, SCREEN_HEIGHT- image.getHeight());
	image.free();
}

void game::resume()
{
	LTexture image;
	image.Load("image/resume.png", 1);
	image.Render(SCREEN_WIDTH - 50, 20);
	image.free();
}

void game::pause()
{
	LTexture image;
	image.Load("image/pause.png", 1);
	image.Render(SCREEN_WIDTH - 50, 20);
	image.free();
}

void game::renderPauseTab()
{
	LTexture image;
	image.Load("image/pauseTab.png", 1);
	image.Render((SCREEN_WIDTH - image.getWidth()) / 2, 230);
	image.free();
}

void game::lightTheme()
{
	LTexture image;
	image.Load("image/shiba.png", 0.8);
	image.Render(105, 315);
	image.free();
}

void game::darkTheme()
{
	LTexture image;
	image.Load("image/shiba-dark.png", 0.8);
	image.Render(105, 315);
	image.free();
}

void game::nextButton()
{
	LTexture image;
	image.Load("image/nextRight.png", 1);
	image.Render(149, 322);
	image.Load("image/nextLeft.png", 1);
	image.Render(88, 322);
	image.free();
}

bool game::changeTheme()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	if (((x > 149 && x < 149+13)  || (x > 88 && x < 88 + 13)) && (y > 322 && y < 322 + 16))
	{
		return true;
	}
	return false;
}

void game::renderGameOver()
{
	LTexture image;
	image.Load("image/gameOver.png", 1);
	image.Render((SCREEN_WIDTH - image.getWidth()) / 2, 150);
	image.free();
}

void game::renderMedal()
{
	LTexture image;
		
	if (score > 20 && score <= 50)
	{
		image.Load("medal/silver.png", scaleNumberS);
	}
	else if (score > 50)
	{
		image.Load("medal/gold.png", scaleNumberS);
	}
	else 
	{
		image.Load("medal/honor.png", scaleNumberS);
	}
	image.Render(82, 275);

	image.free();
}

void game::replay()
{
	LTexture image;
	image.Load("image/replay.png", 1);
	image.Render((SCREEN_WIDTH - image.getWidth()) / 2, 380);
	image.free();
}

bool game::checkReplay()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	if (x > (SCREEN_WIDTH - 100)/2 && x < (SCREEN_WIDTH + 100) / 2 && y > 380 && y < 380 + 60)
	{
		return true;
	}
	return false;
}

void game::Restart()
{
    lost = false;
    score = 0;
    shiba.resetTime();
}