/*
 * SPDX-FileName: ArgumentParser.hxx
 * SPDX-FileComment: FlightGear viewer argument parser
 * SPDX-FileCopyrightText: Copyright (C) 2021 by Erik Hofman
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <map>
#include <vector>

#include <osg/ArgumentParser>

class ArgumentParser
{
public:
    ArgumentParser(int argc, char** argv);
    ~ArgumentParser() = default;

    bool read(const char* arg);
    bool read(const char* arg, std::string& value);
    bool read(const char* arg, std::string& name, std::string& value);

    int argc() { return files.size(); }
    std::string& getApplicationName() { return appName; }

    void reportRemainingOptionsAsUnrecognized();
    void writeErrorMessages(std::ostream& output);

    osg::ArgumentParser& osg() { return arguments; }

    operator std::vector<std::string>&() { return files; }

private:
    int arg_num;
    char** arg_values;
    std::string appName;

    std::vector<std::string> errors;
    std::map<std::string, std::string> args;
    std::vector<std::string> files;

    osg::ArgumentParser arguments;
};
