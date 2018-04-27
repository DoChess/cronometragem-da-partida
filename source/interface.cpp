#include "../include/interface.hpp"

using namespace std;

Interface::Interface()
{
    informationGame = "Chess Game";
    statusGame = "../assets/imgs/play.png";
	lightCurrentPlayer = true;
	gWindow = NULL;
    gRenderer = NULL;
    gFont = NULL;
    gFontTimer = NULL;
    gTexture = NULL;
}

void Interface::setStatusGame(string status)
{
    statusGame = status;
}

string Interface::getStatusGame()
{
    return statusGame;
}

void Interface::setLightCurrentPlayer(bool currentPlayer)
{
    lightCurrentPlayer = currentPlayer;
}

bool Interface::isLightCurrentPlayer()
{
    return lightCurrentPlayer;
}

void Interface::setInformation(string info)
{
    informationGame = info;
}

string Interface::getInformation()
{
    return informationGame;
}

pair<Player, Player> Interface::controlTime(SDL_Event e, 
    pair<Player, Player> players, Interface* interface){
	string statusGameText = "";

	if( e.key.keysym.sym == SDLK_s )
	{
		if( players.second.timer.isStarted() and players.first.timer.isStarted())
		{
			players.second.timer.stop();
			players.first.timer.stop();
			statusGameText = "../assets/imgs/play.png";
		}
		else
		{
			players.first.timer.start();
			players.second.timer.start();
			players.second.timer.pause();
	
			statusGameText = "../assets/imgs/play.png";
		}
	}
	else if( e.key.keysym.sym == SDLK_p )
	{
		if( players.first.timer.isPaused() and players.second.timer.isPaused() )
		{
			(interface->isLightCurrentPlayer()) ? players.first.timer.unpause():
				players.second.timer.unpause();
			
			statusGameText = "../assets/imgs/pause.png";
		}
		else
		{
			players.first.timer.pause();
			players.second.timer.pause();
			statusGameText = "../assets/imgs/play.png";
		}
	}
	else if( e.key.keysym.sym == SDLK_c )
	{

		if (players.second.timer.isPaused())
		{
			players.second.timer.unpause();
			players.first.timer.pause();
			players.first.setFault();

			interface->setLightCurrentPlayer(false);
		} else {
			players.second.timer.pause();
			players.first.timer.unpause();
			players.second.setFault();

			interface->setLightCurrentPlayer(true);
		}

		statusGameText = "../assets/imgs/pause.png";
	}

	interface->setStatusGame(statusGameText);

	return players;
}


void Interface::drawBackgroundInterface(string statusOfInformation){
	SDL_Rect blackRet = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 };
	SDL_SetRenderDrawColor( this->gRenderer, 0x00, 0x00, 0x00, 0x00 );		
	SDL_RenderFillRect( this->gRenderer, &blackRet );
	SDL_Rect whiteRet = { SCREEN_WIDTH / 500, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	SDL_SetRenderDrawColor( this->gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );		
	SDL_RenderFillRect( this->gRenderer, &whiteRet );
	
	int color[4];

	if(statusOfInformation == "analysing"){
		color[3] = 0;
		color[2] = 96;
		color[1] = 255;
		color[0] = 255;
	} else if(statusOfInformation == "valid"){
		color[3] = 13;
		color[2] = 239;
		color[1] = 66;
		color[0] = 255;
	} else if(statusOfInformation == "invalid"){
		color[3] = 220;
		color[2] = 61;
		color[1] = 42;
		color[0] = 255;
	} else {
		color[3] = 211;
		color[2] = 211;
		color[1] = 211;
		color[0] = 211;
	}
	
	SDL_Rect fillRect2 = { 1, SCREEN_HEIGHT / SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT/2};
	SDL_SetRenderDrawColor( gRenderer, color[3], color[2], color[1], color[0] );		
	SDL_RenderFillRect( gRenderer, &fillRect2 );
}


bool Interface::initLibsSDL()
{
	bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		this->gWindow = SDL_CreateWindow( "Chess Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( this->gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			this->gRenderer = SDL_CreateRenderer( this->gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( this->gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor( this->gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

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

bool Interface::loadMedias()
{
	bool success = true;

	this->gFontTimer = TTF_OpenFont( "../assets/font/lazy.ttf", 120 );
	this->gFont = TTF_OpenFont( "../assets/font/lemon.ttf", 80 );
	
	if( this->gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	if( this->gFontTimer == NULL )
	{
		printf( "Failed to load lazy font Timer! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}


	return success;
}

void Interface::close()
{
    playerTimeDarkTexture.free();
	playerTimeLightTexture.free();
	gStatusGameTexture.free();
	gSpriteSheetTexture.free();

	SDL_DestroyTexture( this->gTexture );
    this->gTexture = NULL;

	TTF_CloseFont( this->gFont );
	this->gFont = NULL;

	TTF_CloseFont( this->gFontTimer );
	this->gFontTimer = NULL;

	SDL_DestroyRenderer( this->gRenderer );
	this->gRenderer = NULL;

	SDL_DestroyWindow( this->gWindow );
	this->gWindow = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}


void Interface::updateElements(pair<Player, Player> players){
	SDL_Color textColorBlack = { 0, 0, 0, 255 };
	SDL_Color textColorWhite = { 255, 255, 255, 255 };
	//SDL_Color textColorRed = { 255, 0, 0, 255 };
	
	if( !playerTimeLightTexture.loadFromRenderedText( players.first.timer.showCurrentTime(), textColorBlack, this->gRenderer, this->gFontTimer ) )
	{
		printf( "Unable to render time texture!\n" );
	}
	if( !playerTimeDarkTexture.loadFromRenderedText( players.second.timer.showCurrentTime(), textColorWhite, this->gRenderer, this->gFontTimer ) )
	{
		printf( "Unable to render time texture!\n" );
	}
	if( !playerFailuresDarkTexture.loadFromRenderedText( players.first.getFaults(), textColorWhite, this->gRenderer, this->gFontTimer ) )
	{
		printf( "Unable to render time texture!\n" );
	}
	if( !playerFailuresLightTexture.loadFromRenderedText( players.second.getFaults(), textColorBlack, this->gRenderer, this->gFontTimer ) )
	{
		printf( "Unable to render time texture!\n" );
	}
	if( !gInfoTexture.loadFromRenderedText( this->getInformation(), textColorBlack, this->gRenderer, this->gFont ) )
	{
		printf( "Unable to render information texture!\n" );
	}
	if( !gStatusGameTexture.loadFromFile( this->getStatusGame(), this->gRenderer) )
	{
		printf( "Unable to render start/stop prompt texture!\n" );
	}
}

void Interface::renderElements(){
	short int timeHeight = (((SCREEN_HEIGHT - playerTimeDarkTexture.getHeight())) / 2) + ((SCREEN_HEIGHT - playerTimeDarkTexture.getHeight()) / 2.5);
	short int lightWidth = (SCREEN_WIDTH - playerTimeLightTexture.getWidth()) / 8;
	short int darkWidth = ((SCREEN_WIDTH - playerTimeDarkTexture.getWidth()) / 2) + ((SCREEN_WIDTH - playerTimeDarkTexture.getWidth()) / 2.5);

	short int failureHeight = (((SCREEN_HEIGHT - playerTimeDarkTexture.getHeight())) / 2) + ((SCREEN_HEIGHT - playerTimeDarkTexture.getHeight()) / 7.5);
	short int lightFailureWidth = (SCREEN_WIDTH - playerFailuresDarkTexture.getWidth()*4) / 20;
	short int darkFailureWidth = (SCREEN_WIDTH - playerTimeDarkTexture.getWidth()) - ((SCREEN_WIDTH - playerTimeDarkTexture.getWidth()) / 250);

	gInfoTexture.render( ( SCREEN_WIDTH - gInfoTexture.getWidth() ) / 2, ((SCREEN_HEIGHT - gInfoTexture.getHeight()) / 4), this->gRenderer );
	
	playerTimeLightTexture.render( lightWidth , timeHeight, this->gRenderer );
	playerTimeDarkTexture.render( darkWidth, timeHeight , this->gRenderer );
	
	playerFailuresLightTexture.render( lightFailureWidth, failureHeight, this->gRenderer );
	playerFailuresDarkTexture.render( darkFailureWidth, failureHeight, this->gRenderer );

	gStatusGameTexture.render( (SCREEN_WIDTH - gStatusGameTexture.getWidth()) / 2, (SCREEN_HEIGHT - gStatusGameTexture.getHeight()) / 16, this->gRenderer );
}
bool Interface::initInterface(){
	bool sucess = true;
	if( !this->initLibsSDL() )
	{
		printf( "Failed to initialize graphic interface!\n" );
		sucess = false;
	}
	else
	{
		if( !this->loadMedias() )
		{
			printf( "Failed to load medias!\n" );
			sucess = false;
		}
	}
	return sucess;
}