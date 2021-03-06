#pragma once
#ifndef DATAMANAGEMENT_H
#define DATAMANAGEMENT_H

#include "string"

class DataManager
{
    public:
        static void validate_key(std::string key);

        static void incr_key(std::string key, int x);
        static void incr_key(std::string key, double x);
        static void decr_key(std::string key, int x);
        static void decr_key(std::string key, double x);

        static int get_int_from_data(std::string key, int _default=0);
        static void set_int_from_data(std::string key, int val);

        static bool get_bool_from_data(std::string key, bool _default=false);
        static void set_bool_from_data(std::string key, bool val);

        static double get_double_from_data(std::string key, double _default=0.0);
        static void set_double_from_data(std::string key, double val);

        static std::string get_string_from_data(std::string key, std::string _default="");
        static void set_string_from_data(std::string key, std::string val);

};

#endif
