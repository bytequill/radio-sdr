#include "sources/wav-baseband.hpp"
#include "fft.hpp"

#include <fstream>

//GUI
#include <SDL3/SDL.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

int GUIMain() {
	if(!SDL_Init(SDL_INIT_VIDEO)) {
		return 1;
	}
	
	SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
	SDL_Window *window = SDL_CreateWindow("Title", 1280, 720, window_flags);
	if(window == NULL) {
		return 1;
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
	if(renderer == NULL) {
		return 1;
	}
	
	SDL_SetRenderVSync(renderer, 1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	
	ImGui::StyleColorsDark();
	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
	
	return 0;
}

int main() {
	int res2 = GUIMain();
	
	return res2;
	RFSourceWAV src;
	
	bool res = src.Load("./dev/test2.wav");
	std::vector<cd> samples;
	for (int i = 0; i != 10000 * 2; i++) {
		std::vector<cd> sampleFrame = src.GetSamples(i * pow(2,11), pow(2,11));
		fft(sampleFrame, false);
		samples.insert(samples.end(), sampleFrame.begin(), sampleFrame.end());
	}
	std::ofstream out;
	out.open("dev/out1.csv");
	out << "Magnitude\n";
    
    for (size_t i = 0; i < samples.size(); i++) {
        int mag = static_cast<int>(std::abs(samples[i]));
        out << mag << "\n";
    }
    
    out.close();
	
	return res;
}

