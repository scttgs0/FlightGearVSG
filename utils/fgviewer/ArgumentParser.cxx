/*
 * SPDX-FileName: ArgumentParser.cxx
 * SPDX-FileComment: FlightGear viewer argument parser
 * SPDX-FileCopyrightText: Copyright (C) 2021 by Erik Hofman
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <cstring>
#include <iostream>
#include <map>
#include <string>

#include "ArgumentParser.hxx"

ArgumentParser::ArgumentParser(int argc, char** argv) : arg_num(argc),
                                                        arg_values(argv),
                                                        appName(argv[0]),
                                                        arguments(osg::ArgumentParser(&arg_num, arg_values))
{
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        std::string val;

        if (arg[0] == '-') {
            std::size_t pos = arg.find('=');
            if (pos != std::string::npos) {
                val = arg.substr(pos + 1);
                arg = arg.substr(0, pos);
            } else if ((i + 1) < argc && argv[i + 1][0] != '-') {
                val = argv[++i];
            } else {
                val = "";
            }

            args[arg] = val;
        } else {
            files.push_back(arg);
        }
    }
}

bool ArgumentParser::read(const char* arg)
{
    auto it = args.find(arg);
    if (it != args.end()) {
        args.erase(it);
        return true;
    }
    return false;
}

bool ArgumentParser::read(const char* arg, std::string& value)
{
    auto it = args.find(arg);
    if (it != args.end()) {
        value = it->second;
        args.erase(it);
        return true;
    }
    return false;
}

bool ArgumentParser::read(const char* arg, std::string& name, std::string& value)
{
    auto it = args.find(arg);

    // old fgviewer behavior: --prop name=value
    if (it != args.end()) {
        std::string str = it->second;

        std::size_t pos = str.find("=");
        if (pos != std::string::npos && pos != str.size()) {
            value = str.substr(pos);
            name = str.substr(0, pos);
            args.erase(it);
            return true;
        }
    }

    // FlightGear behavior: --prop:name=value
    std::string str = name;
    str += ':';
    str += name;

    it = args.find(str);
    if (it != args.end()) {
        value = it->second;
        args.erase(it);
        return true;
    }

    return false;
}

void ArgumentParser::reportRemainingOptionsAsUnrecognized()
{
    for (auto it : args) {
        std::cerr << "Unsupported argument: " << it.first << std::endl;
    }
}

void ArgumentParser::writeErrorMessages(std::ostream& output)
{
}
