/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/

// https://github.com/Smoothieware/Smoothieware/blob/edge/src/modules/communication/utils/Gcode.h
#include "Gcode.h"
// #include "libs/StreamOutput.h"
// #include "utils."
// #include "nist_float.h"

#include <stdlib.h>
#include <algorithm>
#include "WString.h"
#include "HardwareSerial.h"

// This is a gcode object. It represents a GCode string/command, and caches some important values about that command for the sake of performance.
// It gets passed around in events, and attached to the queue ( that'll change )
// Gcode::Gcode(const string &command, StreamOutput *stream, bool strip)
Gcode::Gcode(const std::string &command, bool strip)
// Gcode::Gcode(String command, bool strip)
{
    this->command= strdup(command.c_str());
    // Serial.print("\n[Debug]Gcode::Gcode()")
    // Serial.print(command.c_str());
    // Serial.print("-----------");
    // Serial.print(this->command);
    // Serial.print("-----------");
    // Serial.println(this->get_command());

    this->m= 0;
    this->g= 0;
    this->subcode= 0;
    this->add_nl= false;
    this->is_error= false;
    // this->stream= stream;
    prepare_cached_values(strip);
    this->stripped= strip;

    // Serial.print(command.c_str());
    // Serial.print(" ================= ");
    // Serial.print(this->command);
    // Serial.print(" ================= ");
    // Serial.println(this->get_command());
}

Gcode::~Gcode()
{
    if(command != nullptr) {
        // TODO we can reference count this so we share copies, may save more ram than the extra count we need to store
        free(command);
    }
}

Gcode::Gcode(const Gcode &to_copy)
{
    this->command               = strdup(to_copy.command); // TODO we can reference count this so we share copies, may save more ram than the extra count we need to store
    this->has_m                 = to_copy.has_m;
    this->has_g                 = to_copy.has_g;
    this->m                     = to_copy.m;
    this->g                     = to_copy.g;
    this->subcode               = to_copy.subcode;
    this->add_nl                = to_copy.add_nl;
    this->is_error              = to_copy.is_error;
    // this->stream                = to_copy.stream;
    this->txt_after_ok.assign( to_copy.txt_after_ok );
}

Gcode &Gcode::operator= (const Gcode &to_copy)
{
    if( this != &to_copy ) {
        this->command               = strdup(to_copy.command); // TODO we can reference count this so we share copies, may save more ram than the extra count we need to store
        this->has_m                 = to_copy.has_m;
        this->has_g                 = to_copy.has_g;
        this->m                     = to_copy.m;
        this->g                     = to_copy.g;
        this->subcode               = to_copy.subcode;
        this->add_nl                = to_copy.add_nl;
        this->is_error              = to_copy.is_error;
        // this->stream                = to_copy.stream;
        this->txt_after_ok.assign( to_copy.txt_after_ok );
    }
    return *this;
}


// Whether or not a Gcode has a letter
bool Gcode::has_letter( char letter ) const
{
    for (size_t i = 0; i < strlen(this->command); ++i) {
        if( command[i] == letter ) {
            return true;
        }
    }
    return false;
}

// Retrieve the value for a given letter
float Gcode::get_value( char letter, char **ptr ) const
{
    const char *cs = command;
    char *cn = NULL;
    for (; *cs; cs++) {
        if( letter == *cs ) {
            cs++;
            float r = parse_float(cs, &cn);
            if(ptr != nullptr) *ptr= cn;
            if (cn > cs)
                return r;
        }
    }
    if(ptr != nullptr) *ptr= nullptr;
    return 0;
}

int Gcode::get_int( char letter, char **ptr ) const
{
    const char *cs = command;
    char *cn = NULL;
    for (; *cs; cs++) {
        if( letter == *cs ) {
            cs++;
            int r = strtol(cs, &cn, 10);
            if(ptr != nullptr) *ptr= cn;
            if (cn > cs)
                return r;
        }
    }
    if(ptr != nullptr) *ptr= nullptr;
    return 0;
}

uint32_t Gcode::get_uint( char letter, char **ptr ) const
{
    const char *cs = command;
    char *cn = NULL;
    for (; *cs; cs++) {
        if( letter == *cs ) {
            cs++;
            int r = strtoul(cs, &cn, 10);
            if(ptr != nullptr) *ptr= cn;
            if (cn > cs)
                return r;
        }
    }
    if(ptr != nullptr) *ptr= nullptr;
    return 0;
}

int Gcode::get_num_args() const
{
    int count = 0;
    for(size_t i = stripped?0:1; i < strlen(command); i++) {
        if( this->command[i] >= 'A' && this->command[i] <= 'Z' ) {
            if(this->command[i] == 'T') continue;
            count++;
        }
    }
    return count;
}

std::map<char,float> Gcode::get_args() const
{
    std::map<char,float> m;
    for(size_t i = stripped?0:1; i < strlen(command); i++) {
        char c= this->command[i];
        if( c >= 'A' && c <= 'Z' ) {
            if(c == 'T') continue;
            m[c]= get_value(c);
        }
    }
    return m;
}

std::map<char,int> Gcode::get_args_int() const
{
    std::map<char,int> m;
    for(size_t i = stripped?0:1; i < strlen(command); i++) {
        char c= this->command[i];
        if( c >= 'A' && c <= 'Z' ) {
            if(c == 'T') continue;
            m[c]= get_int(c);
        }
    }
    return m;
}

// Cache some of this command's properties, so we don't have to parse the string every time we want to look at them
void Gcode::prepare_cached_values(bool strip)
{
    char *p= nullptr;
    if( this->has_letter('G') ) {
        this->has_g = true;
        this->g = this->get_int('G', &p);

    } else {
        this->has_g = false;
    }

    if( this->has_letter('M') ) {
        this->has_m = true;
        this->m = this->get_int('M', &p);

    } else {
        this->has_m = false;
    }

    if(has_g || has_m) {
        // look for subcode and extract it
        if(p != nullptr && *p == '.') {
            this->subcode = strtoul(p+1, &p, 10);

        }else{
            this->subcode= 0;
        }
    }

    if(!strip) return;

    // remove the Gxxx or Mxxx from string
    if (p != nullptr) {
        char *n= strdup(p); // create new string starting at end of the numeric value
        free(command);
        command= n;
    }

}

// strip off X Y Z I J K parameters if G0/1/2/3
void Gcode::strip_parameters()
{
    if(has_g && g < 4){
        // strip the command of the XYZIJK parameters
        std::string newcmd;
        char *cn= command;
        // find the start of each parameter
        char *pch= strpbrk(cn, "XYZIJK");
        while (pch != nullptr) {
            if(pch > cn) {
                // copy non parameters to new string
                newcmd.append(cn, pch-cn);
            }
            // find the end of the parameter and its value
            char *eos;
            parse_float(pch+1, &eos);
            cn= eos; // point to end of last parameter
            pch= strpbrk(cn, "XYZIJK"); // find next parameter
        }
        // append anything left on the line
        newcmd.append(cn);

        // strip whitespace to save even more, this causes problems so don't do it
        //newcmd.erase(std::remove_if(newcmd.begin(), newcmd.end(), ::isspace), newcmd.end());

        // release the old one
        free(command);
        // copy the new shortened one
        command= strdup(newcmd.c_str());
    }
}

// https://github.com/Smoothieware/Smoothieware/blob/f01177614a495473d4018cb00e06416a6c0194f1/src/libs/nist_float.cpp


float Gcode::parse_float(const char* nptr, char** endptr) const
{
	const int kStrtofMaxDigits = 8;
	const char *p = nptr;
	// Skip leading white space, if any. Not necessary
	while (isspace(*p) ) ++p;

	// Get sign, if any.
	bool sign = true;
	if (*p == '-') {
		sign = false; ++p;
	} else if (*p == '+') {
		++p;
	}

	// Get digits before decimal point, if any.
	uint32_t predec;  // to store digits before decimal point
	for (predec = 0; isdigit(*p); ++p) {
		predec = predec * 10UL + static_cast<uint32_t>(*p - '0');
	}
	float value = static_cast<float>(predec);

	// Get digits after decimal point, if any.
	if (*p == '.') {
		uint32_t pow10 = 1;
		uint32_t val2 = 0;
		int digit_cnt = 0;
		++p;
		while (isdigit(*p)) {
			if (digit_cnt < kStrtofMaxDigits) {
				val2 = val2 * 10UL + static_cast<uint32_t>(*p - '0');
				pow10 *= 10UL;
			}  // when kStrtofMaxDigits is read, ignored following digits
			++p;
			++digit_cnt;
		}
		value += static_cast<float>(
		             static_cast<double>(val2) / static_cast<double>(pow10));
	}

// #pragma GCC diagnostic ignored "-Wcast-qual"
	if (endptr) *endptr = (char*)p;  // NOLINT(*)
	return sign ? value : - value;
}