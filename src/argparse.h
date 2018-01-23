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
    int position;

    Arg() {}

    Arg(std::string name_): name(name_), value(""), position(0) {}

};

class ArgParser {
  public:
    ArgParser(): n_args(0) {}

    void add_argument(const std::string& optname) {
      n_args += 1;

      std::string key;
      if (optname[0] == '-' && optname[1] == '-') {
        key = optname.substr(2, optname.size());
        Arg arg(key);
        opt_args[key] = arg;
      } else if (optname[0] == '-') {
        key = lexical_cast<char, std::string>(optname[1]);
        Arg arg(key);
        opt_args[key] = arg;
      } else { // positional argument
        key = optname;
        Arg arg(key);
//        printf("add positional argument:%d %s\n", n_args, key.c_str());
        pos_args[n_args] = arg;
        position_by_key[key] = n_args;
      }
    }

    void add_argument(const std::string& optname, const std::string& default_v) {
      std::string key;
      if (optname[0] == '-' && optname[1] == '-') { // long option
        key = optname.substr(2, optname.size());
      } else if (optname[0] == '-') { // short option
        key = lexical_cast<char, std::string>(optname[1]);
      }
      Arg arg(key);
      arg.value = default_v;
      opt_args[key] = arg;
    }

    template<typename T> 
    T get(const std::string& key) {
      std::unordered_map<std::string, Arg>::const_iterator it = opt_args.find(key);
      if (it == opt_args.end()) { // not found in optional arguments

        std::unordered_map<std::string, int>::const_iterator it2 = position_by_key.find(key);
        if (it2 == position_by_key.end()) { // not found in positional arguments
          std::cerr << "not found:" << key << std::endl;
          T ret = T();
          return ret;
        } else { // found in positional arguments
          int position = it2->second;
          return lexical_cast<std::string, T>(pos_args[position].value);
        }

      } else { // found in optional arguments
        return lexical_cast<std::string, T>(it->second.value);
      }
    }

    void parse_args(int argc, const char* argv[]) {

      int position = 0;
      for (int optind=0; optind < argc; ++optind) {
        if (argv[optind][0] == '-') { 
          if (argv[optind][1] == '-') { // long option

            std::string key(argv[optind] + 2, argv[optind] + strlen(argv[optind]));

            std::unordered_map<std::string, Arg>::const_iterator it = opt_args.find(key);
            if (it != opt_args.end()) {
              optind += 1;
              std::string value = lexical_cast<const char*, std::string>(argv[optind]);
              opt_args[key].value = value;
              position += 1;
            }

          } else { // short option
            std::string key = lexical_cast<char, std::string>(argv[optind][1]);
            std::unordered_map<std::string, Arg>::const_iterator it = opt_args.find(key);
            if (it != opt_args.end()) {
              optind += 1;
              std::string value = lexical_cast<const char*, std::string>(argv[optind]);
              opt_args[key].value = value;
              position += 1;
            }
          }
        } else { // positional argument
//          printf("positional argument:%s %d\n", argv[optind], position);
          std::unordered_map<int, Arg>::const_iterator it = pos_args.find(position);
          if (it != pos_args.end()) {
            std::string value = lexical_cast<const char*, std::string>(argv[optind]);
            pos_args[position].value = value;
          }
          position += 1;

        }
      }
    }

  private:
    int n_args;
    std::unordered_map<std::string, Arg> opt_args;
    std::unordered_map<int, Arg> pos_args;
    std::unordered_map<std::string, int> position_by_key;
};


} // namespace argparse
} // namespace strpercpp

#endif // STRPERCPP_ARGPARSE_H
