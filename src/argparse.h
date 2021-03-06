#ifndef STRPERCPP_ARGPARSE_H
#define STRPERCPP_ARGPARSE_H

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace strpercpp {
namespace argparse {

enum action_type {
  store_const = 0,
  store_true = 1,
  store_false = 2,
};

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
    std::string help;
    std::string opt;

    Arg() {}

    Arg(std::string name_): name(name_), value(""), help(""), opt(""), atype_(store_const) {}

    Arg(std::string name_, std::string help_, std::string opt_)
      : name(name_), value(""), help(help_), opt(opt_), atype_(store_const) {}

    Arg(std::string name_, std::string value_, std::string help_, std::string opt_) 
      :name(name_), value(value_), help(help_), opt(opt_), atype_(store_const) {}

    Arg(std::string name_, std::string value_, std::string help_, std::string opt_,
        action_type atype)
      :name(name_), value(value_), help(help_), opt(opt_) , atype_(atype) {}

    action_type atype() { return atype_; }

  private:
    action_type atype_;
};

class ArgParser {
  public:
    ArgParser(): n_pos_arg(0) {}
    
    void print_help() {
      std::map<std::string, Arg>::iterator it;
      std::map<int, Arg>::iterator it2;

      std::cout << "Usage: " << cmd << " [options]";
      for (it2 = pos_args.begin(); it2 != pos_args.end(); ++it2) {
        std::cout << " " << it2->second.name; 
      }
      std::cout << std::endl;

      std::cout << "options:" << std::endl;
      for (it = opt_args.begin(); it != opt_args.end(); ++it) {
        std::cout << "  " << it->second.opt << "\t\t\t" << it->second.help << std::endl; 
      }

      for (it2 = pos_args.begin(); it2 != pos_args.end(); ++it2) {
        std::cout << "  " << it2->second.opt << "\t\t\t" << it2->second.help << std::endl; 
      }
    }

    void add_argument(const std::string& optname, const std::string& help) {
      std::string key;
      if (optname[0] == '-' && optname[1] == '-') {
        key = optname.substr(2, optname.size());
        Arg arg(key, help, optname);
        opt_args[key] = arg;
      } else if (optname[0] == '-') {
        key = lexical_cast<char, std::string>(optname[1]);
        Arg arg(key, help, optname);
        opt_args[key] = arg;
      } else { // positional argument
        key = optname;
        Arg arg(key, help, optname);
        pos_args[n_pos_arg] = arg;
        position_by_key[key] = n_pos_arg;
        n_pos_arg += 1;
      }
    }

    void add_argument(const std::string& optname, const std::string& default_v,
        const std::string& help) {
      std::string key;
      if (optname[0] == '-' && optname[1] == '-') { // long option
        key = optname.substr(2, optname.size());
        Arg arg(key, default_v, help, optname);
        opt_args[key] = arg;
      } else if (optname[0] == '-') { // short option
        key = lexical_cast<char, std::string>(optname[1]);
        Arg arg(key, default_v, help, optname);
        opt_args[key] = arg;
      } else{
        key = optname;
        Arg arg(key, default_v, help, optname);
        pos_args[n_pos_arg] = arg;
        position_by_key[key] = n_pos_arg;
        n_pos_arg += 1;
      }
    }

    void add_argument(const std::string& optname, action_type atype, const std::string& help) {
      std::string key;
      if (optname[0] == '-' && optname[1] == '-') { // long option
        key = optname.substr(2, optname.size());
      } else if (optname[0] == '-') { // short option
        key = lexical_cast<char, std::string>(optname[1]);
      }

      std::string flag;
      switch (atype) {
        case store_true:
          flag = "0";
          break;
        case store_false:
          flag = "1";
          break;
        default:
          break;
      }

      Arg arg(key, flag, help, optname, atype);
      opt_args[key] = arg;
    }


    template<typename T> 
    T get(const std::string& key) {
      std::map<std::string, Arg>::const_iterator it = opt_args.find(key);
      if (it == opt_args.end()) { // not found in optional arguments
        std::map<std::string, int>::const_iterator it2 = position_by_key.find(key);
        if (it2 == position_by_key.end()) { // not found in positional arguments
          std::cerr << "not found:" << key << std::endl;
          T ret = T();
          return ret;
        } else { // found in positional arguments
          int pos_arg = it2->second;
          return lexical_cast<std::string, T>(pos_args[pos_arg].value);
        }

      } else { // found in optional arguments
        return lexical_cast<std::string, T>(it->second.value);
      }
    }

    void parse_args(int argc, char* argv[]) {
      int p = 0;
      cmd = lexical_cast<const char*, std::string>(argv[0]);

      for (int optind=1; optind < argc; ++optind) {
        if (argv[optind][0] == '-') { 
          if (argv[optind][1] == '-') { // long option
            std::string key(argv[optind] + 2, argv[optind] + strlen(argv[optind]));
            if (key == "help") {
              print_help();
              exit(0);
            }
            optind = add_optional_argument(optind, key, argv);
          } else { // short option
            if (argv[optind][1] == 'h') {
              print_help();
              exit(0);
            }
            std::string key = lexical_cast<char, std::string>(argv[optind][1]);
            optind = add_optional_argument(optind, key, argv);
          }
        } else { // positional argument
          std::map<int, Arg>::const_iterator it = pos_args.find(p);
          if (it != pos_args.end()) {
            std::string value = lexical_cast<const char*, std::string>(argv[optind]);
            pos_args[p].value = value;
            p += 1;
          }

        }
      }
    }

  private:
    std::string cmd;
    int n_pos_arg;
    std::map<std::string, Arg> opt_args;
    std::map<int, Arg> pos_args;
    std::map<std::string, int> position_by_key;

    int add_optional_argument(int optind, const std::string& key, char* argv[]) {
      std::map<std::string, Arg>::const_iterator it = opt_args.find(key);
      if (it != opt_args.end()) {
        if (opt_args[key].atype() == store_const) {
          optind += 1;
          std::string value = lexical_cast<const char*, std::string>(argv[optind]);
          opt_args[key].value = value;
        } else {

          if (opt_args[key].atype() == store_true) {
            opt_args[key].value = "1";
          } else {
            opt_args[key].value = "0";
          }

        }
      } else {
        std::cerr << "Unrecognized option:" << argv[optind] << std::endl;
        exit(1);
      }
      return optind;
    }
};


} // namespace argparse
} // namespace strpercpp

#endif // STRPERCPP_ARGPARSE_H
