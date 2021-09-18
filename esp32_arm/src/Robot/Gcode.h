/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef _GCODE_H_
#define _GCODE_H_

#include <string>
#include <map>

#include "GcodeConst.h"
using std::string;


// Object to represent a Gcode command
class Gcode {
    public:
        Gcode(const string&, bool strip=true);
        Gcode(const Gcode& to_copy);
        Gcode& operator= (const Gcode& to_copy);
        ~Gcode();

        const char* get_command() const { return command; }
        bool has_letter ( char letter ) const;
        float get_value ( char letter, char **ptr= nullptr ) const;
        int get_int ( char letter, char **ptr= nullptr ) const;
        uint32_t get_uint ( char letter, char **ptr= nullptr ) const;
        int get_num_args() const;
        std::map<char,float> get_args() const;
        std::map<char,int> get_args_int() const;
        void strip_parameters();

        // FIXME these should be private
        unsigned int m;
        unsigned int g;

        struct {
            bool add_nl:1;
            bool has_m:1;
            bool has_g:1;
            bool stripped:1;
            bool is_error:1;
            uint8_t subcode:3;
        };

        // StreamOutput* stream;
        string txt_after_ok;

    private:
        void prepare_cached_values(bool strip=true);
        char *command;

        //functions from mBED system.
        char * xxx;
        unsigned int strlen(char* var) const {return 5;};
        char* strdup(const char* var ){return xxx;};
        float parse_float(const char* v1, char** v2) const {return 1.0f;};
        char* strpbrk(char* v1,char* v2) {return xxx;};
};
#endif