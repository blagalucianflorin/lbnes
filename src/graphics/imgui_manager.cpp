//
// Created by blaga on 26-Oct-22.
//

#include "graphics/imgui_manager.hpp"


imgui_manager::imgui_manager (SDL_Renderer *renderer, SDL_Window *window, class nes *nes) :
        renderer (renderer), window (window), nes (nes)
{
    IMGUI_CHECKVERSION ();
    ImGui::CreateContext ();
    ImGuiIO& io = ImGui::GetIO ();
    ImGui::StyleColorsDark ();

    io.IniFilename = nullptr;

#ifdef IMGUI_RENDERER_SDL
    ImGui_ImplSDL2_InitForSDLRenderer (this -> window, this -> renderer);
    ImGui_ImplSDLRenderer_Init (this -> renderer);
#endif
}


imgui_manager::~imgui_manager ()
{
    LOGGER_INFO ("Deleting ImGui manager");
#ifdef IMGUI_RENDERER_SDL
    ImGui_ImplSDLRenderer_Shutdown ();
    ImGui_ImplSDL2_Shutdown ();
#endif
    ImGui::DestroyContext ();
}


void imgui_manager::draw_menu ()
{
    static int old_speed;

#ifdef IMGUI_RENDERER_SDL
    ImGui_ImplSDLRenderer_NewFrame ();
    ImGui_ImplSDL2_NewFrame ();
#endif

    ImGui::NewFrame ();

    if (this -> nes -> options.show_menu)
    {
        ImGui::SetNextWindowPos (ImVec2 (0, 0));

        float menu_height = 350;
        if (this -> nes -> options.is_client)
            menu_height = 250;
        else if (this -> nes -> options.is_server)
            menu_height = 380;

        ImGui::SetNextWindowSize (ImVec2 (290, menu_height));
        ImGui::Begin ("Menu");

        ImGui::Text ("Press P to toggle the menu");
        ImGui::NewLine ();

        if (this -> nes -> options.is_client)
        {
            ImGui::Text ((std::string ("Client connected to ") +
                                configurator::get_instance ()["server_ip"].as <std::string> () +
                                std::string (":") +
                                configurator::get_instance ()["port"].as <std::string> ()).c_str ());
            ImGui::NewLine ();
        }

        if (this -> nes -> options.is_server)
        {
            ImGui::Text ((std::string ("Server on ") + configurator::get_instance ()["server_ip"].as <std::string> ()
                          + std::string (":") + configurator::get_instance ()["port"].as <std::string> ()).c_str ());
            ImGui::NewLine ();
        }

        if (!this -> nes -> options.is_client)
        {
            ImGui::Text ("Emulation speed");
            if (this -> nes -> options.vsync)
            {
                ImGui::BeginDisabled();
            }
            ImGui::SliderInt (" ", &this -> nes -> options.speed, 1, 300);
            if (this -> nes -> options.vsync)
                ImGui::EndDisabled ();
            ImGui::NewLine ();
        }


        if (!this -> nes -> options.is_client)
        {
            if (ImGui::Button ("Load ROM"))
            {
                LOGGER_INFO ("Loading ROM file from GUI.");
                auto selection = pfd::open_file ("Select a file").result ();

                if (!selection.empty ())
                {
                    LOGGER_INFO ("Trying to load '" + selection[0] + "'.");
                    this -> nes -> reload (selection[0], this -> nes -> joypads.empty ());
                }
            }
            ImGui::NewLine ();
        }

        bool old_fullscreen_checkbox = this -> nes -> options.fullscreen;
        ImGui::Checkbox ("Fullscreen", &(this -> nes -> options.fullscreen));
        if (old_fullscreen_checkbox != this -> nes -> options.fullscreen)
        {
            LOGGER_INFO ("Toggled fullscreen.");
            ::toggle_fullscreen(this->window);
        }
        ImGui::NewLine ();

        ImGui::Checkbox ("Display FPS", &this -> nes -> options.display_fps);
        ImGui::NewLine ();

        bool old_vsync_checkbox = this -> nes -> options.vsync;
        ImGui::Checkbox ("VSync", &(this -> nes -> options.vsync));
        if (old_vsync_checkbox != this -> nes -> options.vsync)
        {
            LOGGER_INFO ("Toggled VSync.");

            configurator::get_instance ()["vsync"] = this -> nes -> options.vsync;
            update_vsync (this -> renderer);

            if (!this -> nes -> options.vsync)
                this -> nes -> options.speed = old_speed;
            else
            {
                old_speed = this -> nes -> options.speed;
                this -> nes -> options.speed = 100;
            }
        }
        ImGui::NewLine ();

        if (!this -> nes -> options.is_client)
        {
            if (ImGui::Button ("Reset"))
            {
                LOGGER_INFO ("Clicked reset button.");
                this -> nes -> reset ();
            }
            ImGui::NewLine ();
        }

        if (ImGui::Button ("Quit"))
        {
            LOGGER_INFO ("Clicked quit button.");
            this -> nes -> options.quit = true;
        }

        ImGui::End ();
    }

    ImGui::Render ();

#ifdef IMGUI_RENDERER_SDL
    ImGui_ImplSDLRenderer_RenderDrawData (ImGui::GetDrawData ());
#endif
}


void imgui_manager::process_event (SDL_Event *event)
{
    ImGui_ImplSDL2_ProcessEvent (event);
}