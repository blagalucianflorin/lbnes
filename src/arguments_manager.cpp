//
// Created by blaga on 21-Oct-22.
//

#include "arguments_manager.hpp"

// TODO checks needed
int arguments_manager::process (int argc, char **argv)
{
    std::string current_argument;
    std::string next_argument;

    for (int i = 1; i < argc; i++)
    {
        current_argument = argv[i];

        if (current_argument == "-h" or current_argument == "--help")
        {
            arguments_manager::print_help ();

            return (1);
        }

        if (current_argument == "-r" or current_argument == "--rom")
        {
            configurator::get_instance ()["rom"] = std::string (argv[i + 1]);

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

            goto SKIP_ARGUMENT_CHECKS;
        }

        if (current_argument == "-W" or current_argument == "--width")
        {
            next_argument = argv[i + 1];

            if (!configurator::get_instance ()["resolution"])
                configurator::get_instance ()["resolution"] = YAML::Node ();

            configurator::get_instance ()["resolution"]["width"] = std::stoi (next_argument);

            goto SKIP_ARGUMENT_CHECKS;
        }

        if (current_argument == "-H" or current_argument == "--height")
        {
            next_argument = argv[i + 1];

            if (!configurator::get_instance ()["resolution"])
                configurator::get_instance ()["resolution"] = YAML::Node ();

            configurator::get_instance ()["resolution"]["height"] = std::stoi (next_argument);

            goto SKIP_ARGUMENT_CHECKS;
        }

        if (current_argument == "--display-fps")
        {
            if (i >= argc - 1)
            {
                next_argument = argv[i + 1];

                if (next_argument[0] != '-')
                    configurator::get_instance ()["display-fps"] = next_argument == "true";
                else
                    configurator::get_instance ()["display-fps"] = true;
            }
            else
                configurator::get_instance ()["display-fps"] = true;

            goto SKIP_ARGUMENT_CHECKS;
        }

        if (current_argument == "-s" or current_argument == "--speed")
        {
            configurator::get_instance ()["speed"] = std::stoi (std::string (argv[i + 1]));

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

            goto SKIP_ARGUMENT_CHECKS;
        }

SKIP_ARGUMENT_CHECKS:
        continue;
    }

    return (0);
}

void arguments_manager::print_help ()
{
    std::string help =
        "lbnes help. Options:\n"
        "\t-h / --help: Display this menu.\n\n"

        "\t-r / --rom <rom_file>: ROM file to be loaded at startup.\n"
        "\tDefault: none\n\n"

        "\t-f / --fullscreen [true/false]: Start in fullscreen.\n"
        "\tNot specifying true/false will default to true.\n"
        "\t\tNote: This should only be used with a ROM file also specified at the moment.\n"
        "\tDefault: false\n\n"

        "\t-W / --width <width>: Width of the window.\n"
        "\tDefault: 768\n\n"

        "\t-H / --height <height>: Height of the window.\n"
        "\tDefault: 720\n\n"

        "\t--display-fps [true/false]: Display fps in title bar.\n"
        "\tNot specifying true/false will default to true.\n"
        "\tDefault: false\n\n"

        "\t--new_controller_replaces_player_one [true/false]: Connecting a new controller while\n"
        "\tthe emulator is running will take over player one's controls.\n"
        "\tDefault: true\n\n"

        "\t-s / --speed <speed>: Speed of the game in %.\n"
        "\tDefault: 100\n\n";

    std::cout << help;
}
