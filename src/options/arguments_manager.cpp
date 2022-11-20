//
// Created by blaga on 21-Oct-22.
//

#include "options/arguments_manager.hpp"


// TODO checks needed
int arguments_manager::process (int argc, char **argv)
{
    std::string current_argument;
    std::string next_argument;

    // Special loop for log file
    for (int i = 1; i < argc; i++)
    {
        current_argument = argv[i];
        if (current_argument == "-l" or current_argument == "--log")
        {
            if (i >= argc - 1)
                LOGGER_ERROR_STD ("No log file specified.");

            next_argument = argv[i + 1];

            if (!configurator::get_instance ()["log_file"])
                configurator::get_instance ()["log_file"] = next_argument;
        }
    }

    logger::init (configurator::get_instance ()["log_file"].as<std::string> ());
    LOGGER_INFO ("lbnes v" LBNES_VERSION ".");
    LOGGER_INFO ("Built at " __TIMESTAMP__ ".");

#if defined (_WIN32)
    LOGGER_INFO ("Built for Windows.");
#elif defined (UNIX) and not defined (__APPLE__)
    LOGGER_INFO ("Built for Linux.");
#elif defined (__APPLE__)
    LOGGER_INFO ("Built for macOS.");
#endif

#ifdef DEBUG
    LOGGER_INFO ("Debug build.");
#else
    LOGGER_INFO ("Release build.");
#endif

    LOGGER_INFO ("Loaded config defaults. Parsing CLI arguments.");

    for (int i = 1; i < argc; i++)
    {
        current_argument = argv[i];
        if (current_argument[0] != '-')
            LOGGER_INFO ("Got argument '" + current_argument + "'.");

        if (current_argument == "-h" or current_argument == "--help")
        {
            LOGGER_INFO ("Exiting.");

            arguments_manager::print_help ();

            return (1);
        }

        if (current_argument == "-v" or current_argument == "--version")
        {
            LOGGER_INFO ("Exiting.");

            std::cout << "lbnes v" LBNES_VERSION << std::endl;

            return (1);
        }

        if (current_argument == "-r" or current_argument == "--rom")
        {
            if (i == argc - 1 or argv[i + 1][0] == '-')
                LOGGER_ERROR ("No ROM file provided.");

            configurator::get_instance ()["rom"] = std::string (argv[i + 1]);

            LOGGER_INFO ("Set ROM file to: '" + std::string (argv[i + 1]) + "'.");

            goto SKIP_ARGUMENT_CHECKS;
        }

        if (current_argument == "-f" or current_argument == "--fullscreen")
        {
            if (i >= argc - 1)
            {
                next_argument = argv[i + 1];

                if (next_argument[0] != '-')
                    configurator::get_instance ()["fullscreen"] = next_argument == "true";
                else
                    configurator::get_instance ()["fullscreen"] = true;
            }
            else
                configurator::get_instance ()["fullscreen"] = true;

            LOGGER_INFO (std::string ("Set fullscreen to '") +
                                (configurator::get_instance ()["fullscreen"].as <bool> () ? "true" : "false")+ "'.");

            goto SKIP_ARGUMENT_CHECKS;
        }

        if (current_argument == "-W" or current_argument == "--width")
        {
            next_argument = argv[i + 1];

            if (!configurator::get_instance ()["resolution"])
                configurator::get_instance ()["resolution"] = YAML::Node ();

            configurator::get_instance ()["resolution"]["width"] = std::stoi (next_argument);

            LOGGER_INFO ("Set window width to '" + next_argument + "'.");

            goto SKIP_ARGUMENT_CHECKS;
        }

        if (current_argument == "-H" or current_argument == "--height")
        {
            next_argument = argv[i + 1];

            if (!configurator::get_instance ()["resolution"])
                configurator::get_instance ()["resolution"] = YAML::Node ();

            configurator::get_instance ()["resolution"]["height"] = std::stoi (next_argument);

            LOGGER_INFO ("Set window height to '" + next_argument + "'.");

            goto SKIP_ARGUMENT_CHECKS;
        }

        if (current_argument == "--display_fps")
        {
            if (i >= argc - 1)
            {
                next_argument = argv[i + 1];

                if (next_argument[0] != '-')
                    configurator::get_instance ()["display_fps"] = next_argument == "true";
                else
                    configurator::get_instance ()["display_fps"] = true;
            }
            else
                configurator::get_instance ()["display_fps"] = true;

            LOGGER_INFO (std::string ("Set display-fps to '") +
                         (configurator::get_instance ()["fullscreen"].as <bool> () ? "true" : "false")+ "'.");

            goto SKIP_ARGUMENT_CHECKS;
        }

        if (current_argument == "-s" or current_argument == "--speed")
        {
            configurator::get_instance ()["speed"] = std::stoi (std::string (argv[i + 1]));

            LOGGER_INFO ("Set emulation speed to " + std::string (argv[i + 1]) + "'.");

            goto SKIP_ARGUMENT_CHECKS;
        }

        if (current_argument == "--new_controller_replaces_player_one")
        {
            if (i >= argc - 1)
            {
                next_argument = argv[i + 1];

                if (next_argument[0] != '-')
                    configurator::get_instance ()["new_controller_replaces_player_one"] = next_argument == "true";
                else
                    configurator::get_instance ()["new_controller_replaces_player_one"] = true;
            }
            else
                configurator::get_instance ()["new_controller_replaces_player_one"] = true;

            LOGGER_INFO (std::string ("Set new_controller_replaces_player_one to '") +
                         (configurator::get_instance ()["fullscreen"].as <bool> () ? "true" : "false")+ "'.");

            goto SKIP_ARGUMENT_CHECKS;
        }

        if (current_argument == "-m" or current_argument == "--show_menu")
        {
            if (i >= argc - 1)
            {
                next_argument = argv[i + 1];

                if (next_argument[0] != '-')
                    configurator::get_instance ()["show_menu"] = next_argument == "true";
                else
                    configurator::get_instance ()["show_menu"] = true;
            }
            else
                configurator::get_instance ()["show_menu"] = true;

            LOGGER_INFO (std::string ("Set show_menu to '") +
                         (configurator::get_instance ()["fullscreen"].as <bool> () ? "true" : "false")+ "'.");

            goto SKIP_ARGUMENT_CHECKS;
        }

        if (current_argument == "-c" or current_argument == "--config_file")
        {
            if (i >= argc - 1)
                LOGGER_ERROR ("No config file specified!");

            configurator::config_file = std::string (argv[i + 1]);

            LOGGER_INFO ("Set config_file to " + std::string (argv[i + 1]) + "'.");

            goto SKIP_ARGUMENT_CHECKS;
        }

        if (current_argument == "--vsync")
        {
            if (i >= argc - 1)
            {
                next_argument = argv[i + 1];

                if (next_argument[0] != '-')
                    configurator::get_instance ()["vsync"] = next_argument == "true";
                else
                    configurator::get_instance ()["vsync"] = true;
            }
            else
                configurator::get_instance ()["vsync"] = true;

            LOGGER_INFO (std::string ("Set vsync to '") +
                         (configurator::get_instance ()["fullscreen"].as <bool> () ? "true" : "false")+ "'.");

            goto SKIP_ARGUMENT_CHECKS;
        }

        if (current_argument == "--client")
        {
            if (i <= argc - 2)
            {
                next_argument = argv[i + 1];

                if (next_argument[0] != '-')
                    configurator::get_instance ()["client"] = next_argument == "true";
                else
                    configurator::get_instance ()["client"] = true;
            }
            else
                configurator::get_instance ()["client"] = true;

            // TODO Log

            goto SKIP_ARGUMENT_CHECKS;
        }

        if (current_argument == "--server")
        {
            if (i <= argc - 2)
            {
                next_argument = argv[i + 1];

                if (next_argument[0] != '-')
                    configurator::get_instance ()["server"] = next_argument == "true";
                else
                    configurator::get_instance ()["server"] = true;
            }
            else
                configurator::get_instance ()["server"] = true;

            // TODO Log

            goto SKIP_ARGUMENT_CHECKS;
        }

        if (current_argument == "-p" or current_argument == "--port")
        {
            configurator::get_instance ()["vsync"] = std::stoi (std::string (argv[i + 1]));

            // TODO Log

            goto SKIP_ARGUMENT_CHECKS;
        }

        if (current_argument == "--server_ip")
        {
            configurator::get_instance ()["server_ip"] = std::string (argv[i + 1]);

            // TODO Log

            goto SKIP_ARGUMENT_CHECKS;
        }

        // This simulates a switch for strings.
SKIP_ARGUMENT_CHECKS:
        continue;
    }

    LOGGER_INFO ("Initial configuration set:\n" + YAML::Dump (configurator::get_instance ().get_whole_config ()));

    return (0);
}


void arguments_manager::print_help ()
{
    std::string help =
        "lbnes " LBNES_VERSION "\n\n"

        "The options priority is as follows:\n"
        "\t1. CLI arguments.\n"
        "\t2. Config file values.\n"
        "\t3. Default values.\n\n"

        "Default controls mapping (only player one):\n"
        "\t- DPAD: \tWASD\n"
        "\t- A:\t\tN\n"
        "\t- B:\t\tM\n"
        "\t- Start:\tK\n"
        "\t- Select:\tJ\n\n"

        "Options:\n"
        "\t-h / --help: Display this message.\n\n"

        "\t-r / --rom <rom_file>: ROM file to be loaded at startup.\n"
        "\tDefault: none\n\n"

        "\t-f / --fullscreen [true/false]: Start in fullscreen.\n"
        "\tNot specifying true/false will default to true.\n"
        "\tDefault: false\n\n"

        "\t-W / --width <width>: Width of the window.\n"
        "\tDefault: 768\n\n"

        "\t-H / --height <height>: Height of the window.\n"
        "\tDefault: 720\n\n"

        "\t--display_fps [true/false]: Display fps in title bar.\n"
        "\tNot specifying true/false will default to true.\n"
        "\tDefault: false\n\n"

        "\t--new_controller_replaces_player_one [true/false]: Connecting a new controller while\n"
        "\tthe emulator is running will take over player one's controls.\n"
        "\tDefault: true\n\n"

        "\t-s / --speed <speed>: Speed of the game in %.\n"
        "\tDefault: 100\n\n"

        "\t-m / --show_menu [true/false]: Display the menu.\n"
        "\tNot specifying true/false will default to true.\n"
        "\tDefault: true\n\n"

        "\t--vsync [true/false]: VSync option. Only works for 60 FPS.\n"
        "\tNot specifying true/false will default to true.\n"
        "\tDefault: false\n\n"

        "\t--client [true/false]: Use lbnes as a client (Player 2).\n"
        "\tNot specifying true/false will default to true.\n"
        "\tNote: You'll most likely also want to specify --server_ip if you use lbnes as a client.\n"
        "\tDefault: false\n\n"

        "\t--server [true/false]: Use lbnes as a server (Player 1).\n"
        "\tNot specifying true/false will default to true.\n"
        "\tDefault: false\n\n"

        "\t-p / --port <port>: Port to be used when lbnes is used as client or a server.\n"
        "\tDefault: 5035\n\n"

        "\t--server_ip <ip>: IP of the server. Used by both client and server.\n"
        "\tDefault: localhost\n\n"

        "\t-c / --config_file <config_file_path>: The config file to load at startup.\n"
        "\tDefault: config.yaml\n\n"

        "\t -l / --log_file <log_file_path>: The file where the logs for the gaming session will be saved.\n"
        "\tNote: <log_file_path> will pe appended with the current date and time, as well as the .log extension.\n"
        "\tDefault: lbnes\n\n";

    std::cout << help;
}
