/* Defines the entry point for the console application */

#include "graph.h"
#include <getopt.h>
#include <fstream>
#include <string.h>

int main(int argc, char * argv[])
{
    CGraph graph = CGraph();
    ulong boys, girls, edges;
    ulong method;
    bool help, create, load, find, print, match;
    char *filename;

    help = create = load = find = print = match = false;

    int c;
    while ((c = getopt(argc, argv, "hc:l:f:gm")) != -1) 
	{
		switch (c)
		{
		case 0:
		{
			break;
		}
		case 'h':
		{
			if (help)
			{
				break;
			}
			else
			{
				help = true;
			}

			std::cout << "-h: Zobrazí tuto nápovìdu." << std::endl;
			std::cout << "-c B G E: Vygeneruje bipartitní graf s poètem uzlù B a G a poètem hran E." << std::endl;
			std::cout << "-l F: Naète bipartitní graf ze souboru F." << std::endl;
			std::cout << "-f M: Spoèítá maximální párování metodou M (h: Hopcroft-Karp, e: Edmonds-Karp)." << std::endl;
			std::cout << "-g: Vytiskne graf." << std::endl;
			std::cout << "-m: Vytiskne maximální párování." << std::endl;
			break;
		}
		case 'c':
		{
			if (create || load)
			{
				break;
			}

			create = true;
			optind--;

			if (optind >= argc)
			{
				return 1;
			}

			boys = strtoul(argv[optind++], NULL, 10);

			if (optind >= argc)
			{
				return 1;
			}

			girls = strtoul(argv[optind++], NULL, 10);

			if (optind >= argc)
			{
				return 1;
			}

			edges = strtoul(argv[optind++], NULL, 10);

			break;
		}
		case 'l':
		{
			if (load || create)
			{
				break;
			}

			load = true;
			filename = optarg;

			break;
		}
		case 'f':
		{
			if (find)
			{
				break;
			}

			find = true;
			optind--;

			if (optind >= argc)
			{
				return 1;
			}

			if (strcmp(optarg, "h") == 0)
			{
				method = HOPCROFT_KARP;
			}
			else if (strcmp(optarg, "e") == 0)
			{
				method = EDMONDS_KARP;
			}
			else
			{
				return 1;
			}

			break;
		}
		case 'g':
		{
			if (print)
			{
				break;
			}

			print = true;

			break;
		}
		case 'm':
		{
			if (match)
			{
				break;
			}

			match = true;

			break;
		}
		case '?':
		{
			return 1;
		}
		default:
		{
			return 1;
		}
		}
    }

    if (create) 
	{ 
		/* Generate bipartite graph */
        srand(static_cast<unsigned int>(time(NULL)));
        graph.generateBipartiteGraph(boys, girls, edges);
    }

    if (load) 
	{  
		/* Load bipartite graph from file */
        std::ifstream file(filename);

        if (file.is_open()) 
		{
            std::stringstream ss;
            ss << file.rdbuf();
            graph.parseGraph(ss.str());
        }

        file.close();
    }

    if (load || create) 
	{
		if (print) 
		{ 
			/* Print bipartite graph */
			std::cout << graph.printGraph();
		}

		if (print && find) std::cout << "\n";

		if (find) 
		{ 
			/* Find maximum matching */
			//std::string maxMatch = graph.printMaximumMatching(method);
			std::string maxMatch = graph.printMaximumMatching(method);

			if (match) 
			{ 
				/* Print maximum matching */
				std::cout << maxMatch;
			}
		}
    }

    /* std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get(); */

    return 0;
}

