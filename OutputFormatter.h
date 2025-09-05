#pragma once

#include <sstream>
#include <vector>

using std::vector;
using std::string;
using std::stringstream;

#define DEFAULT_LENGTH 100

class OutputFormatter
{
private:
    vector<string> lines;
    int default_length = 100;

public:
    OutputFormatter(string raw_text, int default_length = DEFAULT_LENGTH)
    {
        lines.push_back(raw_text); 
        this->default_length = default_length;
    }

    OutputFormatter(vector<string> lines, int default_length = DEFAULT_LENGTH)
    {
        this->lines = lines; 
        this->default_length = default_length;
    }

    string GetResult()
    {
        string edges = "";
        string edge_top_left_corner = "╔";
        string edge_top_right_corner = "╗";
        string edge_bottom_left_corner = "╚";
        string edge_bottom_right_corner = "╝";
        string edge_side = "═";

        for(int i = 0; i < default_length - 2; i++)
            edges += edge_side;

        string top_edge = edge_top_left_corner + edges + edge_top_right_corner;
        string bottom_edge = edge_bottom_left_corner + edges + edge_bottom_right_corner;
        
        vector<string> middle_lines;
        string middle_edge = "║";

        for(int i = 0; i < lines.size(); i++)
        {
            string middle_space_left = "";
            string middle_space_right = "";
            int num_chars_left;
            int num_chars_right;
            
            if((default_length - lines[i].size()) % 2 == 1)
            {
                //handle case for odd-length lines.
                int num_chars = (default_length - lines[i].size()) / 2;
                num_chars_left = num_chars;
                num_chars_right = num_chars + 1;
            }   
            else
            {
                //handle case for even-length lines
                int num_chars = (default_length - lines[i].size()) / 2;
                num_chars_left = num_chars_right = num_chars;
            }
        
            for(int i = 0; i < num_chars_left - 1; i++)
                middle_space_left += " ";
            for(int i = 0; i < num_chars_right - 1; i++)
                middle_space_right += " ";
            string middle = middle_edge + middle_space_left + lines[i] + middle_space_right + middle_edge;
           
            middle_lines.push_back(middle);

        }

        
        string result = "";
        result += top_edge+ "\n";
        for(int i = 0; i < middle_lines.size(); i++)
            result += middle_lines[i] + "\n";
        result += bottom_edge + "\n";

        return result;
    }
};
