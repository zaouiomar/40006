#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <limits>
#include <iterator>
#include <algorithm>
#include <functional>
#include "commands.hpp"

int main(int argc, char * argv[])
{
  using namespace zaoui;
  std::unordered_map< std::string, std::unordered_map< std::string, std::vector< std::pair< size_t, size_t > > > > texts;
  if (argc == 2 && std::string(argv[1]) == "--help")
  {
    printHelp(std::cout);
    return 0;
  }
  else if (argc == 3 && std::string(argv[1]) == "--continue")
  {
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  }

  std::map< std::string, std::function< void() > > cmds;
  cmds["generatelinks"] = std::bind(generateLinks, std::ref(std::cin), std::ref(texts));
  cmds["removelinks"] = std::bind(removeLinks, std::ref(std::cin), std::ref(texts));
  cmds["printlinks"] = std::bind(printLinks, std::ref(std::cin), std::ref(std::cout), std::cref(texts));
  cmds["printtext"] = std::bind(printText, std::ref(std::cin), std::ref(std::cout), std::cref(texts));
  cmds["printtextinfile"] = std::bind(printTextInFile, std::ref(std::cin), std::cref(texts));
  cmds["merge"] = std::bind(mergeTexts, std::ref(std::cin), std::ref(texts));
  cmds["insert"] = std::bind(insertText, std::ref(std::cin), std::ref(texts));
  cmds["removelines"] = std::bind(removeLines, std::ref(std::cin), std::ref(texts));
  cmds["move"] = std::bind(moveText, std::ref(std::cin), std::ref(texts));
  cmds["split"] = std::bind(splitTexts, std::ref(std::cin), std::ref(texts));
  cmds["invertlines"] = std::bind(invertLines, std::ref(std::cin), std::ref(texts));
  cmds["invertwords"] = std::bind(invertWords, std::ref(std::cin), std::ref(texts));
  cmds["replaceword"] = std::bind(replaceWord, std::ref(std::cin), std::ref(texts));

  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
    }
    catch (...)
    {
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
