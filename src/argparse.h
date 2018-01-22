#ifndef STRPERCPP_ARGPARSE_H
#define STRPERCPP_ARGPARSE_H

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace strpercpp {
namespace argparse {

template<typename src, typename trg> 
trg lexical_cast(src v) {
  std::stringstream ss;
  trg ret;
  ss << v;
  ss >> ret;
  return ret;
}

template<> 
std::string lexical_cast(std::string v) {
  return v;
}

class Arg {
  public:
    std::string name;
    std::string value;

    Arg() {}

    Arg(std::string name_): name(name_), value("") {}

};

class ArgParser {
  public:
    ArgParser() {}

    void add_argument(const std::string& optname) {
      std::string key;
      if (optname[0] == '-' && optname[1] == '-') {
        key = optname.substr(2, optname.size());
      } else if (optname[0] == '-') {
        key = lexical_cast<char, std::string>(optname[1]);
      }
//      printf("add key:'%s'\n", key.c_str());
      Arg arg(key);
      args[key] = arg;

      std::unordered_map<std::string, Arg>::iterator it;
    }

    template<typename T> 
    T get(const std::string& key) {
      std::unordered_map<std::string, Arg>::const_iterator it = args.find(key);
      if (it == args.end()) {
        std::cout << "not found:" << key << std::endl;
        T ret = T();
        return ret;
      }
//      std::cout << "ret:" << it->second.value << std::endl;
      return lexical_cast<std::string, T>(it->second.value);
    }

    void parse_args(int argc, const char* argv[]) {

      for (int optind=0; optind < argc; ++optind) {
        if (argv[optind][0] == '-') { 
          if (argv[optind][1] == '-') { // long option

            std::string key(argv[optind] + 2, argv[optind] + strlen(argv[optind]));

            std::unordered_map<std::string, Arg>::const_iterator it = args.find(key);
            if (it != args.end()) {
              optind += 1;
              std::string value = lexical_cast<const char*, std::string>(argv[optind]);
              args[key].value = value;
            } else {
            }

          } else { // short option
            std::string key = lexical_cast<char, std::string>(argv[optind][1]);
            std::unordered_map<std::string, Arg>::const_iterator it = args.find(key);
            if (it != args.end()) {
              optind += 1;
              std::string value = lexical_cast<const char*, std::string>(argv[optind]);
              args[key].value = value;
            }

          }
        }
      }
    }

  private:
    std::unordered_map<std::string, Arg> args;
};


} // namespace argparse
} // namespace strpercpp

#endif // STRPERCPP_ARGPARSE_H
