#include <iostream>
#include "Config.h"
#include "FWApplication.h"
#include <SDL_events.h>
#include "SDL_timer.h"
#include <SDL_render.h>
#include <time.h>
#include "Domain/Graph.h"
#include <stdlib.h>     /* srand, rand */

#include "ExampleGameObject.h"


int main(int args[])
{

	bool debug = false; //THIS TOGGLES THE DEBUG OPTION

	srand(time(NULL));
	//auto window = Window::CreateSDLWindow();
	auto application = new FWApplication();
	if (!application->GetWindow())
	{
		LOG("Couldn't create window...");
		return EXIT_FAILURE;
	}
	
	application->SetTargetFPS(60);
	application->SetColor(Color(255, 10, 40, 255));
	
	Graph* graph = new Graph();
	SDL_Texture* map = application->LoadTexture("map.png");
	//while (true){}
	while (application->IsRunning())
	{
		application->StartTick();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				application->Quit();
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym){
				default:
					break;
				}
			}
		}

		
		application->DrawTexture(map, 0, 0);
		
		application->SetColor(Color(0, 0, 0, 255));
		
		//Draw the edges
		for (int i = 0; i < (int)graph->getEdges().size(); i++) {
			application->DrawLine(graph->getEdges().at(i)->getVertex1()->getXCoord(), graph->getEdges().at(i)->getVertex1()->getYCoord(),
			graph->getEdges().at(i)->getVertex2()->getXCoord(), graph->getEdges().at(i)->getVertex2()->getYCoord());
			if (debug) { //If debug, set the edge number in the line
				application->SetColor(Color(0, 0, 0, 255));
				application->SetFontSize(8);

				int middleX = (graph->getEdges().at(i)->getVertex1()->getXCoord() + graph->getEdges().at(i)->getVertex2()->getXCoord()) / 2;
				int middleY = (graph->getEdges().at(i)->getVertex1()->getYCoord() + graph->getEdges().at(i)->getVertex2()->getYCoord()) / 2;

				application->DrawText(std::to_string(i), middleX, middleY);
				application->SetFontSize(12);
			}
		}

		//Draw the vertexes
		
		for (int j = 0; j < (int)graph->getVertexes().size(); j++) {
			if (graph->target != nullptr && graph->getVertexes().at(j)->number == graph->target->number) {
				application->SetColor(Color(255, 255, 255, 255));
			}
			else {
				application->SetColor(Color(0, 0, 255, 255));
			}
			
			application->DrawCircle(graph->getVertexes().at(j)->getXCoord(), graph->getVertexes().at(j)->getYCoord(), 10, true);
			if (debug) { //if debug, write the number of the vertex in it
				application->SetColor(Color(255, 255, 255, 255));
				application->DrawText(std::to_string(j), graph->getVertexes().at(j)->getXCoord(), graph->getVertexes().at(j)->getYCoord());
			}
		}

		if (debug) { //if debug, show the legend
			application->SetColor(Color(0, 0, 255, 255));
			application->DrawText("Normal", 700, 125);
		}
		application->SetColor(Color(255, 255, 255, 255));
		application->DrawText("ReturnHomeChance " + std::to_string(graph->fsm->ReturnHomeChance * 100) + "%", 100, 50);
		application->DrawText("GetPowerChance " + std::to_string(graph->fsm->GetPowerUpChance * 100) + "%", 100, 70);
		application->DrawText("IdleChance " + std::to_string(graph->fsm->GetIdleChance * 100) + "%", 100, 90);
		// For the background
		application->SetColor(Color(255, 255, 255, 255));
		graph->Update();
		application->UpdateGameObjects();
		application->RenderGameObjects();
		application->DrawText(std::to_string(graph->bees.size()) + " bees in the field", 542, 590);
		application->EndTick();
	}
		
	return EXIT_SUCCESS;
}