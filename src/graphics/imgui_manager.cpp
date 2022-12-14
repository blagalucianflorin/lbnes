//
// Created by blaga on 26-Oct-22.
//

#include <utility>

#include "graphics/imgui_manager.hpp"


imgui_manager::imgui_manager (SDL_Renderer *renderer, SDL_Window *window, std::shared_ptr <class nes> new_nes) :
        renderer (renderer), window (window), my_nes (std::move(new_nes))
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

    if (this -> my_nes -> options.show_menu)
    {
        ImGui::SetNextWindowPos (ImVec2 (0, 0));

        float menu_height = 465;
        if (this -> my_nes -> options.is_client)
            menu_height = 250;
        else if (this -> my_nes -> options.is_server)
            menu_height = 380;

        ImGui::SetNextWindowSize (ImVec2 (290, menu_height));
        ImGui::Begin ("Menu");

        ImGui::Text ("Press P to toggle the menu");
        ImGui::NewLine ();

        if (this -> my_nes -> options.is_client)
        {
            ImGui::Text ("%s", (std::string ("Client connected to ") +
                                configurator::get_instance ()["server_ip"].as <std::string> () +
                                std::string (":") +
                                configurator::get_instance ()["port"].as <std::string> ()).c_str ());
            ImGui::NewLine ();
        }

        if (this -> my_nes -> options.is_server)
        {
            ImGui::Text ("%s", (std::string ("Server on ")
                          + configurator::get_instance ()["server_ip"].as <std::string> ()
                          + std::string (":") + configurator::get_instance ()["port"].as <std::string> ()).c_str ());
            ImGui::NewLine ();
        }

        if (!this -> my_nes -> options.is_client)
        {
            ImGui::Text ("Emulation speed");
            if (this -> my_nes -> options.vsync)
            {
                ImGui::BeginDisabled();
            }
            ImGui::SliderInt (" ", &this -> my_nes -> options.speed, 1, 300);
            if (this -> my_nes -> options.vsync)
                ImGui::EndDisabled ();
            ImGui::NewLine ();
        }


        if (!this -> my_nes -> options.is_client && this -> my_nes -> rom_loaded)
        {
            if (!this -> my_nes -> options.paused)
            {
                if (ImGui::Button("Pause"))
                {
                    LOGGER_INFO ("Paused game.");
                    this -> my_nes -> options.paused = true;
                    this -> my_nes -> emulate_frame = std::bind (&nes::emulate_frame_paused, my_nes); // NOLINT
                }
            }
            else
            {
                if (ImGui::Button("Resume"))
                {
                    LOGGER_INFO ("Resumed game.");
                    this -> my_nes -> options.paused = false;
                    this -> my_nes -> emulate_frame = std::bind (&nes::emulate_frame_real, my_nes); // NOLINT
                }
            }
            ImGui::NewLine();
        }


        if (!this -> my_nes -> options.is_client)
        {
            if (ImGui::Button ("Load ROM"))
            {
                cross_platform_mkdir ("games");

                LOGGER_INFO ("Loading ROM file from GUI.");
                auto selection = pfd::open_file ("Select a file",

#ifdef WIN32
                                                 ".\\games\\",
#else
                                                 "./games/",
#endif
                                                 {"NES ROM (.nes)", "*.nes *.ines"}).result ();

                if (!selection.empty ())
                {
                    LOGGER_INFO ("Trying to load '" + selection[0] + "' ROM file.");
                    this -> my_nes -> reload (selection[0], this -> my_nes -> joypads.empty ());
                }
            }
            ImGui::NewLine ();

            if (ImGui::Button ("Save State"))
            {
                cross_platform_mkdir ("saves");

                auto destination = pfd::save_file ("Select a file",
#ifdef WIN32
                                                   ".\\saves\\",
#else
                                                   "./saves/",
#endif
                                                   {"LBNES Save File (.lbnes_save)", "*.lbnes_save"},
                                                   pfd::opt::force_overwrite).result ();
                LOGGER_INFO (std::string ("Trying to save state to '") + destination + "'.");

                if (!destination.empty ())
                {
                    size_t last_dot = destination.find_last_of ('.');

                    if (last_dot == std::string::npos)
                        destination += ".lbnes_save";
                    else
                        destination = destination.substr(0, last_dot) + ".lbnes_save";

                    std::ofstream state_file (destination);

                    state_file << this -> my_nes -> save_state ();

                    state_file.close ();
                }
            }
            ImGui::NewLine ();

            if (ImGui::Button ("Load State"))
            {
                cross_platform_mkdir ("saves");

                LOGGER_INFO ("Loading save state file from GUI.");
                auto selection = pfd::open_file ("Select a file",
#ifdef WIN32
                                                 ".\\saves\\",
#else
                                                 "./saves/",
#endif
                                                 {"LBNES Save File (.lbnes_save)", "*.lbnes_save"}).result ();

                if (!selection.empty ())
                {
                    LOGGER_INFO ("Trying to load state from '" + selection[0] + "'.");

                    std::ifstream fin (selection[0]);

                    if (fin.good ())
                    {
                        fin.close ();
                        this -> my_nes -> load_state (YAML::Dump (YAML::LoadFile (selection[0])));
                        this -> my_nes -> options.paused = false;
                    }
                    else
                    {
                        LOGGER_ERROR ("State file doesn't exist.");
                    }
                }
            }
            ImGui::NewLine ();
        }

        bool old_fullscreen_checkbox = this -> my_nes -> options.fullscreen;
        ImGui::Checkbox ("Fullscreen", &(this -> my_nes -> options.fullscreen));
        if (old_fullscreen_checkbox != this -> my_nes -> options.fullscreen)
        {
            LOGGER_INFO ("Toggled fullscreen.");
            ::toggle_fullscreen(this->window);
        }
        ImGui::NewLine ();

        bool old_fps_checkbox = this -> my_nes -> options.display_fps;
        ImGui::Checkbox ("Display FPS", &this -> my_nes -> options.display_fps);
        if (old_fps_checkbox != this -> my_nes -> options.display_fps)
        {
            LOGGER_INFO ("Toggled Display FPS.");

            if (this -> my_nes -> options.display_fps)
                this -> my_nes -> set_title = std::bind (&nes::set_title_fps, this -> my_nes); // NOLINT
            else
                this -> my_nes -> set_title = std::bind (&nes::set_title_simple, this -> my_nes); // NOLINT
        }
        ImGui::NewLine ();

        bool old_vsync_checkbox = this -> my_nes -> options.vsync;
        ImGui::Checkbox ("VSync", &(this -> my_nes -> options.vsync));
        if (old_vsync_checkbox != this -> my_nes -> options.vsync)
        {
            LOGGER_INFO ("Toggled VSync.");

            configurator::get_instance ()["vsync"] = this -> my_nes -> options.vsync;
            update_vsync (this -> renderer);

            if (!this -> my_nes -> options.vsync)
                this -> my_nes -> options.speed = old_speed;
            else
            {
                old_speed = this -> my_nes -> options.speed;
                this -> my_nes -> options.speed = 100;
            }
        }
        ImGui::NewLine ();

        if (!this -> my_nes -> options.is_client)
        {
            if (ImGui::Button ("Reset"))
            {
                LOGGER_INFO ("Clicked reset button.");
                this -> my_nes -> reset ();
            }
            ImGui::NewLine ();
        }

        if (ImGui::Button ("Quit"))
        {
            LOGGER_INFO ("Clicked quit button.");
            this -> my_nes -> options.quit = true;
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