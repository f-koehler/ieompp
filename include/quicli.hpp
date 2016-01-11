#ifndef QUICLI_HPP_
#define QUICLI_HPP_

#include <algorithm>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

namespace quicli
{
    using Occurance = std::vector<std::string>;
    using ValueMap  = std::map<std::string, std::vector<Occurance>>;

    std::vector<std::string> convert(int argc, char** argv)
    {
        auto list = std::vector<std::string>(argv + 1, argv + argc);
        for(auto iter = list.begin(); iter != list.end(); ++iter) {
            auto pos = std::find(iter->begin(), iter->end(), '=');
            if(pos == iter->end()) continue;
            auto index = pos - iter->begin();
            auto left  = iter->substr(0, index);
            auto right = iter->substr(index + 1);
            list.insert(iter, left);
            *iter = right;
        }
        return list;
    }

    class Argument
    {
        protected:
            std::vector<std::string> _names;
            int _priority;
            std::tuple<bool, std::string> _default;

        public:
            Argument(std::initializer_list<std::string> names) : _names(names) {}

            Argument& priority(int val)
            {
                _priority = val;
                return *this;
            }
            Argument& default_value(const std::string& val)
            {
                std::get<1>(_default) = val;
                return *this;
            }

            std::tuple<bool, std::size_t> match(const std::string& str) const
            {
                auto iter = std::find(_names.begin(), _names.end(), str);
                if(iter == _names.end()) return std::make_tuple(false, 0);
                return std::make_tuple(true, iter - _names.begin());
            }

            virtual void extract(const std::tuple<bool, std::size_t>& match,
                                 std::vector<std::string>& args, ValueMap vm) const = 0;
    };

    class Flag : public Argument
    {
        public:
            using Argument::Argument;

            virtual void extract(const std::tuple<bool, std::size_t>& match,
                                 std::vector<std::string>& args, ValueMap vm) const
            {
                static auto count = 0;
                if(!std::get<0>(match)) return;
                args.erase(args.begin());
                ++count;
                auto pos = vm.find(_names.front());
                if(pos != vm.end()) {
                    pos->second[0][0] = std::to_string(count);
                    return;
                }
                vm.insert(std::make_pair(_names.front(),
                                         std::vector<Occurance>{{std::to_string(count)}}));
            }
    };

    class Value : public Argument
    {
        private:
            std::size_t _num_vals;
            bool _multi;

        public:
            using Argument::Argument;

            Value& num_values(std::size_t val)
            {
                _num_vals = val;
                return *this;
            }
            Value& multi(bool val)
            {
                _multi = val;
                return *this;
            }

            virtual void extract(const std::tuple<bool, std::size_t>& match,
                                 std::vector<std::string>& args, ValueMap vm) const
            {
                if(!std::get<0>(match)) return;
                if(_num_vals > args.size())
                    throw std::runtime_error("TODO"); // TODO
                auto pos = vm.find(_names.front());
                if(pos == vm.end())
                    vm.insert(std::make_pair(_names.front(), std::vector<Occurance>()));
                vm[_names.front()].emplace_back(Occurance(args.begin(), args.begin()+_num_vals));
            }
    };

    class Prompt : public Argument
    {
        protected:
            std::function<std::string(void)> _prompt;

        public:
            using Argument::Argument;

            Prompt(std::initializer_list<std::string> names, std::function<std::string(void)> f)
                : Argument(names), _prompt(f)
            {
            }
    };

    class CLI
    {
        private:
            std::vector<std::unique_ptr<Argument>> _args;

        public:
            template<typename T>
            T& add(const T& arg) {
                _args.push_back(std::unique_ptr<Argument>(new T(arg)));
                return *dynamic_cast<T*>(_args.back().get());
            }
    };
}

#endif
