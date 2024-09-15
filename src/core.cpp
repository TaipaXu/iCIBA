#include "./core.hpp"
#include <iostream>
#include <numeric>
#include <boost/program_options.hpp>
#include "./config.hpp"
#include "./network.hpp"
#include "utils/terminalprint.hpp"

void Core::start(int argc, char *argv[]) const
{
    namespace po = boost::program_options;

    po::options_description optionsDescription("Allowed options");
    optionsDescription.add_options()("help,h", "Show help message")                     // help option
        ("version,v", "Show version information")                                       // version option
        ("query,q", po::value<std::string>(), "Translate the word or sentence")         // query option
        ("input", po::value<std::vector<std::string>>(), "Input positional arguments"); // input positional arguments

    po::positional_options_description positionalOptionsDescription;
    positionalOptionsDescription.add("input", -1);

    po::variables_map variablesMap;
    try
    {
        po::store(po::command_line_parser(argc, argv)
                      .options(optionsDescription)
                      .positional(positionalOptionsDescription)
                      .run(),
                  variablesMap);
        po::notify(variablesMap);
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return;
    }

    if (variablesMap.count("help"))
    {
        std::cout << optionsDescription << std::endl;
    }
    else if (variablesMap.count("version"))
    {
        std::cout << DISPLAY_NAME << " " << PROJECT_VERSION << std::endl;
    }
    else if (variablesMap.count("query"))
    {
        const std::string command = variablesMap["query"].as<std::string>();

        std::vector<std::string> arguments;
        if (variablesMap.count("input"))
        {
            arguments = variablesMap["input"].as<std::vector<std::string>>();
        }

        const std::string input = command + " " + (arguments.empty() ? "" : std::accumulate(arguments.begin(), arguments.end(), std::string(), [](const std::string &a, const std::string &b)
                                                                                            { return a + " " + b; }));

        query(input);
    }
    else if (variablesMap.count("input")) [[likely]]
    {
        const std::vector<std::string> arguments = variablesMap["input"].as<std::vector<std::string>>();
        const std::string input = std::accumulate(arguments.begin(), arguments.end(), std::string(), [](const std::string &a, const std::string &b)
                                                  { return a + " " + b; });
        query(input);
    }
    else
    {
        std::cout << optionsDescription << std::endl;
    }
}

void Core::query(const std::string &input) const
{
    Network network;
    try
    {
        std::variant<Model::WordResult, Model::SentenceResult> result = network.translate(input);
        if (std::holds_alternative<Model::WordResult>(result))
        {
            const Model::WordResult wordResult = std::get<Model::WordResult>(result);
            const TerminalPrint terminalPrint(wordResult);
            std::cout << terminalPrint.getStr();
        }
        else
        {
            const Model::SentenceResult sentenceResult = std::get<Model::SentenceResult>(result);
            TerminalPrint terminalPrint(sentenceResult);
            std::cout << terminalPrint.getStr();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
