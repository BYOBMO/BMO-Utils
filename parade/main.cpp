#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "CText.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string>
using namespace std;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
SDL_Joystick* gGameController = NULL;
//The window renderer
SDL_Renderer* gRenderer = NULL;
TTF_Font *gFont = NULL;
CText mText;

int exists(const char *name)
{
  struct stat   buffer;
  return (stat (name, &buffer) == 0);
}

bool init()
{
	//Initialization flag
	bool success = true;



	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{

		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 320, 240, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_RendererInfo info;
				SDL_GetRendererInfo(gRenderer, &info);

				printf("%s\n", info.name);

				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
				SDL_RenderSetLogicalSize(gRenderer, 320, 240);


				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}

			}
		}

		gFont = TTF_OpenFont("fonts/8bitoperator/8bitoperator.ttf", 36);
		if (gFont == NULL)
		{
			printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
			//success = false;
		}

		SDL_ShowCursor(0);
	}

	return success;
}

void ProcessVoice(char *message)
{
	FILE *cmd = NULL;

	string command = "/home/pi/bmos/scripts/speech-recog.sh";

	command += " -d ";
	command += "2";
	command += " -D ";
	command += "plughw:1";
	command += " -l ";
	command += "en";

#ifdef _WIN32
	cmd = _popen(command.c_str(), "r");
#else
	cmd = popen(command.c_str(), "r");
#endif

	fscanf(cmd, "\"%[^\"\n]\"", message);
/*	printf("Cmd: %s\n", command.c_str());
	printf("Mess: %s\n", message)*/;
	fclose(cmd);
}

void playVideo(char *filename)
{
	char vide[1024];
	sprintf(vide, "/home/pi/bmos/videos/%s.mp4", filename);
	char c2;
	SDL_Event e;

	pid_t pid = fork();

	if (pid == -1) {
		// When fork() returns -1, an error happened.
		perror("fork failed");
		return;
	}
	else if (pid == 0) {
		char *argv[] = {"omxplayer", "--layer", "10010", "-o", "alsa", "--no-keys", "--no-osd", vide, NULL};
		execvp("omxplayer", argv);
	}
	else {
		// When fork() returns a positive number, we are in the parent process
		// and the return value is the PID of the newly created child process.
		int status;
		usleep(600000);
		int wpid = 0;
		while(wpid == 0)
		{
			wpid = waitpid(pid, &status, WNOHANG);
			if (wpid == 0)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
					{

						switch (e.key.keysym.sym)
						{
						case SDLK_SPACE:
							system("/home/pi/bmos/dbuscontrol.sh stop");
							break;

						default:

							break;
						}
					}
				}
			}

			usleep(100000);
		}
	}
}

#undef main
int main(int argc, char **argv)
{
	char message[1000];
	strcpy(message, "");

	int counter = 150;

	std::string response = "You said: Hello there my name is Finn. It really is Finn. Trust me! ";
	std::string rstr = "";
	std::string teststr = "";



	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		int row = 0;
		int textW, textH;
		int lastPos = 0;
		SDL_Color textYellow = { 255, 255, 0 };
		SDL_Color textGreen = { 0, 255, 0 };

		if (exists("/home/pi/bmos/parade/parade.txt"))
		{
			playVideo("paradelong");
			system("sudo rm /home/pi/bmos/parade/parade.txt");
		}
		else
			playVideo("paradewhat");
		
		mText.SetText(gRenderer, gFont, "I'm listening...", textYellow);
		mText.Draw(gRenderer);
		SDL_RenderPresent(gRenderer);

		if (argc > 1)
		{
			response = "You said: ";
			response += argv[1];
		}
		else
		{
			response = "You said: ";
			ProcessVoice(message);
			response += message;
			response += " ";
		}


		bool quit = false;

		//Event handler
		SDL_Event e;

		while (!quit)
		{
			// Check for special keys like shift and control.
			const Uint8 *keys = SDL_GetKeyboardState(NULL);

			//Handle events on queuxe
			while (SDL_PollEvent(&e) != 0)
			{
				//User requests quit
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				else if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
				{

					switch (e.key.keysym.sym)
					{

					case SDLK_SPACE:
						quit = true;
						break;

					default:

						break;
					}
				}
			}

			SDL_RenderClear(gRenderer);


			int rpos = 0;
			lastPos = 0;
			row = 0;
			const char *c;

			while (rpos != std::string::npos)
			{
				rstr = "";
				teststr = "";
				lastPos = rpos;
				while (true)
				{

					//printf("%d\n", rpos);
					teststr += response.substr(lastPos, rpos - lastPos);
					c = teststr.c_str();
					TTF_SizeText(gFont, c, &textW, &textH);
					//printf("%d, %d\n", textW, textH);
					if (textW < 320)
					{
						rstr = teststr;
						//printf("%s\n", rstr.c_str());
						lastPos = rpos;
						rpos = response.find(' ', lastPos + 1);
						if (rpos == std::string::npos)
						{
							if (lastPos < response.length())
							{
								rstr += response.substr(lastPos, response.length() - lastPos);
							}
							break;
						}
					}
					else
					{
						rpos = lastPos;
						break;
					}
				}
				c = rstr.c_str();
				TTF_SizeText(gFont, c, &textW, &textH);
				//printf("%d, %d\n", textW, textH);
				mText.SetText(gRenderer, gFont, c, textGreen);
				mText.SetPosY(5+ row * (textH + 2));

				mText.Draw(gRenderer);
				row++;
			}

			mText.SetText(gRenderer, gFont, "You said:", textYellow);
			mText.SetPosY(5);

			mText.Draw(gRenderer);
			row++;
			SDL_RenderPresent(gRenderer);

			counter--;
			if (counter <= 0)
				quit = true;
		}

		SDL_RenderClear(gRenderer);
		SDL_RenderPresent(gRenderer);
		playVideo("response");
		SDL_DestroyRenderer(gRenderer);
		SDL_DestroyWindow(gWindow);

		SDL_Quit();


	}
	return 0;
}
