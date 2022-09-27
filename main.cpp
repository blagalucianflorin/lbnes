#include "nes.h"

#include <yaml-cpp/yaml.h>
#include "misc/base64.h"

int main (int argc, char **argv)
{
//    class nes nes (argc > 1 ? argv[1] : "");
//    nes.start ();

//    uint16_t  upper = 8192;
//    memory old_memory (0, upper);
//
//    for (int i = 0; i < upper; i++)
//        old_memory.write (0x0000 + i, 0x00 + i);
//
//    std::string old_memory_state = old_memory.save_state ("old_memory");
//    std::cout << old_memory_state << std::endl << std::endl;
//
//    memory new_memory (0, 0);
//    new_memory.load_state (old_memory_state);
//
//    for (int i = 0; i < upper; i++)
//        if (old_memory.read (i) != new_memory.read (i))
//            std::cout << "a";

//    YAML::Node node;

//    YAML::Emitter emitter;
//    emitter << YAML::Binary((const unsigned char *) "Hello, World!", 13);
//    node["info"] = emitter.c_str();

//usage base64
//    std::vector<BYTE> myData;
//    ...
//    std::string encodedData = base64_encode((const unsigned char *) "Hello, World!", 13);
//    node["info-coded"] = encodedData;
//    std::cout << node["info-coded"] << std::endl;
//    std::vector<BYTE> decodedData = base64_decode(encodedData);
//    std::string test =  (char*) decodedData.data();
//    node["info-decoded"] = test;
//    std::cout << node["info-decoded"] << std::endl;



//    std::string test (decodedData.data());

//    YAML::Binary binary = node["info"].as<YAML::Binary>();
//    const unsigned char * data = binary.data();
//    std::size_t size = binary.size();
//
//    std::cout << (char *) data;

    return (0);
}
